/* Copyright (C) 2018 Oliver Lemke <oliver.lemke@uni-hamburg.de>

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

/*!
  \file   hitran_xsec.h
  \author Oliver Lemke <oliver.lemke@uni-hamburg.de>
  \date   2018-01-08

  \brief  Methods and classes for HITRAN absorption cross section data.
*/

#ifndef HITRAN_XSEC_H
#define HITRAN_XSEC_H

#include "array.h"
#include "arts.h"
#include "bifstream.h"
#include "gridded_fields.h"
#include "matpackI.h"
#include "messages.h"
#include "mystring.h"
#include "species.h"

/** Hitran crosssection class.
 *
 * Stores the coefficients from our model for hitran crosssection data and
 * applies them to calculate the crossections.
 */
class XsecRecord {
 public:
  /** Return species index */
  [[nodiscard]] const Species::Species& Species() const { return mspecies; };

  /** Return species name */
  [[nodiscard]] String SpeciesName() const;

  /** Set species name */
  void SetSpecies(const Species::Species species) { mspecies = species; };

  /** Return species index */
  [[nodiscard]] Index Version() const { return mversion; };

  /** Set species name */
  void SetVersion(Index version);

  /** Calculate hitran cross section data.

     Calculate crosssections at each frequency for given pressure and
     temperature.

     The extrapolation for temperatures and pressures values that lie outside
     the fit data of the model can optionally be turned off. It is
     recommended to leave these on.

     \param[out] result     Crosssections for given frequency grid.
     \param[in] f_grid      Frequency grid.
     \param[in] pressure    Scalar pressure.
     \param[in] temperature Scalar temperature.
     \param[in] extrapolate_pressure     Extrapolate data outside model fit.
     \param[in] extrapolate_temperature  Extrapolate data outside model fit.
     \param[in] verbosity   Verbosity.
     */
  void Extract(VectorView result,
               const Vector& f_grid,
               Numeric pressure,
               Numeric temperature,
               Index extrapolate_pressure,
               Index extrapolate_temperature,
               const Verbosity& verbosity) const;

  /************ VERSION 2 *************/
  /** Get mininum pressures from fit */
  [[nodiscard]] const Vector& FitMinPressures() const {
    return mfitminpressures;
  };

  /** Get maximum pressures from fit */
  [[nodiscard]] const Vector& FitMaxPressures() const {
    return mfitmaxpressures;
  };

  /** Get mininum temperatures from fit */
  [[nodiscard]] const Vector& FitMinTemperatures() const {
    return mfitmintemperatures;
  };

  /** Get maximum temperatures */
  [[nodiscard]] const Vector& FitMaxTemperatures() const {
    return mfitmaxtemperatures;
  };

  /** Get coefficients */
  [[nodiscard]] const ArrayOfGriddedField2& FitCoeffs() const {
    return mfitcoeffs;
  };

  /** Get mininum pressures from fit */
  [[nodiscard]] Vector& FitMinPressures() { return mfitminpressures; };

  /** Get maximum pressures from fit */
  [[nodiscard]] Vector& FitMaxPressures() { return mfitmaxpressures; };

  /** Get mininum temperatures from fit */
  [[nodiscard]] Vector& FitMinTemperatures() { return mfitmintemperatures; };

  /** Get maximum temperatures */
  [[nodiscard]] Vector& FitMaxTemperatures() { return mfitmaxtemperatures; };

  /** Get coefficients */
  [[nodiscard]] ArrayOfGriddedField2& FitCoeffs() { return mfitcoeffs; };

 private:
  /** Calculate crosssections */
  void CalcXsec(VectorView& xsec,
                Index dataset,
                Range range,
                Numeric pressure,
                Numeric temperature) const;

  /** Calculate temperature derivative of crosssections */
  void CalcDT(VectorView& xsec_dt,
              Index dataset,
              Range range,
              Numeric pressure,
              Numeric temperature) const;

  /** Calculate pressure derivative of crosssections */
  void CalcDP(VectorView& xsec_dp,
              Index dataset,
              Range range,
              Numeric pressure,
              Numeric temperature) const;

  static const Index P00 = 0;
  static const Index P10 = 1;
  static const Index P01 = 2;
  static const Index P20 = 3;
  static const Index P11 = 4;
  static const Index P02 = 5;

  Index mversion{2};
  Species::Species mspecies;
  /* VERSION 2 */
  Vector mfitminpressures;
  Vector mfitmaxpressures;
  Vector mfitmintemperatures;
  Vector mfitmaxtemperatures;
  ArrayOfGriddedField2 mfitcoeffs;
};

using ArrayOfXsecRecord = Array<XsecRecord>;

Index hitran_xsec_get_index(const ArrayOfXsecRecord& xsec_data,
                            Species::Species species);

std::ostream& operator<<(std::ostream& os, const XsecRecord& xd);

#endif  // HITRAN_XSEC_H
