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

#ifndef TRANSPOSE_UNPAD_2D_H
#define TRANSPOSE_UNPAD_2D_H

// XY -> xY
void cuda_transpose_unpad_c2c_2d(
	int dim_x, int dim_y, // input size
	int red_x, // red_x <= dim_x
	const float *in_x, const float *in_y, const float *in_z, // size: dim_x * dim_y
	      float *out_x, float *out_y, float *out_z);         // size: dim_y * red_x

// XY -> xY, scalar version
void cuda_transpose_unpad_c2c_2d(
	int dim_x, int dim_y, // input size
	int red_x, // red_x <= dim_x
	const float *in,   // size: dim_x * dim_y
	      float *out); // size: dim_y * red_x

#endif
