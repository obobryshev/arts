/* Copyright (C) 2002 Patrick Eriksson <Patrick.Eriksson@rss.chalmers.se>
                      Stefan Buehler   <sbuehler@uni-bremen.de>

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
  === File description 
  ===========================================================================*/

/*!
   \file   physics_funcs.cc
   \author Patrick Eriksson <Patrick.Eriksson@rss.chalmers.se>
   \date   2002-05-08 

   This file contains the code of functions of physical character.
   Modified by Claudia Emde (2002-05-28).
*/



/*===========================================================================
  === External declarations
  ===========================================================================*/

#include <cmath>
#include <stdexcept>
#include "physics_funcs.h"
#include "messages.h"          
#include "mystring.h"
#include "physics_funcs.h"

extern const Numeric PLANCK_CONST;
extern const Numeric SPEED_OF_LIGHT;
extern const Numeric BOLTZMAN_CONST;



/*===========================================================================
  === The functions (in alphabetical order)
  ===========================================================================*/

//! invplanck
/*!
   Converts a radiance to Plack brightness temperature.

    \return     Planck brightness temperature
    \param  i   radiance
    \param  f   frequency

    \author Patrick Eriksson 
    \date   2002-08-11
*/
Numeric invplanck(
        const Numeric&  i,
        const Numeric&  f )
{
  assert( i >= 0 );
  assert( f > 0 );

  // Use always double to avoid numerical problem (see invrayjean)
  static const double a = PLANCK_CONST / BOLTZMAN_CONST;
  static const double b = 2 * PLANCK_CONST / ( SPEED_OF_LIGHT*SPEED_OF_LIGHT );

  return   a * f / log( b*pow(f,3)/i + 1 );
}



//! invrayjean
/*! 
   Converts a radiance to Rayleigh-Jean brightness temperature.

    \return     RJ brightness temperature
    \param  i   radiance
    \param  f   frequency

    \author Patrick Eriksson 
    \date   2000-09-28 
*/
Numeric invrayjean(
        const Numeric&  i,
        const Numeric&  f )
{
  assert( i >= 0 );
  assert( f > 0 );

  // Double must be used here (if not, the result can be NaN when using float)
  // Integrated this calculation into the return statement and changed
  // the order of the terms to avoid numerical instability.
  // (OLE 2002-08-26)
  //static const double   a = SPEED_OF_LIGHT*SPEED_OF_LIGHT/(2*BOLTZMAN_CONST);

  return   SPEED_OF_LIGHT / ((double)f * (double)f)
    * (double)i * SPEED_OF_LIGHT / (2 * BOLTZMAN_CONST);
}


//! number_density
/*! 
   Calculates the atmospheric number density.
   
   \return     number density
   \param  p   pressure
   \param  t   temperature
   
   \author Patrick Eriksson 
   \date   2000-04-08 
*/
Numeric number_density(  
        const Numeric&   p,
        const Numeric&   t )
{
  assert( p >= 0 );
  assert( t >= 0 );
  return   p / ( t * BOLTZMAN_CONST );
}



//! planck
/*! 
  Calculates the Planck function for a single temperature.
  
  \return     blackbody radiation
  \param  f   frequency
  \param  t   temperature
  
  \author Patrick Eriksson 
  \date   2000-04-08 
*/
Numeric planck( 
        const Numeric&   f, 
        const Numeric&   t )
{
  assert( f > 0 );
  assert( t >= 0 );

  // Double must be used here (if not, a becomes 0 when using float)
  static const double a = 2 * PLANCK_CONST / (SPEED_OF_LIGHT*SPEED_OF_LIGHT);
  static const double b = PLANCK_CONST / BOLTZMAN_CONST;
  
  return   a * f*f*f / ( exp( b*f/t ) - 1 );
}




