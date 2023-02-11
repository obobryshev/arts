/* Copyright (C) 2022 Oliver Lemke <oliver.lemke@uni-hamburg.de>

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
  \file   wsv_aux.cc
  \author Stefan Buehler <stefan.buehler@uni-hamburg.de>,
          Oliver Lemke <oliver.lemke@uni-hamburg.de>
  \date   Wed Jul  6 09:47:47 CEST 2022
  
  \brief  Implementation of WSV aux functions.
*/

#include "wsv_aux_operator.h"

#include "tokval_io.h"

#include <iostream>
#include <map>

#include "global_data.h"
#include "workspace_ng.h"

//! Output operator for WsvRecord.
/*! 
  This has to be here rather than with workspace.cc or
  workspace_aux.cc, because it uses agenda_data and AgendaMap.

  \param os  Output stream.
  \param wr  Workspace variable record.

  \return Output stream.
*/
std::ostream& operator<<(std::ostream& os, const WsvRecord& wr) {
  using global_data::wsv_groups;

  // We need a special treatment for the case that the WSV is an agenda.

  if (get_wsv_group_id("Agenda") != wr.Group() &&
      get_wsv_group_id("ArrayOfAgenda") != wr.Group()) {
    // No agenda.

    os << "\n*-------------------------------------------------------------------*\n"
       << "Workspace variable = " << wr.Name()
       << "\n---------------------------------------------------------------------\n"
       << "\n"
       << wr.Description() << "\n";
    if (wr.has_defaults()) {
      os << "Default: ";
      std::ostringstream ostr;
      ostr << TokValPrinter{wr.default_value()};
      if (ostr.str().empty()) ostr << "[]";
      if (ostr.str().find('\n') != std::string::npos) os << "\n";
      os << ostr.str() << "\n";
    }
    os << "\n---------------------------------------------------------------------\n"
       << "Group = " << wsv_groups[wr.Group()]
       << "\n*-------------------------------------------------------------------*\n";
  } else {
    // Agenda.

    using global_data::agenda_data;

    // AgendaMap is constant here and should never be changed
    using global_data::AgendaMap;

    auto j = AgendaMap.find(wr.Name());

    // Just for added safety, check that we really found something:
    ARTS_ASSERT(j != AgendaMap.end());

    cout << agenda_data[j->second] << "\n";
  }

  return os;
}
