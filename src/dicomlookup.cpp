/*
 * The GNU Octave dicom package is Copyright Andy Buckle 2010
 * Contact: blondandy using the sf.net system, 
 * <https://sourceforge.net/sendmessage.php?touser=1760416>
 * 
 * The GNU Octave dicom package is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * 
 * The GNU Octave dicom packag is distributed in the hope that it 
 * will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 * 
 * Please see the file, "COPYING" for further details of GNU General 
 * Public License version 3.
 * 
 */

#include "octave/oct.h"

#include "gdcmDict.h"
#include "gdcmVR.h"
#include "gdcmVM.h"

#include "dicomdict.h"

#include <typeinfo>

#define OCT_FN_NAME_LU dicomlookup
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

// build_against_gdcm dicomlookup.cpp dicomdict.cpp -o dicomlookup.oct

DEFUN_DLD (OCT_FN_NAME_LU, args, nargout,"\
keyword = dicomlookup(group, elem)  \n\
[group elem] = dicomlookup(keyword) \n\
") {
	octave_value_list retval;  // create object to store return values
	if (args.length()==1) { // keyword to tag
		charMatrix arg0mat = args(0).char_matrix_value ();
		if (arg0mat.rows()!=1) {
			error(QUOTED(OCT_FN_NAME_LU)": first arg should be a single row of chars: a string containing a DICOM keyword");
			return retval;
		}
		std::string keyword = arg0mat.row_as_string (0);
		gdcm::Tag tag;
		lookup_dicom_tag(tag, keyword);
		octave_uint16 group=tag.GetGroup();
		octave_uint16 elem=tag.GetElement();
		retval(0)=octave_value(group);
		retval(1)=octave_value(elem);
		return retval;
	}
	if (args.length()==2) { // tag to keyword 
		uint16_t tagvals[2];
		for( int i=0 ; i<2 ; i++) {
			if (args(i).is_string()) {
				std::istringstream iss(args(i).char_matrix_value().row_as_string(0));
				iss >> std::setbase(16) >> tagvals[i];
			} else {
				tagvals[i] = args(i).int_vector_value().fortran_vec()[0] ;
			}
		}
		gdcm::Tag tag(tagvals[0], tagvals[1]);
		std::string keyword;
		lookup_dicom_keyword(keyword, tag);
		return octave_value(keyword);
	}
	error(QUOTED(OCT_FN_NAME_LU)": takes 1 or 2 arguments, got %i. see help",args.length ());
	return retval;
}

/*

%!test
%! addpath('../inst'); % so it can find the dictionary
%! assert(dicomlookup(0x10,0x10),"PatientName");
%! assert(dicomlookup("10","10"),"PatientName");
%! assert(dicomlookup("0x10","0x10"),"PatientName");
%! assert(dicomlookup(16,16),"PatientName");

%!test
%! [g e]=dicomlookup("PatientName");
%! assert([g e], uint16([0x10 0x10]));
%! 
*/
