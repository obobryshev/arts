/* Copyright (C) 2002 Claudia Emde <claudia@sat.physik.uni-bremen.de>
                      
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. 
*/

 /*!
     \file   cloudbox.h
     \author Claudia Emde <claudia@sat.physik.uni-bremen.de>
     \date   Thu May  23 14:34:05 2002
     
     \brief  Internal cloudbox functions.
     
   */

#ifndef cloudbox_h
#define cloudbox_h

#include "matpackVII.h"
#include "interpolation.h"

void iy_interp_cloudbox_field(
            Matrix&         iy,
      const Tensor7&        scat_i_p,
      const Tensor7&        scat_i_lat,
      const Tensor7&        scat_i_lon,
      const GridPos&        rte_gp_p,
      const GridPos&        rte_gp_lat,
      const GridPos&        rte_gp_lon,
      const Vector&         rte_los,
      const Index&          cloudbox_on,
      const ArrayOfIndex&   cloudbox_limits,
      const Index&          atmosphere_dim,
      const Index&          stokes_dim,
      const Vector&         scat_za_grid,
      const Vector&         scat_aa_grid,
      const Vector&         f_grid,
      const String&         interpmeth );

#endif //cloudbox_h

