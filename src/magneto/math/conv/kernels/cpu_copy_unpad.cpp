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

#include "cpu_copy_unpad.h"

static void cpu_copy_unpad_c2r_comp(
	int dim_x, int dim_y, int dim_z,
	int red_x,
	const double *in,  // size: dim_x * dim_y * dim_z
	      double *out) // size: red_x * dim_y * dim_z (cut in x-direction from dim_x -> red_x)
{
	const int  in_stride_x = 2 * 1;
	const int  in_stride_y = 2 * dim_x;
	const int  in_stride_z = 2 * dim_x * dim_y;

	const int out_stride_x = 1 * 1;
	const int out_stride_y = 1 * red_x;
	const int out_stride_z = 1 * red_x * dim_y;

	// (x,y,z) loop through the out matrix indices
	for (int z=0; z<dim_z; ++z)
	for (int y=0; y<dim_y; ++y)
	for (int x=0; x<red_x; ++x) {
		      double *dst = out + z*out_stride_z + y*out_stride_y + x*out_stride_x;
		const double *src =  in + z* in_stride_z + y* in_stride_y + x* in_stride_x;
		dst[0] = src[0];
	}
}

void cpu_copy_unpad_c2r(
	int dim_x, int dim_y, int dim_z,
	int red_x,
	const double *in_x, const double *in_y, const double *in_z,
	      double *out_x,      double *out_y,      double *out_z)
{
	cpu_copy_unpad_c2r_comp(dim_x, dim_y, dim_z, red_x, in_x, out_x);
	cpu_copy_unpad_c2r_comp(dim_x, dim_y, dim_z, red_x, in_y, out_y);
	cpu_copy_unpad_c2r_comp(dim_x, dim_y, dim_z, red_x, in_z, out_z);
}

void cpu_copy_unpad_c2r(
	int dim_x, int dim_y, int dim_z,
	int red_x,
	const double  *in_x,
	      double *out_x)
{
	cpu_copy_unpad_c2r_comp(dim_x, dim_y, dim_z, red_x, in_x, out_x);
}

static void cpu_copy_unpad_r2r_comp(
	int dim_x, int dim_y, int dim_z,
	int red_x,
	const double *in,  // size: dim_x * dim_y * dim_z
	      double *out) // size: red_x * dim_y * dim_z (cut in x-direction from dim_x -> red_x)
{
	const int  in_stride_x = 1 * 1;
	const int  in_stride_y = 1 * dim_x;
	const int  in_stride_z = 1 * dim_x * dim_y;

	const int out_stride_x = 1 * 1;
	const int out_stride_y = 1 * red_x;
	const int out_stride_z = 1 * red_x * dim_y;

	// (x,y,z) loop through the out matrix indices
	for (int z=0; z<dim_z; ++z)
	for (int y=0; y<dim_y; ++y)
	for (int x=0; x<red_x; ++x) {
		      double *dst = out + z*out_stride_z + y*out_stride_y + x*out_stride_x;
		const double *src =  in + z* in_stride_z + y* in_stride_y + x* in_stride_x;
		dst[0] = src[0];
	}
}
    
void cpu_copy_unpad_r2r(
	int dim_x, int dim_y, int dim_z,
	int red_x,
	const double *in_x, const double *in_y, const double *in_z,
	      double *out_x,      double *out_y,      double *out_z)
{
	cpu_copy_unpad_r2r_comp(dim_x, dim_y, dim_z, red_x, in_x, out_x);
	cpu_copy_unpad_r2r_comp(dim_x, dim_y, dim_z, red_x, in_y, out_y);
	cpu_copy_unpad_r2r_comp(dim_x, dim_y, dim_z, red_x, in_z, out_z);
}

void cpu_copy_unpad_r2r(
	int dim_x, int dim_y, int dim_z,
	int red_x,
	const double * in_x,
	      double *out_x)
{
	cpu_copy_unpad_r2r_comp(dim_x, dim_y, dim_z, red_x, in_x, out_x);
}
