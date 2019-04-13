/*
 * The GNU Octave dicom package is Copyright Andy Buckle 2010
 * Contact: blondandy using the sf.net system, 
 * <https://sourceforge.net/sendmessage.php?touser=1760416>
 * 
 * Changes Copyright Kris Thielemans 2011:
 * - support usage dicomread(struct-returned-by-dicominfo)
 * - return image in same order as matlab
 *
 * The GNU Octave dicom package is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * 
 * The GNU Octave dicom package is distributed in the hope that it 
 * will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 * 
 * Please see the file, "COPYING" for further details of GNU General 
 * Public License version 3.
 * 
 */

#include "octave/oct.h"
#include <octave/ov-struct.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gdcmImageReader.h"
using namespace gdcm;

#define DICOM_ERR -1
#define DICOM_OK 0

#define OCT_FN_NAME dicomread
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

DEFUN_DLD (dicomread, args, nargout,
		"-*- texinfo -*- \n\
 @deftypefn {Loadable Function} @var{image} = dicomread (@var{filename}) \n\
 @deftypefnx {Loadable Function} @var{image} = dicomread (@var{structure}) \n\
 \n\
 Load the image from a DICOM file. \n\
 @var{filename} is a string (giving the filename).\n\
 @var{structure} is a structure with a field @code{Filename} (such as returned by @code{dicominfo}).\n\
 @var{image} may be two or three dimensional, depending on the content of the file. \n\
 An integer or float matrix will be returned, the number of bits will depend on the file. \n\
\n\
 @seealso{dicominfo} \n\
 @end deftypefn \n\
 ")
{
	octave_value_list retval;  // create object to store return values
	if ( 0 == args.length()) {
		error(QUOTED(OCT_FN_NAME)": one arg required: dicom filename");
		return retval; 
	}

	std::string filename;
	// argument processing
	// check if 1st argument is a string or a struct with field Filename
	// If so, assign to filename variable, otherwise exit.
	if (args(0).is_string()) {
	  filename = args(0).string_value();
	}
	else {
	  octave_scalar_map arg0 = args(0).scalar_map_value ();
          if (error_state) {
	        error(QUOTED(OCT_FN_NAME)": arg should be a filename, 1 row of chars, or a struct returned by dicominfo");
		return retval; 
	  }
	  if (!arg0.contains("Filename")) {
	        error(QUOTED(OCT_FN_NAME)": if arg is a struct, it should have the Filename field");
		return retval; 
	  }
	  octave_value tmp = arg0.getfield ("Filename");
	  filename = tmp.string_value();
	}
		
	
#if 0 /* TODO support 'frames' stuff, see Matlab docs for dicomread */
	int i; // parse any additional args
	for (i=1; i<args.length(); i++) {
		charMatrix chex = args(i).char_matrix_value();
		if (chex.rows()!=1) {
			error(QUOTED(OCT_FN_NAME)": arg should be a string, 1 row of chars");
			return retval; 
		}
	}
#endif
	
	gdcm::ImageReader reader;
	reader.SetFileName( filename.c_str() );
	if( !reader.Read() ) {
		error(QUOTED(OCT_FN_NAME)": Could not read DICOM file with image: %s",filename.c_str());
		return retval;
	}
	
	const gdcm::Image &image = reader.GetImage();

	const octave_idx_type ndim = image.GetNumberOfDimensions();
	const unsigned int * const dims = image.GetDimensions();
	// dim 0: cols (width)
	// dim 1: rows (height)
	// dim 2: number of frames

	dim_vector dv;
	Array<octave_idx_type> perm_vect(dim_vector(ndim,1));
	perm_vect(0) = 1;
	perm_vect(1) = 0;

	// TODO check with non-square images if this needs to be dims[1],dims[0] etc
	if( 2==ndim ) {
		//this transposes first two dimensions
		dv = dim_vector(octave_idx_type(dims[0]), octave_idx_type(dims[1]));
	} else if (3==ndim) {
		// should be (rows, cols, pages) in octave idiom
		dv = dim_vector(octave_idx_type(dims[0]), octave_idx_type(dims[1]), octave_idx_type(dims[2])); 
		perm_vect(2)=2;
	} else {
		error(QUOTED(OCT_FN_NAME)": %i dimensions. not supported: %s",(int)ndim, filename.c_str());
		return retval;
	}

	if ( gdcm::PixelFormat::UINT32 == image.GetPixelFormat() ) { //tested
		uint32NDArray arr(dv);
		image.GetBuffer((char *)arr.fortran_vec());
		return octave_value(arr.permute(perm_vect));
	} else if ( gdcm::PixelFormat::UINT16 == image.GetPixelFormat() ) { //tested
		uint16NDArray arr(dv);
		image.GetBuffer((char *)arr.fortran_vec());
		return octave_value(arr.permute(perm_vect));
	} else if ( gdcm::PixelFormat::UINT8 == image.GetPixelFormat() ) { //tested
		uint8NDArray arr(dv);
		image.GetBuffer((char *)arr.fortran_vec());
		return octave_value(arr.permute(perm_vect));
	} else if ( gdcm::PixelFormat::INT8 == image.GetPixelFormat() ) { // no example found to test
		int8NDArray arr(dv);
		image.GetBuffer((char *)arr.fortran_vec());
		return octave_value(arr.permute(perm_vect));
	} else if ( gdcm::PixelFormat::INT16 == image.GetPixelFormat() ) { // no example found to test
		int16NDArray arr(dv);
		image.GetBuffer((char *)arr.fortran_vec());
		return octave_value(arr.permute(perm_vect));
	} else if ( gdcm::PixelFormat::INT32 == image.GetPixelFormat() ) { // no example found to test
		int32NDArray arr(dv);
		image.GetBuffer((char *)arr.fortran_vec());
		return octave_value(arr.permute(perm_vect));
	} else {
		octave_stdout << image.GetPixelFormat() << '\n' ;
		error(QUOTED(OCT_FN_NAME)": pixel format not supported yet: %s", filename.c_str());
		return retval;
	}
	
}

/*
%!shared testfile
%! testfile = urlwrite ( ...
%!   'http://sourceforge.net/p/octave/code/11601/tree/trunk/octave-forge/extra/dicom/dcm_examples/RD.15MV.DCM?format=raw', ...
%!   tempname() );

%!fail ("dicomread");
%!fail ("dicomread (1)");
%!fail ("dicomread ('hopefully_a_non_existant_file')");

%!test
%! rd=dicomread(testfile);
%! assert(rd(100,101,30),uint16(2021));
%! % todo

%! if exist (testfile, 'file')
%!   delete (testfile);
%! endif
*/
