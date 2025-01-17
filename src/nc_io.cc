/* Copyright (C) 2012 Oliver Lemke <olemke@core-dump.info>

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
  \file   nc_io.cc
  \author Oliver Lemke <olemke@core-dump.info>
  \date   2008-09-12

  \brief This file contains basic functions to handle NetCDF data files.

*/

#include "arts.h"

#ifdef ENABLE_NETCDF

#include "exceptions.h"
#include "file.h"
#include "messages.h"
#include "nc_io.h"
#include "nc_io_types.h"

////////////////////////////////////////////////////////////////////////////
//   Default file name
////////////////////////////////////////////////////////////////////////////

//! Gives the default filename for the NetCDF formats.
/*!
 The default name is only used if the filename is empty.

 \param filename filename
 \param varname variable name

 \author Oliver Lemke
*/
void nca_filename(String &filename, const String &varname) {
  if ("" == filename) {
    extern const String out_basename;
    filename = out_basename + "." + varname + ".nc";
  }
}

//! Gives the default filename, with file index, for the NetCDF formats.
/*!
 The default name is only used if the filename is empty.

 \param[out] filename   filename
 \param[in]  file_index Index appended to the filename
 \param[in]  varname    variable name

 \author Oliver Lemke
*/
void nca_filename_with_index(String &filename, const Index &file_index,
                             const String &varname) {
  if ("" == filename) {
    extern const String out_basename;
    ostringstream os;
    os << out_basename << "." << varname << "." << file_index << ".nc";
    filename = os.str();
  } else {
    ostringstream os;
    os << filename << "." << file_index << ".nc";
    filename = os.str();
  }
}

//! Reads a variable from a NetCDF file
/*!
 \param[in]  filename    NetCDF filename
 \param[out] type        Input variable
 \param[in]  verbosity   Verbosity

 \author Oliver Lemke
*/
template <typename T>
void nca_read_from_file(const String &filename, T &type,
                        const Verbosity &verbosity) {
  CREATE_OUT2;

  String efilename = expand_path(filename);

  out2 << "  Reading " << efilename << '\n';

  bool fail = false;
  String fail_msg;
#pragma omp critical(netcdf__critical_region)
  {
    int ncid;
    if (nc_open(efilename.c_str(), NC_NOWRITE, &ncid)) {
      fail = true;
      fail_msg = "Error opening file. Does it exists?";
    } else {
      try {
        nca_read_from_file(ncid, type, verbosity);
      } catch (const std::runtime_error &e) {
        fail = true;
        fail_msg = e.what();
      }
      nc_close(ncid);
    }
  }

  if (fail)
    ARTS_USER_ERROR("Error reading file: ", efilename, '\n', fail_msg);
}

//! Writes a variable to a NetCDF file
/*!
 \param[in]  filename    NetCDF filename
 \param[in]  type        Output variable
 \param[in]  verbosity   Verbosity

 \author Oliver Lemke
*/
template <typename T>
void nca_write_to_file(const String &filename, const T &type,
                       const Verbosity &verbosity) {
  CREATE_OUT2;

  String efilename = add_basedir(filename);

  out2 << "  Writing " << efilename << '\n';

  bool fail = false;
  String fail_msg;
#pragma omp critical(netcdf__critical_region)
  {
    int ncid;
    if (nc_create(efilename.c_str(), NC_CLOBBER | NC_NETCDF4, &ncid)) {
      fail = true;
      fail_msg = "Error opening file for writing.";
    } else {
      try {
        nca_write_to_file(ncid, type, verbosity);
      } catch (const std::runtime_error &e) {
        fail = true;
        fail_msg = e.what();
      }
      nc_close(ncid);
    }
  }

  if (fail)
    ARTS_USER_ERROR("Error writing file: ", efilename, '\n', fail_msg);
}

//! Define NetCDF dimension.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Dimension name
 \param[in]  nelem  Dimension size
 \param[out] ncdim  NetCDF dimension handle

 \author Oliver Lemke
 */
void nca_def_dim(const int ncid, const String &name, const Index nelem,
                 int *ncdim) {
  int retval;
  if ((retval = nc_def_dim(ncid, name.c_str(), nelem, ncdim)))
    nca_error(retval, "nc_def_dim");
}

//! Define NetCDF variable.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[in]  type   NetCDF type
 \param[in]  ndims  Number of dimensions
 \param[in]  dims   Pointer to dimensions
 \param[out] varid  NetCDF variable handle

 \author Oliver Lemke
 */
void nca_def_var(const int ncid, const String &name, const nc_type type,
                 const int ndims, const int *dims, int *varid) {
  int retval;
  if ((retval = nc_def_var(ncid, name.c_str(), type, ndims, dims, varid)))
    nca_error(retval, "nc_def_var");
}

//! Define NetCDF dimensions and variable for an ArrayOfIndex.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[in]  a      ArrayOfIndex
 \returns Variable handle

 \author Oliver Lemke
 */
int nca_def_ArrayOfIndex(const int ncid, const String &name,
                         const ArrayOfIndex &a) {
  std::array<int, 1> ncdims;
  int varid;
  if (a.nelem()) {
    nca_def_dim(ncid, name + "_nelem", a.nelem(), &ncdims[0]);
    nca_def_var(ncid, name, NC_INT, 1, &ncdims[0], &varid);
  } else
    varid = -1;

  return varid;
}

//! Define NetCDF dimensions and variable for a Vector.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[in]  v      Vector
 \returns Variable handle

 \author Oliver Lemke
 */
int nca_def_Vector(const int ncid, const String &name, const Vector &v) {
  std::array<int, 1> ncdims;
  int varid;
  if (v.nelem()) {
    nca_def_dim(ncid, name + "_nelem", v.nelem(), &ncdims[0]);
    nca_def_var(ncid, name, NC_DOUBLE, 1, &ncdims[0], &varid);
  } else
    varid = -1;

  return varid;
}

//! Define NetCDF dimensions and variable for a Matrix.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[in]  m      Matrix
 \returns Variable handle

 \author Oliver Lemke
 */
int nca_def_Matrix(const int ncid, const String &name, const Matrix &m) {
  std::array<int, 2> ncdims;
  int varid;
  if (m.nrows() && m.ncols()) {
    nca_def_dim(ncid, name + "_nrows", m.nrows(), &ncdims[0]);
    nca_def_dim(ncid, name + "_ncols", m.ncols(), &ncdims[1]);
    nca_def_var(ncid, name, NC_DOUBLE, 2, &ncdims[0], &varid);
  } else
    varid = -1;

  return varid;
}

//! Define NetCDF dimensions and variable for a Tensor4.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[in]  t      Tensor4
 \returns Variable handle

 \author Oliver Lemke
 */
int nca_def_Tensor4(const int ncid, const String &name, const Tensor4 &t) {
  std::array<int, 4> ncdims;
  int varid;
  if (t.nbooks() && t.npages() && t.nrows() && t.ncols()) {
    nca_def_dim(ncid, name + "_nbooks", t.nbooks(), &ncdims[0]);
    nca_def_dim(ncid, name + "_npages", t.npages(), &ncdims[1]);
    nca_def_dim(ncid, name + "_nrows", t.nrows(), &ncdims[2]);
    nca_def_dim(ncid, name + "_ncols", t.ncols(), &ncdims[3]);
    nca_def_var(ncid, name, NC_DOUBLE, 4, &ncdims[0], &varid);
  } else
    varid = -1;

  return varid;
}

//! Read a dimension from NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Dimension name in NetCDF file
 \param[in]  noerror  Return 0 instead of throwing an exception if dimension
 does not exist in file \returns Dimension size

 \author Oliver Lemke
 */
Index nca_get_dim(const int ncid, const String &name, const bool noerror) {
  int retval, dimid;
  size_t ndim;
  if ((retval = nc_inq_dimid(ncid, name.c_str(), &dimid))) {
    if (!noerror)
      nca_error(retval, "nc_inq_ndims(" + name + ")");
    else
      return 0;
  }
  if ((retval = nc_inq_dimlen(ncid, dimid, &ndim))) {
    if (!noerror)
      nca_error(retval, "nc_inq_dimlen(" + name + ")");
    else
      return 0;
  }

  return (Index)ndim;
}

//! Read variable of type int from NetCDF file.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[out] data   Data read from file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, int *data) {
  int retval, varid;
  if ((retval = nc_inq_varid(ncid, name.c_str(), &varid)))
    nca_error(retval, "nc_inq_varid(" + name + ")");
  if ((retval = nc_get_var_int(ncid, varid, data)))
    nca_error(retval, "nc_get_var(" + name + ")");
}

//! Read variable of type long from NetCDF file.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[out] data   Data read from file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, long *data) {
  int retval, varid;
  if ((retval = nc_inq_varid(ncid, name.c_str(), &varid)))
    nca_error(retval, "nc_inq_varid(" + name + ")");
  if ((retval = nc_get_var_long(ncid, varid, data)))
    nca_error(retval, "nc_get_var(" + name + ")");
}

//! Read variable of type long long from NetCDF file.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[out] data   Data read from file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, long long *data) {
  int retval, varid;
  if ((retval = nc_inq_varid(ncid, name.c_str(), &varid)))
    nca_error(retval, "nc_inq_varid(" + name + ")");
  if ((retval = nc_get_var_longlong(ncid, varid, data)))
    nca_error(retval, "nc_get_var(" + name + ")");
}

//! Read variable of type double from NetCDF file.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[out] data   Data read from file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, Numeric *data) {
  int retval, varid;
  if ((retval = nc_inq_varid(ncid, name.c_str(), &varid)))
    nca_error(retval, "nc_inq_varid(" + name + ")");
  if ((retval = nc_get_var_double(ncid, varid, data)))
    nca_error(retval, "nc_get_var(" + name + ")");
}

//! Read variable of type array of double from NetCDF file.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[out] data   Data read from file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, size_t start,
                  size_t count, Numeric *data) {
  int retval, varid;
  if ((retval = nc_inq_varid(ncid, name.c_str(), &varid)))
    nca_error(retval, "nc_inq_varid(" + name + ")");
  if ((retval = nc_get_vara_double(ncid, varid, &start, &count, data)))
    nca_error(retval, "nc_get_var(" + name + ")");
}

//! Read variable of type array of char from NetCDF file.
/**
 \param[in]  ncid   NetCDF file descriptor
 \param[in]  name   Variable name in NetCDF file
 \param[out] data   Data read from file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, char *data) {
  int retval, varid;
  if ((retval = nc_inq_varid(ncid, name.c_str(), &varid)))
    nca_error(retval, "nc_inq_varid(" + name + ")");
  if ((retval = nc_get_var_text(ncid, varid, data)))
    nca_error(retval, "nc_get_var(" + name + ")");
}

//! Read variable of type ArrayOfIndex from NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[out] aoi      Data read from file
 \param[in]  noerror  Return empty variable instead of throwing an exception if
 variable does not exist in file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, ArrayOfIndex &aoi,
                  const bool noerror) {
  Index nelem = nca_get_dim(ncid, name + "_nelem", noerror);
  aoi.resize(nelem);
  if (nelem) {
    nca_get_data(ncid, name, aoi.data());
  }
}

//! Read variable of type ArrayOfArrayOfSpeciesTag from NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[out] aast     Data read from file
 \param[in]  noerror  Return empty variable instead of throwing an exception if
 variable does not exist in file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name,
                  ArrayOfArrayOfSpeciesTag &aast, const bool noerror) {
  ArrayOfIndex species_count;
  nca_get_data(ncid, name + "_count", species_count, noerror);
  aast.resize(species_count.nelem());
  if (species_count.nelem()) {
    Index species_strings_nelem =
        nca_get_dim(ncid, name + "_strings_nelem", noerror);
    Index species_strings_length =
        nca_get_dim(ncid, name + "_strings_length", noerror);
    char *species_strings =
        new char[species_strings_nelem * species_strings_length];  // FIXME: THIS SHOULD NOT USE "new"
    if (species_count.nelem())
      nca_get_data(ncid, name + "_strings", species_strings);

    Index si = 0;
    for (Index i = 0; i < species_count.nelem(); i++) {
      aast[i].resize(0);
      for (Index j = 0; j < species_count[i]; j++) {
        aast[i].push_back(SpeciesTag(&species_strings[si]));
        si += species_strings_length;
      }
    }

    delete[] species_strings;
  }
}

//! Read variable of type Vector from NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[out] v        Data read from file
 \param[in]  noerror  Return empty variable instead of throwing an exception if
 variable does not exist in file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, Vector &v,
                  const bool noerror) {
  Index nelem = nca_get_dim(ncid, name + "_nelem", noerror);
  v.resize(nelem);
  if (nelem)
    nca_get_data(ncid, name, v.unsafe_data_handle());
}

//! Read variable of type Matrix from NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[out] m        Data read from file
 \param[in]  noerror  Return empty variable instead of throwing an exception if
 variable does not exist in file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, Matrix &m,
                  const bool noerror) {
  Index nrows = nca_get_dim(ncid, name + "_nrows", noerror);
  Index ncols = nca_get_dim(ncid, name + "_ncols", noerror);
  m.resize(nrows, ncols);
  if (nrows && ncols)
    nca_get_data(ncid, name, m.unsafe_data_handle());
}

//! Read variable of type Tensor4 from NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[out] t        Data read from file
 \param[in]  noerror  Return empty variable instead of throwing an exception if
 variable does not exist in file

 \author Oliver Lemke
 */
void nca_get_data(const int ncid, const String &name, Tensor4 &t,
                  const bool noerror) {
  Index nbooks = nca_get_dim(ncid, name + "_nbooks", noerror);
  Index npages = nca_get_dim(ncid, name + "_npages", noerror);
  Index nrows = nca_get_dim(ncid, name + "_nrows", noerror);
  Index ncols = nca_get_dim(ncid, name + "_ncols", noerror);
  t.resize(nbooks, npages, nrows, ncols);
  if (nbooks && npages && nrows && ncols)
    nca_get_data(ncid, name, t.unsafe_data_handle());
}

//! Write variable of type long* to NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[in]  a        Data to be written
 \returns True if variable was not empty

 \author Oliver Lemke
 */
void nca_put_var(const int ncid, const int varid, const long *ind_arr) {
  int retval;
  if ((retval = nc_put_var_long(ncid, varid, ind_arr)))
    nca_error(retval, "nc_put_var");
}

//! Write variable of type long long* to NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[in]  a        Data to be written
 \returns True if variable was not empty

 \author Oliver Lemke
 */
void nca_put_var(const int ncid, const int varid, const long long *ind_arr) {
  int retval;
  if ((retval = nc_put_var_longlong(ncid, varid, ind_arr)))
    nca_error(retval, "nc_put_var");
}

//! Write variable of type ArrayOfIndex to NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[in]  a        Data to be written
 \returns True if variable was not empty

 \author Oliver Lemke
 */
bool nca_put_var(const int ncid, const int varid, const ArrayOfIndex &a) {
  bool fail = true;
  if (a.nelem()) {
    nca_put_var(ncid, varid, a.data());
    fail = false;
  }
  return fail;
}

//! Write variable of type Vector to NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[in]  v        Data to be written
 \returns True if variable was not empty

 \author Oliver Lemke
 */
bool nca_put_var(const int ncid, const int varid, const Vector &v) {
  bool fail = true;
  if (v.nelem()) {
    int retval;
    if ((retval = nc_put_var_double(ncid, varid, v.unsafe_data_handle())))
      nca_error(retval, "nc_put_var");
    fail = false;
  }
  return fail;
}

//! Write variable of type Matrix to NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[in]  m        Data to be written
 \returns True if variable was not empty

 \author Oliver Lemke
 */
bool nca_put_var(const int ncid, const int varid, const Matrix &m) {
  bool fail = true;
  if (m.nrows() && m.ncols()) {
    int retval;
    if ((retval = nc_put_var_double(ncid, varid, m.unsafe_data_handle())))
      nca_error(retval, "nc_put_var");
    fail = false;
  }
  return fail;
}

//! Write variable of type Tensor4 to NetCDF file.
/**
 \param[in]  ncid     NetCDF file descriptor
 \param[in]  name     Variable name in NetCDF file
 \param[in]  t        Data to be written
 \returns True if variable was not empty

 \author Oliver Lemke
 */
bool nca_put_var(const int ncid, const int varid, const Tensor4 &t) {
  bool fail = true;
  if (t.nbooks() && t.npages() && t.nrows() && t.ncols()) {
    int retval;
    if ((retval = nc_put_var_double(ncid, varid, t.unsafe_data_handle())))
      nca_error(retval, "nc_put_var");
    fail = false;
  }
  return fail;
}

//! Throws a runtime error for the given NetCDF error code
/**
 \param[in]  e  NetCDF error code
 \param[in]  s  Error message string

 \author Oliver Lemke
 */

void nca_error(const int e, const std::string_view s) {
  ARTS_USER_ERROR("NetCDF error: ", s, ", ", e, "\nCheck your input file.");
}

// We can't do the instantiation at the beginning of this file, because the
// implementation of nca_write_to_file and nca_read_from_file have to be known.

#include "nc_io_instantiation.h"

#endif /* ENABLE_NETCDF */
