/* Copyright (C) 2002-2008 Claudia Emde <claudia.emde@dlr.de>
                      
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
     \author Claudia Emde <claudia.emde@dlr.de>
     \date   Thu May  23 14:34:05 2002
     
     \brief  Internal cloudbox functions.
     
   */

#ifndef cloudbox_h
#define cloudbox_h

#include "matpackVII.h"
#include "interpolation.h"
#include "optproperties.h"
#include "array.h"
#include "gridded_fields.h"
#include "ppath.h"

void chk_if_pnd_zero_p (const Index& i_p,
                        const GriddedField3& pnd_field_raw,
                        const String& pnd_field_file);

void chk_if_pnd_zero_lat (const Index& i_lat,
                          const GriddedField3& pnd_field_raw,
                          const String& pnd_field_file);

void chk_if_pnd_zero_lon (const Index& i_lon,
                          const GriddedField3& pnd_field_raw,
                          const String& pnd_field_file);

void chk_pnd_data (const GriddedField3& pnd_field_raw,
                   const String& pnd_field_file,
                   const Index& atmosphere_dim,
                   ConstVectorView p_grid,
                   ConstVectorView lat_grid,
                   ConstVectorView lon_grid,
                   const ArrayOfIndex& cloudbox_limits);

void chk_pnd_raw_data (const ArrayOfGriddedField3& pnd_field_raw,
                       const String& pnd_field_file,
                       const Index& atmosphere_dim,
                       ConstVectorView p_grid,
                       ConstVectorView lat_grid,
                       ConstVectorView lon_grid,
                       const ArrayOfIndex& cloudbox_limits);

void chk_scattering_data (const ArrayOfSingleScatteringData& scat_data_raw,
                          const ArrayOfScatteringMetaData& scat_data_meta_array
                          );

void chk_scattering_meta_data (const ScatteringMetaData& scat_data_meta,
                               const String& scat_data_meta_file
                               );

void chk_single_scattering_data (const SingleScatteringData& scat_data_raw,
                                 const String& scat_data_file,
                                 ConstVectorView f_grid);

bool is_gp_inside_cloudbox (const GridPos& gp_p,
                            const GridPos& gp_lat,
                            const GridPos& gp_lon,
                            const ArrayOfIndex& cloudbox_limits,
                            const bool include_boundaries);

bool is_inside_cloudbox (const Ppath& ppath_step,
                         const ArrayOfIndex& cloudbox_limits,
                         const bool include_boundaries);

Numeric barometric_heightformula (const Numeric p,
                                  const Numeric dh,
                                  const Numeric T0
                                  );

Numeric IWCtopnd_MH97 (const Numeric iwc,
                       Numeric dm,
                       const Numeric t,
                       const Numeric density);

Numeric LWCtopnd (const Numeric lwc,
                  //const Numeric density,
                  const Numeric r);

Numeric LWCtopnd2 (//const Numeric density,
                   const Numeric r);


void trapezoid_integrate (Vector& w,
                          const Vector& x,
                          const Vector& y);

void chk_pndsum (Vector& pnd,
                 const Numeric xwc,
                 const Vector density,
                 const Vector vol);

bool chk_hydromet_field (const Index&  dim,	
                         const Tensor3& hydromet,			 
                         const Vector& p_grid,
                         const Vector& lat_grid,
                         const Vector& lon_grid);


#endif //cloudbox_h

