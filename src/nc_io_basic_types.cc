/* Copyright (C) 2003-2008 Oliver Lemke <olemke@core-dump.info>

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


////////////////////////////////////////////////////////////////////////////
//   File description
////////////////////////////////////////////////////////////////////////////
/*!
  \file   nc_io_basic_types.cc
  \author Oliver Lemke <olemke@core-dump.info>
  \date   2008-09-26

  \brief This file contains functions to handle NetCDF data files.

*/

#include "arts.h"
#include "nc_io.h"
#include "nc_io_types.h"


//=== Matrix ==========================================================

//! Reads a Matrix from a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param m       Matrix
*/
void
nc_read_from_file (const int ncid,
                   Matrix& m)
{
  Index nrows, ncols;
  nrows  = nc_get_dim (ncid, "nrows");
  ncols  = nc_get_dim (ncid, "ncols");

  m.resize(nrows, ncols);
  nc_get_data_double (ncid, "Matrix", m.get_c_array());
}


//! Writes a Matrix to a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param m       Matrix
*/
void
nc_write_to_file (const int ncid,
                  const Matrix& m)
{
  int retval;
  int ncdims[2], varid;
  if ((retval = nc_def_dim (ncid, "nrows", m.nrows(), &ncdims[0])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_dim (ncid, "ncols", m.ncols(), &ncdims[1])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_var (ncid, "Matrix", NC_DOUBLE, 2, &ncdims[0], &varid)))
    ncerror (retval, "nc_def_var");
  if ((retval = nc_enddef (ncid))) ncerror (retval, "nc_enddef");
  if ((retval = nc_put_var_double (ncid, varid, m.get_c_array())))
    ncerror (retval, "nc_put_var");
}

//=== Tensor3 ==========================================================

//! Reads a Tensor3 from a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param t       Tensor3
*/
void
nc_read_from_file (const int ncid,
                   Tensor3& t)
{
  Index npages, nrows, ncols;
  npages = nc_get_dim (ncid, "npages");
  nrows  = nc_get_dim (ncid, "nrows");
  ncols  = nc_get_dim (ncid, "ncols");

  t.resize(npages, nrows, ncols);
  nc_get_data_double (ncid, "Tensor3", t.get_c_array());
}


//! Writes a Tensor3 to a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param t       Tensor3
*/
void
nc_write_to_file (const int ncid,
                  const Tensor3& t)
{
  int retval;
  int ncdims[3], varid;
  if ((retval = nc_def_dim (ncid, "npages", t.npages(), &ncdims[0])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_dim (ncid, "nrows", t.nrows(), &ncdims[1])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_dim (ncid, "ncols", t.ncols(), &ncdims[2])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_var (ncid, "Tensor3", NC_DOUBLE, 3, &ncdims[0], &varid)))
    ncerror (retval, "nc_def_var");
  if ((retval = nc_enddef (ncid))) ncerror (retval, "nc_enddef");
  if ((retval = nc_put_var_double (ncid, varid, t.get_c_array())))
    ncerror (retval, "nc_put_var");
}

//=== Tensor4 ==========================================================

//! Reads a Tensor4 from a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param t       Tensor4
*/
void
nc_read_from_file (const int ncid,
                   Tensor4& t)
{
  Index nbooks, npages, nrows, ncols;
  nbooks = nc_get_dim (ncid, "nbooks");
  npages = nc_get_dim (ncid, "npages");
  nrows  = nc_get_dim (ncid, "nrows");
  ncols  = nc_get_dim (ncid, "ncols");

  t.resize(nbooks, npages, nrows, ncols);
  nc_get_data_double (ncid, "Tensor4", t.get_c_array());
}


//! Writes a Tensor4 to a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param t       Tensor4
*/
void
nc_write_to_file (const int ncid,
                  const Tensor4& t)
{
  int retval;
  int ncdims[4], varid;
  if ((retval = nc_def_dim (ncid, "nbooks", t.nbooks(), &ncdims[0])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_dim (ncid, "npages", t.npages(), &ncdims[0])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_dim (ncid, "nrows", t.nrows(), &ncdims[1])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_dim (ncid, "ncols", t.ncols(), &ncdims[2])))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_var (ncid, "Tensor4", NC_DOUBLE, 4, &ncdims[0], &varid)))
    ncerror (retval, "nc_def_var");
  if ((retval = nc_enddef (ncid))) ncerror (retval, "nc_enddef");
  if ((retval = nc_put_var_double (ncid, varid, t.get_c_array())))
    ncerror (retval, "nc_put_var");
}

//=== Vector ==========================================================

//! Reads a Vector from a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param v       Vector
*/
void
nc_read_from_file (const int ncid,
                   Vector& v)
{
  Index nelem;
  nelem = nc_get_dim (ncid, "nelem");

  v.resize(nelem);
  nc_get_data_double (ncid, "Vector", v.get_c_array());
}


//! Writes a Vector to a NetCDF file
/*!
  \param ncf     NetCDF file discriptor
  \param v       Vector
*/
void
nc_write_to_file (const int ncid,
                  const Vector& v)
{
  int retval;
  int ncdim, varid;
  if ((retval = nc_def_dim (ncid, "nelem", v.nelem(), &ncdim)))
    ncerror (retval, "nc_def_dim");
  if ((retval = nc_def_var (ncid, "Vector", NC_DOUBLE, 1, &ncdim, &varid)))
    ncerror (retval, "nc_def_var");
  if ((retval = nc_enddef (ncid))) ncerror (retval, "nc_enddef");
  if ((retval = nc_put_var_double (ncid, varid, v.get_c_array())))
    ncerror (retval, "nc_put_var");
}

////////////////////////////////////////////////////////////////////////////
//   Dummy funtion for groups for which
//   IO function have not yet been implemented
////////////////////////////////////////////////////////////////////////////

#define TMPL_NC_READ_WRITE_FILE_DUMMY(what) \
  void nc_write_to_file (const int, const what&) \
  { \
    throw runtime_error ("NetCDF support not yet implemented for this type!"); \
  } \
  void nc_read_from_file (const int, what&) \
  { \
    throw runtime_error ("NetCDF support not yet implemented for this type!"); \
  }

//=== Basic Types ==========================================================

TMPL_NC_READ_WRITE_FILE_DUMMY( Index )
TMPL_NC_READ_WRITE_FILE_DUMMY( Numeric )
TMPL_NC_READ_WRITE_FILE_DUMMY( Sparse )
TMPL_NC_READ_WRITE_FILE_DUMMY( String )
TMPL_NC_READ_WRITE_FILE_DUMMY( Tensor5 )
TMPL_NC_READ_WRITE_FILE_DUMMY( Tensor6 )
TMPL_NC_READ_WRITE_FILE_DUMMY( Tensor7 )
TMPL_NC_READ_WRITE_FILE_DUMMY( Timer )

//==========================================================================

// Undefine the macro to avoid it being used anywhere else
#undef TMPL_NC_READ_WRITE_FILE_DUMMY


