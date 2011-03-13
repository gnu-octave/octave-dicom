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

#include "gdcmImageReader.h"
              
#define DICOM_ERR -1
#define DICOM_OK 0

#define OCT_FN_NAME dicomread
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

DEFUN_DLD (OCT_FN_NAME, args, nargout,
		"return some info from a dicom file: 1 arg: dicom filename") {
	octave_value_list retval;  // create object to store return values
	if ( 0 == args.length()) {
		error(QUOTED(OCT_FN_NAME)": one arg required: dicom filename");
		return retval; 
	}
	charMatrix ch = args(0).char_matrix_value ();
	if (ch.rows()!=1) {
		error(QUOTED(OCT_FN_NAME)": arg should be a filename, 1 row of chars");
		return retval; 
	}
		
	std::string filename = ch.row_as_string(0);
	
#if 0 /* TODO support 'frames' stuff, see Matlab docs for dicomread */
	int i; // parse any additional args
	for (i=1; i<args.length(); i++) {
		charMatrix chex = args(i).char_matrix_value();
		if (chex.rows()!=1) {
			error(QUOTED(OCT_FN_NAME)": arg should be a string, 1 row of chars");
			return retval; 
		}
		std::string argex = chex.row_as_string (0);
		if (!argex.compare(0,9,"truncate=")) {
			dicom_truncate_numchar=atoi(argex.substr(9).c_str());
		} else {
			warning(QUOTED(OCT_FN_NAME)": arg not understood: %s", argex.c_str());
		}
	}
#endif
	
	gdcm::ImageReader reader;
	reader.SetFileName( filename.c_str() );
	if( !reader.Read() ) {
		octave_stdout << "filename:" << filename << '\n' ;
		error(QUOTED(OCT_FN_NAME)": Could not read DICOM file with image: %s",filename.c_str());
		return retval;
	}
	
	const gdcm::Image &image = reader.GetImage();

	octave_idx_type ndim = image.GetNumberOfDimensions();
	const unsigned int *dims = image.GetDimensions();
	// dim 0: cols (width)
	// dim 1: rows (height)
	// dim 2: number of frames
	
	dim_vector *dv_p;
	
	if( 2==ndim ) {
		dv_p=new dim_vector(dims[0], dims[1]); //this transposes first two dimensions
	} else if (3==ndim) {
		dv_p=new dim_vector(dims[0], dims[1], dims[2]); // should be (rows, cols, pages) in octave idiom
	} else {
		error(QUOTED(OCT_FN_NAME)": %i dimensions. not supported: %s",ndim, filename.c_str());
		return retval;
	}
	
	if ( gdcm::PixelFormat::UINT32 == image.GetPixelFormat() ) { //tested
		uint32NDArray arr(*dv_p);
		image.GetBuffer((char *)arr.fortran_vec());
		delete dv_p;
		return octave_value(arr);
	} else if ( gdcm::PixelFormat::UINT16 == image.GetPixelFormat() ) { //tested
		uint16NDArray arr(*dv_p);
		image.GetBuffer((char *)arr.fortran_vec());
		delete dv_p;
		return octave_value(arr);
	} else if ( gdcm::PixelFormat::UINT8 == image.GetPixelFormat() ) { //tested
		uint8NDArray arr(*dv_p);
		image.GetBuffer((char *)arr.fortran_vec());
		delete dv_p;
		return octave_value(arr);
	} else if ( gdcm::PixelFormat::INT8 == image.GetPixelFormat() ) { // no example found to test
		int8NDArray arr(*dv_p);
		image.GetBuffer((char *)arr.fortran_vec());
		delete dv_p;
		return octave_value(arr);
	} else if ( gdcm::PixelFormat::INT16 == image.GetPixelFormat() ) { // no example found to test
		int16NDArray arr(*dv_p);
		image.GetBuffer((char *)arr.fortran_vec());
		delete dv_p;
		return octave_value(arr);
	} else if ( gdcm::PixelFormat::INT32 == image.GetPixelFormat() ) { // no example found to test
		int32NDArray arr(*dv_p);
		image.GetBuffer((char *)arr.fortran_vec());
		delete dv_p;
		return octave_value(arr);
	} else {
		octave_stdout << image.GetPixelFormat() << '\n' ;
		error(QUOTED(OCT_FN_NAME)": pixel format not supported yet: %s", filename.c_str());
		return retval;
	}
	
}


