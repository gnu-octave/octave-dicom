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
#include "octave/ov-struct.h"

#include "gdcmDictEntry.h"
#include "gdcmImage.h"
#include "gdcmImageWriter.h"
#include "gdcmFileDerivation.h"
#include "gdcmUIDGenerator.h"
              
#include "dicomdict.h"
              
#define DICOM_ERR -1
#define DICOM_OK 0

#define OCT_FN_NAME dicomwrite
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

// TODO: allow user to specify colourmap
// TODO: allow user to specify metadata
// TODO: return status, like the help output says

// TODO all fns here should throw exceptions, not use this "std::string & err" arg

void struct2metadata(gdcm::ImageWriter *w, gdcm::File *file, const octave_value  & ov, bool trial, int sequenceDepth) ;
void structarray2sequence(gdcm::SequenceOfItems & sq, Octave_map * om, bool trial, int sequenceDepth);
void value2element (gdcm::DataElement * de, const octave_value * ov, gdcm::Tag * tag, const std::string & keyword, bool trial, bool * handled, int sequenceDepth);
void octaveVal2dicomImage(gdcm::ImageWriter *w, octave_value *pixval) ;
void genMinimalMetaData(gdcm::ImageWriter *w, gdcm::File *file);

DEFUN_DLD (OCT_FN_NAME, args, nargout,"\
dicomwrite(im, filename)\n\
dicomwrite(im, filename, info)\n\
im       image data or empty matrix, [], if only metadata save is required\n\
filename to write dicom to. if [], then function runs in verbose trial mode.\n\
info     struct, like that produced by dicominfo\n\
") {
	octave_value_list retval;  // create object to store return values
	if(2>args.length()) {
		error(QUOTED(OCT_FN_NAME)": should have at least 2 arguments");
		return retval; 
	}
	charMatrix ch = args(1).char_matrix_value ();
	bool trial = false;
	if (0 == ch.numel()) {
		trial = true; // more output if null string supplied for filename;
	} else if (ch.rows()!=1) {
		error(QUOTED(OCT_FN_NAME)": second arg should be a filename");
		return retval; 
	}
	std::string filename = ch.row_as_string(0);
	
	//bool fn_failed_verbose=trial; //TODO other way of setting this. is it even necessary?
	bool writing_image = (0 != args(0).numel()) ;
		
	// prepare writer
	gdcm::ImageWriter w;
	if (!trial) w.SetFileName( filename.c_str());

	gdcm::SmartPointer<gdcm::File> file = new gdcm::File; // should delete be used later?
	
	if (3<args.length()) { // no metadata supplied, need to make some
		try {
			genMinimalMetaData(&w, file);
		} catch (std::exception) {
			return retval ;
		}
	} else { // 3rd arg should be struct to turn into metadata
		try {
			struct2metadata(&w, file, args(2), trial, 0 /* depth of indent for SQ nesting */);
		} catch (std::exception) {
			return retval ;
		}
	}

	if ( writing_image ) { 
		octave_value pixval = args(0);
		try {
			octaveVal2dicomImage(&w, &pixval);
		} catch (std::exception) {
			return retval ;
		}
	}
	
#if 1 /* debug */
	gdcm::File fc=w.GetFile();
	gdcm::DataSet dsc=fc.GetDataSet();
	const gdcm::DataElement & sopclass = dsc.GetDataElement( gdcm::Tag(0x0008, 0x0016) ); // SOPClassUID
	const gdcm::ByteValue *bv = sopclass.GetByteValue();
	char * bufc=(char *) malloc((bv->GetLength()+1)*sizeof(char));
	memcpy(bufc, bv->GetPointer(), bv->GetLength());
	bufc[bv->GetLength()]='\0';
	octave_stdout << bufc << '\n' ;
	free(bufc);
#endif
	
	if( !trial ) {
		// if not writing image, use superclass
		if (writing_image ? !w.ImageWriter::Write() : !w.Writer::Write()){ 
			error(QUOTED(OCT_FN_NAME)": unable to save");
			return retval; 
		}
	}

	return retval;
}

void struct2metadata(gdcm::ImageWriter *w, gdcm::File *file, const octave_value  & ov, bool trial, int sequenceDepth) {
	if(!ov.is_map()){
		error(QUOTED(OCT_FN_NAME)": 3rd arg should be struct holding metadata. it is %s",ov.type_name().c_str()); 
		throw std::exception() ;
	}
	gdcm::DataSet ds;
	gdcm::FileMetaInformation hds;
	Octave_map om=ov.map_value();
	uint32_t skipped = 0;
	for (Octave_map::iterator it = om.begin(); it != om.end(); it++) {
		std::string keyword(om.key(it));
		Cell cell = om.contents(it);
		if (!dicom_is_present(keyword)) {
			if ( 0==keyword.compare("FileModDate") || 0==keyword.compare("Filename")) {
				if (trial) octave_stdout << "from dicominfo, ignoring:" << keyword << ":[" << cell(0).string_value() << "]\n";
				continue; //dicominfo adds these non-DICOM bits
			}
			skipped++;
			// warning(QUOTED(OCT_FN_NAME)": skipping \"%s\". not in dictionary\n",keyword.c_str());
			continue;
		}
		// PixelData is here becuase it handled by other fn. the others seem to cause bugs
		if ( 0==keyword.compare("PixelData") || 0==keyword.compare("FileMetaInformationVersion") ) {
			if (trial) octave_stdout << "handled separately:" << keyword << std::endl;
			continue; 
		}
		gdcm::DataElement de;
		gdcm::Tag tag;
		bool handled ;
		try {
			value2element(&de, &cell(0), &tag, keyword, trial, &handled, sequenceDepth);
		} catch (std::exception) {
			return;
		}
		if (handled) {
			if (tag.GetGroup() == (uint32_t)0x0002 ) { // group 0x2 : header
				hds.Insert(de) ;
				// TODO move this somehow: if (trial) octave_stdout << ':' << "(header)";
			} else { // everything else is metadata
				ds.Insert(de);
			}
		}
	}
	if (skipped>0) {
		// TODO: this does not count elements nested in SQs
		warning(QUOTED(OCT_FN_NAME)": skipped %i keyword-value pairs. not in dictionary\n",skipped);
	}
	// TODO are these set functions taking a copy? if they take a reference to objects that are about to go out of scope, we have a problem
	file->SetDataSet((gdcm::DataSet &)ds);
	file->SetHeader(hds);
	w->SetFile(*file);
	return ;
}

void structarray2sequence(gdcm::SequenceOfItems & sq, Octave_map * om, bool trial, int sequenceDepth) {
	for (Octave_map::iterator it = om->begin(); it != om->end(); it++) {
		gdcm::Item item;
		// item.SetVLToUndefined(); //TODO: does VL need to be set for items that contain datasets?
		gdcm::DataSet &nds = item.GetNestedDataSet();
		std::string itemname(om->key(it));
		// TODO: test itemname is something like Item_n.
		Cell cell = om->contents(it);
		Octave_map subom = cell(0).map_value();
		// octave_stdout << itemname <<std::endl;
		for (Octave_map::iterator subit = subom.begin(); subit != subom.end(); subit++) {
			std::string subkeyword(subom.key(subit));
			gdcm::DataElement de;
			gdcm::Tag tag;
			bool handled;
			try {
				value2element(&de, &(subom.contents(subit)(0)), &tag, subkeyword, trial, &handled, sequenceDepth);
			} catch (std::exception) {
				return;
			}
			if (!handled) {
				warning(QUOTED(OCT_FN_NAME)": element in sequence not handled %s", subkeyword.c_str());
				continue;
			}
			nds.Insert(de); //insert element into dataset
		}
		sq.AddItem(item); // add dataset to sequence
	}
	return ;
}

void value2element (gdcm::DataElement * de, const octave_value * ov, gdcm::Tag * tag, const std::string & keyword, bool trial, bool * handled, int sequenceDepth) {
	gdcm::DictEntry entry;
	if (!dicom_is_present(keyword)) {
		if (trial) {
			octave_stdout << std::setw(2*sequenceDepth) << "" << std::setw(0) ;
			octave_stdout << keyword << ": not in dictionary" << std::endl ;
		}
		*handled=false;
		return ;
	}
	lookup_dicom_tag(*tag, keyword);
	lookup_dicom_entry(entry, *tag);
	gdcm::VL len((uint32_t)ov->byte_size());
	//gdcm::DataElement de(*tag, len, entry.GetVR()); 
	de->SetTag(*tag); de->SetVL(len); de->SetVR(entry.GetVR());
	*handled = true;
	if (trial) {
		octave_stdout << std::setw(2*sequenceDepth) << "" << std::setw(0) ;
		octave_stdout << *tag << ':' << entry.GetVR() << ':' << keyword << ':' ;
	}
	if ( entry.GetVR() & VRSTRING ) {// TODO: check even padding requirement
		// TODO some dicom string types are stored as numbers
		if (!ov->is_string()) {
			warning(QUOTED(OCT_FN_NAME)": dicomdict gives string VR for %s, octave value is %s", keyword.c_str(), ov->class_name().c_str());
		}
		if (trial) octave_stdout << '[' << ov->string_value() << ']' << std::endl ;
		de->SetByteValue((const char *)ov->string_value().c_str(),len);
	} else if ( entry.GetVR() & gdcm::VR::US) { // unsigned short
		if (!ov->is_scalar_type()) { // dicominfo turns US into double, this turns it back to uint16
			warning(QUOTED(OCT_FN_NAME)": dicomdict gives VR of US for %s, octave value is %s", keyword.c_str(), ov->class_name().c_str());
		}
		if (trial) octave_stdout << '[' << ov->uint16_scalar_value() << ']' << std::endl ;
		de->SetByteValue((const char *)ov->uint16_array_value().fortran_vec(), gdcm::VL((uint32_t)2));
	} else if ( entry.GetVR() & gdcm::VR::OB) { // other byte
		uint8NDArray obv = ov->uint8_array_value();
		//if (!ov->is_scalar_type()) { // dicominfo seems to return string
		//	warning(QUOTED(OCT_FN_NAME)": dicomdict gives VR of OB for %s, octave value is %s", keyword.c_str(), ov->class_name().c_str());
		//}
		if (trial) { // TODO surely there is a better way to do this with c++ output stream
			octave_uint8 * obv_p=obv.fortran_vec();
			char buf[8];
			octave_stdout  << '[' ;
			for (octave_idx_type i=0; i<(octave_idx_type)len; i++) {
				snprintf(buf,7,"%02X ",(const uint8_t)obv_p[i]);
				octave_stdout  << buf << " " ;
			}
			octave_stdout  << ']' << std::endl ;
		}
		de->SetByteValue((const char *)obv.fortran_vec(), gdcm::VL((uint32_t)obv.byte_size()));
	} else if ( entry.GetVR() & gdcm::VR::DS) { // double string. sep by /
		if (!ov->is_double_type()) { 
			warning(QUOTED(OCT_FN_NAME)": dicomdict gives VR of DS for %s, expecting double, octave value is %s", keyword.c_str(), ov->class_name().c_str());
		}
		std::stringstream ss;
		Matrix mat=ov->matrix_value() ; //to matrix of doubles
		double * mat_p=mat.fortran_vec();
		for(int i=0; i<mat.numel() ;i++) ss << mat_p[i] << '/' ;
		std::string s=ss.str();
		s=s.substr(0,s.length()-1); // strip last /
		if (s.length()%2==1) s=s+" "; // ensure even number of chars
		if (trial) octave_stdout << '[' << s << ']' << std::endl ;
		de->SetByteValue( (const char *)s.c_str(), gdcm::VL((uint32_t)s.length()) );
	} else if ( entry.GetVR() & gdcm::VR::IS) { // integer string. only single values, I think
		if (!ov->is_scalar_type()) { 
			warning(QUOTED(OCT_FN_NAME)": dicomdict gives VR of IS for %s, octave value is %s", keyword.c_str(), ov->class_name().c_str());
		}
		int32_t val=ov->int32_scalar_value() ;
		char buf[16];
		snprintf(buf,14,"%i",val);
		int len=strlen(buf);
		if (len%2==1) {buf[len]=' '; buf[len+1]='\0'; }// ensure even number of chars
		if (trial) octave_stdout << '[' << buf << ']' << std::endl;
		de->SetByteValue( buf, gdcm::VL((uint32_t)strlen(buf)) );
	} else if ( entry.GetVR() & gdcm::VR::SQ) { // sequence
		if (!ov->is_map()) { 
			warning(QUOTED(OCT_FN_NAME)": dicomdict gives VR of SQ for %s, octave value is %s", keyword.c_str(), ov->class_name().c_str());
		}
		octave_stdout << std::endl;
		//int nObj = ov->numel() ;
		Octave_map subom = ov->map_value();
		gdcm::SmartPointer<gdcm::SequenceOfItems> sq = new gdcm::SequenceOfItems();
		try {
			structarray2sequence(*sq, &subom, trial, ++sequenceDepth) ;
		} catch (std::exception) {
			return;
		}
		sequenceDepth--;
	} else {
		if (trial) octave_stdout << " ### not handled ### " << std::endl;
		*handled = false;
	}
}

// TODO set HighBit etc using octave class. or cast pixel data using metadata, or just give error if they don't agree
void octaveVal2dicomImage(gdcm::ImageWriter *w, octave_value *pixval) {
	if (pixval->ndims() != 2) {
		error(QUOTED(OCT_FN_NAME)": image has %i dimensions. not implemented. ", pixval->ndims());
		throw std::exception();
	}
	
	// make image
	gdcm::SmartPointer<gdcm::Image> im = new gdcm::Image;

	im->SetNumberOfDimensions( 2 );
	im->SetDimension(0, pixval->dims()(0) );
	im->SetDimension(1, pixval->dims()(1) );
	im->SetPhotometricInterpretation( gdcm::PhotometricInterpretation::MONOCHROME1 );
	
	// prepare to make data from mat available
	char * matbuf; // to be set to point to octave matrix 
	
	if (pixval->is_uint8_type()) {
		matbuf=(char * )pixval->uint8_array_value().fortran_vec();
		im->GetPixelFormat().SetScalarType(gdcm::PixelFormat::UINT8);
	} else if (pixval->is_uint16_type()) {
		matbuf=(char * )pixval->uint16_array_value().fortran_vec();
		im->GetPixelFormat().SetScalarType(gdcm::PixelFormat::UINT16);
	} else if (pixval->is_uint32_type()) {
		matbuf=(char * )pixval->uint32_array_value().fortran_vec();
		im->GetPixelFormat().SetScalarType(gdcm::PixelFormat::UINT32);
	} else if (pixval->is_int8_type()) {
		matbuf=(char * )pixval->int8_array_value().fortran_vec();
		im->GetPixelFormat().SetScalarType(gdcm::PixelFormat::INT8);
	} else if (pixval->is_int16_type()) {
		matbuf=(char * )pixval->int16_array_value().fortran_vec();
		im->GetPixelFormat().SetScalarType(gdcm::PixelFormat::INT16);
	} else if (pixval->is_int32_type()) {
		matbuf=(char * )pixval->int32_array_value().fortran_vec();
		im->GetPixelFormat().SetScalarType(gdcm::PixelFormat::INT32);
	} else { // TODO: gdcm::PixelFormat has float types too. 
		error(QUOTED(OCT_FN_NAME)": cannot write this type"); 
		throw std::exception();
	}
	
	unsigned long buflen=im->GetBufferLength();
	if (buflen != pixval->byte_size()) { 
		error(QUOTED(OCT_FN_NAME)": prepared DICOM buffer size(%i) does not match Octave array buffer size(%i).",buflen,pixval->byte_size());
		throw std::exception() ;
	}

	gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
	pixeldata.SetByteValue( matbuf, buflen );

	im->SetDataElement( pixeldata );
	
	w->SetImage( *im );
	return ;
}

void genMinimalMetaData(gdcm::ImageWriter *w, gdcm::File *file){
	gdcm::UIDGenerator uid; // helper for uid generation

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
		throw std::exception();
	}
	
	w->SetFile( fd.GetFile() );

	return ;
}
