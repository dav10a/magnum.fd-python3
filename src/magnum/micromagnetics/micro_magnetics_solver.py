# Copyright 2012-2014 by the MicroMagnum Team
# Copyright 2014 by the magnum.fd Team
#
# This file is part of magnum.fd.
# magnum.fd is based heavily on MicroMagnum.
# (https://github.com/MicroMagnum/MicroMagnum)
#
# magnum.fd is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# magnum.fd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with magnum.fd. If not, see <http://www.gnu.org/licenses/>.

from __future__ import print_function

import math
import numpy as np

import magnum.tools as tools
import magnum.solver as solver

from magnum.micromagnetics.io import writeOMF
from magnum.micromagnetics.stephandler import ScreenLog, ScreenLogMinimizer
from magnum.mesh import VectorField

class MicroMagneticsSolver(solver.Solver):
    def __init__(self, system, evolver, world):
        super(MicroMagneticsSolver, self).__init__(system, evolver)
        self.__world = world

    def __repr__(self):
        return "MicroMagneticsSolver@%s" % hex(id(self))

    world = property(lambda self: self.__world)

    def relax(self, *args, **kwargs):
        # catch CVode, when using relax condition
        if self.evolver.__class__.__name__ == "Cvode":
          raise Exception("CVode is not usable to relax a system, yet. Please use rkf45.")

        return self.solve(solver.condition.Relaxed(*args, **kwargs))

    def minimize(self, max_dpns = 0.01, samples = 10, h_max = 1e-5, h_min = 1e-16):
        """
        Minimizes the energy with a direct minimization strategy.
        """
        # TODO make use of stephandlers for logging
        h        = self.state.h
        dpnslist = []
        log      = ScreenLogMinimizer()

        # Reset step
        self.state.step = 0

        while len(dpnslist) < samples or max(dpnslist) > max_dpns:
            # Calculate next M and dM for minimization step
            M_next = self.state.minimizer_M(h)
            dM = self.state.minimizer_dM

            # Get s^n-1 for step-size calculation
            M_diff = VectorField(self.mesh)
            M_diff.assign(M_next)
            M_diff.add(self.state.M, -1.0)

            # Set next M
            self.state.y = M_next
            self.state.finish_step() # normalize, TODO really need to do this every step?
            self.state.flush_cache()

            # Calculate deg per ns
            # TODO M.absMax might be the wrong choice if different materials are in use
            dp_timestep = (180.0 / math.pi) * math.atan2(M_diff.absMax(), self.state.M.absMax())
            dpns = abs(1e-9 * dp_timestep / h)
            dpnslist.append(dpns)
            if len(dpnslist) > samples: dpnslist.pop(0)
            self.state.deg_per_ns_minimizer = dpns

            # Get y^n-1 for step-size calculation
            dM_diff = VectorField(self.mesh)
            dM_diff.assign(self.state.minimizer_dM)
            dM_diff.add(dM, -1.0)

            # Next stepsize (Alternate h1 and h2)
            try:
              if (self.state.step % 2 == 0):
                h = M_diff.dotSum(M_diff) / M_diff.dotSum(dM_diff)
              else:
                h = M_diff.dotSum(dM_diff) / dM_diff.dotSum(dM_diff)
            except (ZeroDivisionError, ex):
              h = h_max

            h_sign = math.copysign(1, h)
            h = max(min(abs(h), h_max), h_min) * h_sign

            if (self.state.step % 100 == 0):
              log.handle(self.state)

            # Update step
            self.state.step += 1

    def handle_sigint(self):
        print()

        text = ""
        text += "State:\n"
        text += "       step = %s\n" % self.state.step
        text += "          t = %s\n" % self.state.t
        text += "     avg(M) = %s\n" % (self.state.M.average(),)
        text += " deg_per_ns = %s\n" % self.state.deg_per_ns
        text += "\n"
        text += "Mesh: %s\n" % self.mesh
        text += "\n"
        text += "Options:"

        loggers = [h for (h, _) in self.step_handlers if isinstance(h, ScreenLog)]

        answer = tools.interactive_menu(
            header="Solver interrupted by signal SIGINT (ctrl-c)",
            text=text,
            options=[
                "Continue",
                "Stop solver and return the current state as the result",
                "Save current magnetization to .omf file, then continue",
                "Raise KeyboardInterrupt",
                "Kill program",
                "Start debugger",
                "Toggle console log (now:%s)" % ("enabled" if loggers else "disabled")
            ]
        )
        if answer == 1:
            return
        elif answer == 2:
            raise solver.Solver.FinishSolving()
        elif answer == 3:
            print("Enter file name ('.omf' is appended automatically)")
            path = tools.getline() + ".omf"
            writeOMF(path, self.state.M)
            print("Done.")
            return False
        elif answer == 4:
            raise KeyboardInterrupt()
        elif answer == 5:
            import sys
            sys.exit(-1)
        elif answer == 6:
            raise solver.Solver.StartDebugger()
        elif answer == 7:
            if loggers:
                for logger in loggers:
                    self.removeStepHandler(logger)
                print("Disabled console log.")
            else:
                from magnum.solver.condition import EveryNthStep
                self.addStepHandler(ScreenLog(), EveryNthStep(100))
                print("Enabled console log.")
            return
        assert False
