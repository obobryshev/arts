/* Copyright (C) 2000 Stefan Buehler <sbuehler@uni-bremen.de>

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

#include "arts.h"
#include "token.h"
#include "vecmat.h"
#include "file.h"
#include "auto_wsv.h"
#include "methods.h"


/* Adds commas and indentation to parameter lists. */
void align(ofstream& ofs, bool& is_first_parameter, const string& indent)
{
  // Add comma and line break, if not first element:
  if (is_first_parameter)
    is_first_parameter = false;
  else
    {
      ofs << ",\n";
      // Make proper indentation:
      ofs << indent;
    }
}

int main()
{
  try
    {
      // Make the global data visible:
      extern ARRAY<MdRecord> md_data;
      extern const ARRAY<string> wsv_group_names;
      extern const ARRAY<WsvRecord> wsv_data;

      // Initialize method data.
      define_md_data();

      // Initialize the wsv group name array:
      define_wsv_group_names();

      // Initialize wsv data.
      define_wsv_data();
  

      const size_t n_md  = md_data.size();
      const size_t n_wsv = wsv_data.size();

      // For safety, check if n_wsv and N_WSV have the same value. If not, 
      // then the file wsv.h is not up to date.
      if (N_WSV != n_wsv)
	{
	  cout << "The file wsv.h is not up to date!\n";
	  cout << "(N_WSV = " << N_WSV << ", n_wsv = " << n_wsv << ")\n";
	  cout << "Make wsv.h first. Check if Makefile is correct.\n";
	  return 1;
	}

      // Write auto_md.cc:
      // -----------
      ofstream ofs;
      open_output_file(ofs,"auto_md.cc");
  
      ofs << "// This file was generated automatically by make_md_cc.cc.\n";
      ofs << "// DO NOT EDIT !\n";
      ofs << "// Generated: "
	  << __DATE__ << ", "
	  << __TIME__ << "\n\n";

      ofs << "#include \"arts.h\"\n"
	  << "#include \"make_array.h\"\n"
	  << "#include \"auto_md.h\"\n"
	  << "\n";

      // Declare wsv_data:
      ofs << "// The workspace variable pointers:\n"
	  << "extern const ARRAY<WsvP*> wsv_pointers;\n\n"

	  << "// Other wsv data:\n"
	  << "extern const ARRAY<WsvRecord> wsv_data;\n\n";


      // Write all get-away functions:
      // -----------------------------
      for (size_t i=0; i<n_md; ++i)
	{
	  // This is needed to flag the first function parameter, which 
	  // needs no line break before being written:
	  bool is_first_parameter = true;
	  // The string indent is needed to achieve the correct
	  // indentation of the functin parameters:
	  string indent = string(md_data[i].Name().size()+3,' ');;
	  
	  // There are four lists of parameters that we have to
	  // write. 
	  ARRAY<size_t>  vo=md_data[i].Output();   // Output 
	  ARRAY<size_t>  vi=md_data[i].Input();    // Input
	  ARRAY<size_t>  vgo=md_data[i].GOutput();   // Generic Output 
	  ARRAY<size_t>  vgi=md_data[i].GInput();    // Generic Input
	  // vo and vi contain handles of workspace variables, 
	  // vgo and vgi handles of workspace variable groups.

	  // Check, if some workspace variables are in both the
	  // input and the output list, and erase those from the input 
	  // list:
	  for (ARRAY<size_t>::const_iterator j=vo.begin(); j<vo.end(); ++j)
	    for (ARRAY<size_t>::iterator k=vi.begin(); k<vi.end(); ++k)
	      if ( *j == *k )
		{
		  //		  erase_vector_element(vi,k);
		  k = vi.erase(k) - 1;
		  // We need the -1 here, otherwise due to the
		  // following increment we would miss the element
		  // behind the erased one, which is now at the
		  // position of the erased one.
		}

	  // There used to be a similar block here for the generic
	  // input/output variables. However, this was a mistake. For
	  // example, if a method has a vector as generic input and a
	  // vector as generic output, this does not mean that it is
	  // the same vector!


	  ofs << "void " << md_data[i].Name()
	      << "_g(WorkSpace& ws, const MRecord& mr)\n";
	  ofs << "{\n";


	  // Define generic output pointers
	  for (size_t j=0; j<vgo.size(); ++j)
	    {
	      ofs << "  " << wsv_group_names[md_data[i].GOutput()[j]]
		  << " *GO" << j << " = *wsv_pointers[mr.Output()[" << j
		  << "]];\n";
	    }

	  // Define generic input pointers
	  for (size_t j=0; j<vgi.size(); ++j)
	    {
	      ofs << "  " << wsv_group_names[md_data[i].GInput()[j]]
		  << " *GI" << j << " = *wsv_pointers[mr.Input()[" << j
		  << "]];\n";
	    }

	  ofs << "  " << md_data[i].Name() << "(";

	  // Write the Output workspace variables:
	  for (size_t j=0; j<vo.size(); ++j)
	    {
	      // Add comma and line break, if not first element:
	      align(ofs,is_first_parameter,indent);

	      ofs << "ws." << wsv_data[vo[j]].Name();
	    }

	  // Write the Generic output workspace variables:
	  for (size_t j=0; j<vgo.size(); ++j)
	    {
	      // Add comma and line break, if not first element:
	      align(ofs,is_first_parameter,indent);

	      ofs << "*GO" << j;
	    }

	  // Write the Generic output workspace variable names:
	  for (size_t j=0; j<vgo.size(); ++j)
	    {
	      // Add comma and line break, if not first element:
	      align(ofs,is_first_parameter,indent);

	      ofs << "wsv_data[mr.Output()["
		  << j
		  << "]].Name()";
	    }

	  // Write the Input workspace variables:
	  for (size_t j=0; j<vi.size(); ++j)
	    {
	      // Add comma and line break, if not first element:
	      align(ofs,is_first_parameter,indent);

	      ofs << "ws." << wsv_data[vi[j]].Name();
	    }

	  // Write the Generic input workspace variables:
	  for (size_t j=0; j<vgi.size(); ++j)
	    {
	      // Add comma and line break, if not first element:
	      align(ofs,is_first_parameter,indent);

	      ofs << "*GI" << j;
	    }

	  // Write the Generic input workspace variable names:
	  for (size_t j=0; j<vgi.size(); ++j)
	    {
	      // Add comma and line break, if not first element:
	      align(ofs,is_first_parameter,indent);

	      ofs << "wsv_data[mr.Input()["
		  << j
		  << "]].Name()";
	    }

	  // Write the control parameters:
	  {
	    // The mr parameters look all the same (mr[i]), so we just
	    // need to know the number of them: 
	    size_t n_mr = md_data[i].Keywords().size();

	    for (size_t j=0; j!=n_mr; ++j)
	      {
		// Add comma and line break, if not first element:
		align(ofs,is_first_parameter,indent);

		ofs << "mr.Values()[" << j << "]";
	      }
	  }

	  ofs << ");\n";
	  ofs << "}\n\n";
	}

      // Add getaways, the array that hold pointers to the getaway functions:
      {
	string indent = "     ";
	bool is_first_parameter = true;

	ofs << "// The array holding the pointers to the getaway functions.\n"
	    << "void (*getaways[])(WorkSpace&, const MRecord&)\n"
	    << "  = {";
	for (size_t i=0; i<n_md; ++i)
	  {
	    // Add comma and line break, if not first element:
	    align(ofs,is_first_parameter,indent);

	    ofs << md_data[i].Name() << "_g";
	  }
	ofs << "};\n\n";
      }

    }
  catch (exception x)
    {
      cout << "Something went wrong. Message text:\n";
      cout << x.what() << '\n';
      return 1;
    }

  return 0;
}
