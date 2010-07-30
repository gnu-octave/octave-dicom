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

#include "gdcmImage.h"
#include "gdcmImageWriter.h"
#include "gdcmFileDerivation.h"
#include "gdcmUIDGenerator.h"
              
#define DICOM_ERR -1
#define DICOM_OK 0

#define OCT_FN_NAME dicomwrite
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

// TODO: allow user to specify colourmap
// TODO: allow user to specify metadata
// TODO: return status, like the help output says

DEFUN_DLD (OCT_FN_NAME, args, nargout,"\
dicomwrite(im, filename)\n\
dicomwrite(im, filename, info)\n\
im       image data or empty matrix, [], if only metadata save is required\n\
filename to write dicom to\n\
info     struct, like that produced by dicominfo\n\
") {
	octave_value_list retval;  // create object to store return values
	if(2>args.length()) {
		error(QUOTED(OCT_FN_NAME)": should have at least 2 arguments");
		return retval; 
	}
	charMatrix ch = args(1).char_matrix_value ();
	if (ch.rows()!=1) {
		error(QUOTED(OCT_FN_NAME)": second arg should be a filename");
		return retval; 
	}
	std::string filename = ch.row_as_string(0);
	
	// prepare writer
	gdcm::ImageWriter w;
	w.SetFileName( filename.c_str());
	
	if (0 != args(0).numel() ) { // if image matrix arg is not empty
		if (args(0).ndims() != 2) {
			error(QUOTED(OCT_FN_NAME)": image has wrong number of dimensions.");
			return retval;
		}
		
		// make image
		gdcm::SmartPointer<gdcm::Image> im = new gdcm::Image;

		im->SetNumberOfDimensions( 2 );
		im->SetDimension(0, args(0).dims()(0) );
		im->SetDimension(1, args(0).dims()(1) );

		im->SetPhotometricInterpretation( gdcm::PhotometricInterpretation::MONOCHROME1 );
		
		// prepare to make data from mat available
		char * matbuf; // to be set to point to octave matrix 
		unsigned short bits;
		
		if (args(0).is_uint8_type()) {
			matbuf=(char * )args(0).uint8_array_value().fortran_vec();
			bits = 8;
		} else if (args(0).is_uint16_type()) {
			matbuf=(char * )args(0).uint16_array_value().fortran_vec();
			bits = 16;
		} else if (args(0).is_uint32_type()) {
			matbuf=(char * )args(0).uint32_array_value().fortran_vec();
			bits = 32;
		} else { // TODO: gdcm::PixelFormat has float types too.
			error(QUOTED(OCT_FN_NAME)": cannot write this type"); 
			return retval;
		}
		
		//                                   samplesperpixel bitsallocated bitsstored highbit pixelrepresentation (0 unsigned, 1 signed)
		im->SetPixelFormat(gdcm::PixelFormat(1,              bits,         bits,      bits-1, 0));
			
		unsigned long buflen=im->GetBufferLength();
		if (buflen != args(0).byte_size()) { /* this is triggered for uint16 and 32. only works for 8. GDCM bug? */
			error(QUOTED(OCT_FN_NAME)": %s: prepared DICOM buffer size(%i) does not match Octave array buffer size(%i).",filename.c_str(),buflen,args(0).byte_size());
			return retval;
		}

		gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
		pixeldata.SetByteValue( matbuf, buflen );

		im->SetDataElement( pixeldata );
		
		w.SetImage( *im );
	}
	
	if (3<args.length()) { // no metadata supplied, need to make some
		gdcm::UIDGenerator uid; // helper for uid generation

		gdcm::SmartPointer<gdcm::File> file = new gdcm::File; // empty file

		// Step 2: DERIVED object
		gdcm::FileDerivation fd; //TODO: copied this: don't understand it
		// For the pupose of this execise we will pretend that this image is referencing
		// two source image (we need to generate fake UID for that).
		const char ReferencedSOPClassUID[] = "1.2.840.10008.5.1.4.1.1.7"; // Secondary Capture
		fd.AddReference( ReferencedSOPClassUID, uid.Generate() );
		fd.AddReference( ReferencedSOPClassUID, uid.Generate() );

		// Again for the purpose of the exercise we will pretend that the image is a
		// multiplanar reformat (MPR):
		// CID 7202 Source Image Purposes of Reference
		// {"DCM",121322,"Source image for image processing operation"},
		fd.SetPurposeOfReferenceCodeSequenceCodeValue( 121322 );
		// CID 7203 Image Derivation
		// { "DCM",113072,"Multiplanar reformatting" },
		fd.SetDerivationCodeSequenceCodeValue( 113072 );
		fd.SetFile( *file );
		// If all Code Value are ok the filter will execute properly
		if( !fd.Derive() ) {
			error(QUOTED(OCT_FN_NAME)": file derivation failed");
			return retval;
		}
		
		w.SetFile( fd.GetFile() );

	}

	if( !w.Write() ) {
		error(QUOTED(OCT_FN_NAME)": unable to save");
		return retval;
	}

	//delete mat;
	return retval;
}