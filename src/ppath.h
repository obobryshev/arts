/* Copyright (C) 2002-2008 Patrick Eriksson <Patrick.Eriksson@chalmers.se>

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
   USA. */



/*===========================================================================
  ===  File description 
  ===========================================================================*/

/*!
  \file   ppath.h
  \author Patrick Eriksson <Patrick.Eriksson@chalmers.se>
  \date   2002-05-02
  
  \brief  Propagation path structure and functions.
  
   This file contains the definition of the Ppath structure and the
   functions in ppath.cc that are of interest elsewhere.
*/


#ifndef ppath_h
#define ppath_h


#include "agenda_class.h"
#include "array.h"
#include "arts.h"
#include "interpolation.h"
#include "matpackI.h"
#include "mystring.h"



/*===========================================================================
  === The Ppath structure
  ===========================================================================*/

//! The structure to describe a propagation path and releated quantities.
/*! 
   The fields of the structure are described in the ARTS user guide (AUG).
   It is listed as a sub-entry to "data structures".  
*/

struct Ppath {
  Index             dim;
  Index             np;
  Numeric           constant;
  String            background;
  Matrix            pos;
  Matrix            los;
  Vector            r;
  Vector            l_step;
  Numeric           lspace;
  Vector            nreal;
  ArrayOfGridPos    gp_p;
  ArrayOfGridPos    gp_lat;
  ArrayOfGridPos    gp_lon;
};


/** An array of propagation paths. */
typedef Array<Ppath> ArrayOfPpath;



/*===========================================================================
  === Common precision variables
  ===========================================================================*/

// This variable defines the maximum allowed error tolerance for radius.
// The variable is, for example, used to check that a given a radius is
// consistent with the specified grid cell.
//
const Numeric   RTOL = 1e-3;


// As RTOL but for latitudes and longitudes.
//
const Numeric   LATLONTOL = 1e-8;


// This variable defines how much zenith and azimuth angles can
// deviate from 0, 90 and 180 degrees, but still be treated to be 0,
// 90 or 180.  For example, an azimuth angle of 180-ANGTOL/2 will
// be treated as a strictly southward observation.  However, the
// angles are not allowed to go outside their defined range.  This
// means, for example, that values above 180 are never allowed.
//
const Numeric   ANGTOL = 1e-6; 


// Latitudes with an absolute value > POLELAT are considered to be on
// the south or north pole for 3D.
//
const Numeric   POLELAT = 90-1e-8;


// Maximum tilt of pressure levels, in degrees
//
const Numeric    PTILTMAX = 5;



/*===========================================================================
  === Functions from ppath.cc
  ===========================================================================*/

void map_daa(
             Numeric&   za,
             Numeric&   aa,
       const Numeric&   za0,
       const Numeric&   aa0,
       const Numeric&   aa_grid );

Numeric geometrical_ppc( const Numeric& r, const Numeric& za );

Numeric geompath_za_at_r(
        const Numeric&   ppc,
        const Numeric&   a_za,
        const Numeric&   r );

Numeric geompath_lat_at_za(
        const Numeric&   za0,
        const Numeric&   lat0,
        const Numeric&   za );

bool is_los_downwards( 
        const Numeric&   za,
        const Numeric&   tilt );

Numeric plevel_slope_2d(
        ConstVectorView   lat_grid,           
        ConstVectorView   refellipsoid,
        ConstVectorView   z_surf,
        const GridPos&    gp,
        const Numeric&    za );

Numeric plevel_slope_3d(
        const Numeric&   lat1,
        const Numeric&   lat3,
        const Numeric&   lon5,
        const Numeric&   lon6,
        const Numeric&   r15,
        const Numeric&   r35,
        const Numeric&   r36,
        const Numeric&   r16,
        const Numeric&   lat,
        const Numeric&   lon,
        const Numeric&   aa );

Numeric plevel_slope_3d(
        ConstVectorView   lat_grid,
        ConstVectorView   lon_grid,  
        ConstVectorView   refellipsoid,
        ConstMatrixView   z_surf,
        const GridPos&    gp_lat,
        const GridPos&    gp_lon,
        const Numeric&    aa );

Numeric plevel_angletilt(
        const Numeric&   r,
        const Numeric&   c );

void ppath_init_structure( 
              Ppath&      ppath,
        const Index&      atmosphere_dim,
        const Index&      np );

void ppath_set_background( 
              Ppath&      ppath,
        const Index&      case_nr );

Index ppath_what_background( const Ppath&   ppath );

void ppath_copy(
              Ppath&      ppath1,
        const Ppath&      ppath2 );

void ppath_start_stepping(
              Ppath&            ppath,
        const Index&            atmosphere_dim,
        ConstVectorView         p_grid,
        ConstVectorView         lat_grid,
        ConstVectorView         lon_grid,
        ConstTensor3View        z_field,
        ConstVectorView         refellipsoid,
        ConstMatrixView         z_surface,
        const Index &           cloudbox_on,
        const ArrayOfIndex &    cloudbox_limits,
        const bool &            outside_cloudbox,
        ConstVectorView         rte_pos,
        ConstVectorView         rte_los,
        const Verbosity&        verbosity);


void ppath_step_geom_1d(
              Ppath&      ppath,
        ConstVectorView   z_field,
        ConstVectorView   refellipsoid,
        const Numeric&    z_surface,
        const Numeric&    lmax );

void ppath_geom_updown_1d(
              Ppath&      ppath,
        ConstVectorView   z_field,
        ConstVectorView   refellipsoid,
        const Numeric&    z_surface,
        const Index&      cloudbox_on, 
     const ArrayOfIndex&  cloudbox_limits );

void ppath_step_geom_2d(
              Ppath&      ppath,
        ConstVectorView   lat_grid,
        ConstMatrixView   z_field,
        ConstVectorView   refellipsoid,
        ConstVectorView   z_surface,
        const Numeric&    lmax );

void ppath_step_geom_3d(
              Ppath&       ppath,
        ConstVectorView    lat_grid,
        ConstVectorView    lon_grid,
        ConstTensor3View   z_field,
        ConstVectorView    refellipsoid,
        ConstMatrixView    z_surface,
        const Numeric&     lmax );

void ppath_step_refr_1d(
              Workspace&  ws,
              Ppath&      ppath,
              Numeric&    rte_pressure,
              Numeric&    rte_temperature,
              Vector&     rte_vmr_list,
              Numeric&    refr_index,
        const Agenda&     refr_index_agenda,
        ConstVectorView   p_grid,
        ConstVectorView   z_field,
        ConstVectorView   t_field,
        ConstMatrixView   vmr_field,
        ConstVectorView   refellipsoid,
        const Numeric&    z_surface,
        const String&     rtrace_method,
        const Numeric&    lraytrace,
        const Numeric&    lmax );

void ppath_step_refr_2d(
              Workspace&  ws,
              Ppath&      ppath,
              Numeric&    rte_pressure,
              Numeric&    rte_temperature,
              Vector&     rte_vmr_list,
              Numeric&    refr_index,
        const Agenda&     refr_index_agenda,
        ConstVectorView   p_grid,
        ConstVectorView   lat_grid,
        ConstMatrixView   z_field,
        ConstMatrixView   t_field,
        ConstTensor3View  vmr_field,
        ConstVectorView   refellipsoid,
        ConstVectorView   z_surface,
        const String&     rtrace_method,
        const Numeric&    lraytrace,
        const Numeric&    lmax );

void ppath_step_refr_3d(
              Workspace&  ws,
              Ppath&      ppath,
              Numeric&    rte_pressure,
              Numeric&    rte_temperature,
              Vector&     rte_vmr_list,
              Numeric&    refr_index,
        const Agenda&     refr_index_agenda,
        ConstVectorView   p_grid,
        ConstVectorView   lat_grid,
        ConstVectorView   lon_grid,
        ConstTensor3View  z_field,
        ConstTensor3View  t_field,
        ConstTensor4View  vmr_field,
        ConstVectorView   refellipsoid,
        ConstMatrixView   z_surface,
        const String&     rtrace_method,
        const Numeric&    lraytrace,
        const Numeric&    lmax );

void ppath_calc(
              Workspace&      ws,
              Ppath&          ppath,
        const Agenda&         ppath_step_agenda,
        const Index&          atmosphere_dim,
        const Vector&         p_grid,
        const Vector&         lat_grid,
        const Vector&         lon_grid,
        const Tensor3&        z_field,
        const Vector&         refellipsoid,
        const Matrix&         z_surface,
        const Index&          cloudbox_on, 
        const ArrayOfIndex&   cloudbox_limits,
        const Vector&         rte_pos,
        const Vector&         rte_los,
        const bool&           outside_cloudbox,
        const Verbosity&      verbosity);

#endif  // ppath_h
