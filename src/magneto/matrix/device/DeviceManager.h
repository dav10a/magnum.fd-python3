/*
 * Copyright 2012-2014 by the MicroMagnum Team
 * Copyright 2014 by the magnum.fd Team
 *
 * This file is part of magnum.fd.
 * magnum.fd is based heavily on MicroMagnum.
 * (https://github.com/MicroMagnum/MicroMagnum)
 * 
 * magnum.fd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * magnum.fd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with magnum.fd. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "config.h"
#include <vector>

namespace matty {

class Device;

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	int addCPUDevice();
	int addCUDADevice(int cuda_device_id);

	Device *getDevice(int id) const { return devices[id]; }
	int getNumDevices() const { return devices.size(); }

private:
	std::vector<Device*> devices;
};

} // ns

#endif
