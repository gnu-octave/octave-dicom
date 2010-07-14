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
	
	if( 2==ndim ) {
		error(QUOTED(OCT_FN_NAME)": 2D. not implemented yet: %s",ndim, filename.c_str());
		//create output arg
		return retval;
	} else if (3==ndim) {
		octave_idx_type cols = dims[0]; // NB cols, then rows in DICOM. octave
		octave_idx_type rows = dims[1];
		octave_idx_type frms = dims[2]; // third dim called frames in DICOM
		if ( gdcm::PixelFormat::UINT32 != image.GetPixelFormat() ) {
			error(QUOTED(OCT_FN_NAME)": pixel format not supported yet: %s", filename.c_str());
			return retval;
		}
		if ( (unsigned short)1 != image.GetPixelFormat().GetSamplesPerPixel() ) {
			error(QUOTED(OCT_FN_NAME)": only 1 sample per pixel supported. has %i : %s", 
					image.GetPixelFormat().GetSamplesPerPixel(), filename.c_str());
			return retval;
		}
		//                                should be (rows, cols, pages) in octave idiom
		dim_vector dv(cols, rows, frms);//this transposes first two dimensions
		uint32NDArray arr(dv);
		image.GetBuffer((char *)arr.fortran_vec());
		return octave_value(arr);
	} else {
		error(QUOTED(OCT_FN_NAME)": %i dimensions. not supported: %s",ndim, filename.c_str());
		return retval;
	}

}


