/* Copyright (C) 2003-2012 Oliver Lemke <olemke@core-dump.info>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA. */

#include <iostream>
#include "matpack_data.h"
#include "xml_io_base.h"

int main(int /*argc*/, char * /*argv*/[]) {

  Vector v1{1, 2, 3, 4, 5};
  String filename{"test.xml"};
  xml_write_to_file_base(filename, v1, FILE_TYPE_ASCII, Verbosity());
  std::cout << v1 << '\n';

  Vector v2;
  xml_read_from_file_base(filename, v2, Verbosity());
  std::cout << v2 << '\n';

  return (0);
}
