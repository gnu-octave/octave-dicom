/*
 * The GNU Octave dicom package is Copyright Andy Buckle 2010
 * Contact: blondandy using the sf.net system, 
 * <https://sourceforge.net/sendmessage.php?touser=1760416>
 * 
 * Changes Copyright Kris Thielemans 2011:
 * - support usage dicominfo(filename, 'dictionary', dictname)
 * - support FL, FD and SL VRs, which means that many more fields are now read correctly from the dicom file.
 * - check if the VR in the file is the same as the one in the dictionary. If not, issue a warning but use the VR from the file.
 * - assign values to private dicom fields, just like with others
 * - changed convention for private fields to use lower-case for the hexadecimal numbers to be compatible with Matlab
 * - if an entry is not in the dictionary, determine its VR from the file (if possible) and assign anyway.
 * - updated doc-string
 * - use gdcm functions for getting values as opposed to memcpy, and support arrays (VM>1)
 * - don't skip sequences
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
 
#include <stdlib.h> //for calloc, free
#include <stdio.h>  //for printf

#include <sys/stat.h>
#include <time.h>

#include <iostream>
#include <string>

#include "octave/oct.h"
#include "octave/ov-struct.h"

#include "gdcmReader.h"
#include "gdcmDataSet.h"
#include "gdcmDict.h"
#include "gdcmVR.h"
#include "gdcmSequenceOfItems.h"
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dicomdict.h" 


  
#define DICOM_ERR -1
#define DICOM_OK 0
#define DICOM_NOTHING_ASSIGNED 1

#define TIME_STR_LEN 31

#define OCT_FN_NAME dicominfo
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

#ifdef NOT_OCT
#	define octave_stdout	std::cout
#	define error			printf
#endif

char* byteval2string(char * d, int d_len_p, const gdcm::ByteValue *bv);
char* name2Keyword(char *d, int *d_len_p, const char* s);
Matrix str2DoubleVec(const char*);
octave_map dump(const char filename[], int chatty);
void dumpDataSet(octave_map *om, const gdcm::DataSet *ds, int chatty, int sequenceDepth);
void getFileModTime(char *timeStr, const char *filename);
void dumpElement(octave_map *om, const gdcm::DataElement * elem, int chatty, int sequenceDepth);
void dumpSequence(octave_value *ov, gdcm::SequenceOfItems *seq, int chatty, int sequenceDepth);
int element2value(std::string & varname, octave_value *ov, const gdcm::DataElement * elem, int chatty, int sequenceDepth) ;

int dicom_truncate_numchar=40;

#ifdef NOT_OCT
int main( int argc, const char* argv[] ) {
	dump(argv[1], 1 /* chatty on */ ); // 1 cmd line arg: dicom filename
	return 0;
}
#else
DEFUN_DLD (dicominfo, args, nargout,
		"-*- texinfo -*- \n\
 @deftypefn {Loadable Function} {@var{info}} = dicominfo (@var{filename}) \n\
 @deftypefnx {Loadable Function} {@var{info}} = dicominfo (@var{filename}, @code{dictionary}, @var{dictionary-name}) \n\
 @deftypefnx  {Loadable Function} {} dicominfo (@var{filename}, @var{options}) \n\
 @deftypefnx {Command} {} dicominfo @var{filename} \n\
 @deftypefnx {Command} {} dicominfo @var{filename} @var{options} \n\
 Get all data from a DICOM file, excluding any actual image. \n\
 @var{info} is a nested struct containing the data. \n\
 \n\
 If no return argument is given, then there will be output similar to \n\
 a DICOM dump. \n\
 \n\
 If the @code{dictionary} argument is used, the given @var{dictionary-name} is used for this operation, \n\
 otherwise, the dictionary set by @code{dicomdict} is used.\n\
 \n\
 @var{options}:\n\
 @code{truncate=n}\n\
 where n is the number of characters to limit the dump output display to @code{n}\
 for each value. \n\
\n\
 @seealso{dicomread, dicomdict} \n\
 @end deftypefn \n\
		") {
	octave_value_list retval;  // create object to store return values
	if ( (0 == args.length()) || (! args(0).is_string ())) {
		error(QUOTED(OCT_FN_NAME)": one arg required: dicom filename");
		return retval; 
	}
	int chatty = !nargout; // dump output to stdout if not assigning to var
	charMatrix ch = args(0).char_matrix_value ();
	if (ch.rows()!=1) {
		error(QUOTED(OCT_FN_NAME)": arg should be a filename, 1 row of chars");
		return retval; 
	}
	std::string filename = ch.row_as_string (0);

	// save current dictionary for the case that we're using a different dictionary while reading
	const std::string current_dict = get_current_dicom_dict();

	int i; // parse any additional args
	for (i=1; i<args.length(); i++) {
		charMatrix chex = args(i).char_matrix_value();
		if (chex.rows()!=1) {
			error(QUOTED(OCT_FN_NAME)": arg should be a string, 1 row of chars");
			load_dicom_dict(current_dict.c_str()); // reset dictionary to initial value
			return retval; 
		}
		std::string argex = chex.row_as_string (0);
		if (!argex.compare("dictionary") || !argex.compare("Dictionary")) {
		        if (i+1==args.length()) {
			      error(QUOTED(OCT_FN_NAME)": Dictionary needs another argument");
			      load_dicom_dict(current_dict.c_str()); // reset dictionary to initial value
			      return retval;
			}
			if (!args(i+1).is_string()) {
			      error(QUOTED(OCT_FN_NAME)": Dictionary needs a string argument");
			      load_dicom_dict(current_dict.c_str()); // reset dictionary to initial value
			      return retval;
			}
			std::string dictionary = args(i+1).string_value();
			load_dicom_dict(dictionary.c_str());
			// ignore dictionary argument for further arg processing
			++i;
		}
		else if (!argex.compare(0,9,"truncate=")) {
			dicom_truncate_numchar=atoi(argex.substr(9).c_str());
		} else {
			warning(QUOTED(OCT_FN_NAME)": arg not understood: %s", argex.c_str());
		}
	}
	
	octave_map om=dump(filename.c_str(),chatty);
	retval(0)=om;

	load_dicom_dict(current_dict.c_str()); // reset dictionary to initial value
	return retval;
}
#endif


octave_map dump(const char filename[], int chatty) {
	// output struct
	octave_map om;
	// Instantiate the reader:
	gdcm::Reader reader;
	reader.SetFileName( filename );
	if( !reader.Read() ) {
		error("Could not read: %s",filename);
		return om; //TODO: set error state somehow so the main DEFUN_DLD function knows
		           // KT this doesn't seem to be necessary. Presumably error() sets a flag that tells the interpreter it should abort
	}
	gdcm::File &file = reader.GetFile();
	gdcm::DataSet &ds = file.GetDataSet();
	gdcm::FileMetaInformation &hds=file.GetHeader();
	
	om.assign("Filename", octave_value(filename));
	char dateStr[TIME_STR_LEN+1];
	getFileModTime(dateStr, filename);
	om.assign("FileModDate", octave_value(dateStr));
	if(chatty) octave_stdout << "# file info\nFilename:" 
		<< filename << "\nFileModDate:" << dateStr << '\n';
	
	if(chatty) octave_stdout << "# header\n" ;
	dumpDataSet(&om, &hds, chatty, 0);
	if(chatty) octave_stdout << "# metadata\n" ;
	dumpDataSet(&om, &ds, chatty, 0);
	
	return om;
}

void dumpDataSet(octave_map *om, const gdcm::DataSet *ds, int chatty, int sequenceDepth) {
	
	const gdcm::DataSet::DataElementSet DES=ds->GetDES(); // gdcm::DataSet::DataElementSet is a std::set
	gdcm::DataSet::Iterator it;
	
	for ( it=DES.begin() ; it != DES.end(); it++ ) {		
		dumpElement(om, &(*it), chatty, sequenceDepth);
	}
}

void dumpElement(octave_map *om, const gdcm::DataElement * elem, 
				int chatty, int sequenceDepth) {
	std::string varname;
	octave_value ov;
	if(DICOM_OK==element2value(varname, &ov, elem, chatty, sequenceDepth)) {
		om->assign(varname.c_str(), ov);
	} else {
		if (0==varname.length()) return ;
		om->assign(varname.c_str(), octave_value("not assigned"));
	}
}

/* helper function for element2value below.
   This function converts a 'simple' type (such as an (array of) floats or ints, etc)
   to an octave_value, returning DICOM_NOTHING_ASSIGNED if the dicom element is empty
   or DICOM_OK otherwise.

   It is templated in 
   - the VR Type, such that we can use GDCM's functions to 
     convert data appropriately.
   - valueType, i.e. float, or int etc
   - octaveArrayType, i.e. Array<float>, or intNDArray<octave_int<int> >, etc

  There are other helper functions below that find the template arguments.

  Note, the octaveArrayType is currently necessary as we cannot use Array<int> etc, 
  as octave_value does not have a constructor for Array<int>.
*/
template <gdcm::VR::VRType vrtype, typename valueType, typename octaveArrayType>
static inline
int element2simplevalueHelper2(octave_value *ov, const gdcm::DataElement *elem,
			    const int chatty) {
    if( elem->IsEmpty() )
      return DICOM_NOTHING_ASSIGNED;

#if 0 // KT if to choose between implementations
    // original fast implementation using memcpy. However, ignores byteorder and VMs (i.e. arrays)
    valueType val ; 
    memcpy(&val, elem->GetByteValue()->GetPointer(), sizeof(valueType));
    *ov=val;
    if(chatty) octave_stdout << '[' << val << "]\n";
#else
    // save (but slow?) implementation that uses GDCM functions
    gdcm::Element<vrtype,gdcm::VM::VM1_n> el;
    el.Set( elem->GetValue() );
    // possible optimisation here. If there's only 1 element, maybe we can
    // save time by not making array. However, because all octave values are
    // arrays, maybe this doesn't matter. the "else" statement below works
    // always in any case.
    // If you want to try this optimisation, put #if 1 below
#   if 0
    if (el.GetLength()==1)
      {
	const valueType& val = el.GetValue();
	*ov=val;
	if(chatty) octave_stdout << '[' << val << "]\n";
      }
    else
#   endif
      {
	octaveArrayType val(dim_vector (el.GetLength(),1));
	for (unsigned i=0; i<el.GetLength(); ++i)
	  val(i)  = el.GetValue(i); 
	*ov=val;
	if(chatty) octave_stdout << '[' << val << "]\n";
      }
#endif
    return DICOM_OK;
}

/* Helper functions for elemement2value for integer and real types.
   They simply call element2simplevalueHelper2 with the appropriate template arguments.
*/
template <gdcm::VR::VRType vrtype>
static inline 
int element2intvalueHelper(octave_value *ov, const gdcm::DataElement * elem,
			   const int chatty) {
  typedef typename gdcm::VRToType<vrtype >::Type valueType;
  return element2simplevalueHelper2<vrtype,valueType,intNDArray<octave_int<valueType> > >(ov, elem, chatty);
}


template <gdcm::VR::VRType vrtype>
static inline 
int element2realvalueHelper(octave_value *ov, const gdcm::DataElement * elem,
			   const int chatty) {
  typedef typename gdcm::VRToType<vrtype >::Type valueType;
  return element2simplevalueHelper2<vrtype,valueType,Array<valueType> >(ov, elem, chatty);
}

int element2value(std::string & varname, octave_value *ov, const gdcm::DataElement * elem, 
				int chatty, int sequenceDepth) {
	// get dicom dictionary
	//static const gdcm::Global& g = gdcm::Global::GetInstance();
	//static const gdcm::Dicts &dicts = g.GetDicts();
	const gdcm::Tag tag = elem->GetTag();		
	gdcm::VR vr = elem->GetVR(); // value representation

	// skip "Group Length" tags. note: these are deprecated in DICOM 2008
	if(tag.GetElement() == (uint16_t)0 || (elem->GetByteValue() == NULL && vr != gdcm::VR::SQ)) 
	  return DICOM_NOTHING_ASSIGNED;
	//const gdcm::DictEntry dictEntry = dicts.GetDictEntry(tag,(const char*)0);

	// find dictionary entry for name and to possibly adjust the VR
	gdcm::DictEntry dictEntry ;
	if (!dicom_is_present(tag)) {
		char fallbackVarname[64];
		snprintf(fallbackVarname,63,"Private_%04x_%04x",tag.GetGroup(),tag.GetElement());
		varname=std::string(fallbackVarname);
	}
	else {
	        lookup_dicom_entry(dictEntry, tag);
		varname=dictEntry.GetName();
		const gdcm::VR dictvr= dictEntry.GetVR(); // value representation. ie DICOM 	
		if (!vr.Compatible(vr)) {
    		  warning(QUOTED(OCT_FN_NAME)": %s has different VR from dictionary. Using VR from file.", varname.c_str());
		}
		// find vr from dictionary if it was not in the file
		// lines copied from gdcmPrinter::PrintDataElement
		if( vr == gdcm::VR::INVALID )
		  {
		    vr = dictvr;
		  }
		else if ( vr == gdcm::VR::UN && vr != gdcm::VR::INVALID ) // File is explicit, but still prefer vr from dict when UN
		  {
		    vr = dictvr;
		  }
		else // cool the file is Explicit !
		  {
		    // keep vr from file
		  }
#if 0
		// gdcmPrinter goes on with the following fix for the 
		// case where the VR derived from the dictionary is 'dual' (e.g. US_SS).
		// However, we cannot do it here as we don't have 
		// the FILE* F nor the dataset. 
		// This means that these fields will not be assigned.
		if( vr.IsDual() ) // This means vr was read from a dict entry:
		  {
		    vr = DataSetHelper::ComputeVR(*F,ds, tag);
		  }
#endif
	}


	//int tagVarNameBufLen=127;
	//char *keyword=(char *)malloc((tagVarNameBufLen+1)*sizeof(char));
	//keyword=name2Keyword(keyword,&tagVarNameBufLen,tagName);
	//*varname=std::string(keyword);
	
	if(chatty) {
		octave_stdout << std::setw(2*sequenceDepth) << "" << std::setw(0) ;
		octave_stdout << tag << ":" << vr << ":" << varname << ":" ;
		// TODO: error if var name repeated. 
	}
#define strValBufLen 511
	char strValBuf[strValBufLen+1];
	char* strVal=strValBuf;
	
	if ( vr & gdcm::VR::VRASCII) {
		strVal=byteval2string(strValBuf,strValBufLen,elem->GetByteValue()); 
		if(chatty) {
			if (dicom_truncate_numchar>0) {
				octave_stdout << '[' << std::string(strVal).substr(0,dicom_truncate_numchar) 
					<< ( ((int)strlen(strVal)>dicom_truncate_numchar) ? "..." : "") << ']' << std::endl;
			} else {
				octave_stdout << '[' << strVal << ']' << std::endl;
			}
		}
		if (vr & VRSTRING) { //all straight to string types
			*ov=std::string(strVal); // TODO: error if om already has member with this name
		} else if (vr & gdcm::VR::IS) { // Integer String. spec tallies with signed 32 bit int
			*ov=(int32_t)atoi(strVal); 
		} else if (vr & gdcm::VR::DS) { // Double String. vector separated by '/'
			Matrix vec=str2DoubleVec(strVal);
			*ov=vec;
		} else {
			if(chatty) octave_stdout << "   ### string type not handled ###" << std::endl;
			return DICOM_NOTHING_ASSIGNED;
		}
		if (strVal != strValBuf) free(strVal); // long string. malloc'd instead of using buf, now needs free'ng
	} else if (vr & gdcm::VR::SQ) {
		if(chatty) octave_stdout << " reading sequence. "; // \n provided in dumpSequence fn
		gdcm::SmartPointer<gdcm::SequenceOfItems> sqi = elem->GetValueAsSQ();
		dumpSequence(ov, sqi, chatty, sequenceDepth+1);
	} else if (vr & gdcm::VR::AT) { // attribute tag
		intNDArray<octave_uint16> uint16pair(dim_vector(1,2));
		uint16_t *p=(uint16_t *)elem->GetByteValue()->GetPointer();
                uint16pair(0) = p[0];
                uint16pair(1) = p[1];
		*ov=uint16pair;
		if (chatty) {
			char buf[16];
			snprintf(buf,15,"[(%04X,%04X)]\n",p[0],p[1]);
			octave_stdout << buf  ;
		}
	} else if (vr & gdcm::VR::FL) {// float
	  return element2realvalueHelper<gdcm::VR::FL>(ov, elem, chatty);
	} else if (vr & gdcm::VR::FD) {// double
	  return element2realvalueHelper<gdcm::VR::FD>(ov, elem, chatty);
	} else if (vr & gdcm::VR::UL) {// unsigned long
	  return element2intvalueHelper<gdcm::VR::UL>(ov, elem, chatty);
	} else if (vr & gdcm::VR::SL) {// signed long
	  return element2intvalueHelper<gdcm::VR::SL>(ov, elem, chatty);
	} else if (vr & gdcm::VR::US) {// unsigned short
	  return element2intvalueHelper<gdcm::VR::US>(ov, elem, chatty);
	} else if (vr & gdcm::VR::SS) {// signed short
	  return element2intvalueHelper<gdcm::VR::SS>(ov, elem, chatty);
	} else if (vr & gdcm::VR::OB) {// other byte
		if (tag==gdcm::Tag(0x7FE0,0x0010)) { // PixelData
			if(chatty) octave_stdout  << "skipping, leave for dicomread\n";
			return DICOM_NOTHING_ASSIGNED;
		}
		const uint32_t len=elem->GetByteValue()->GetLength();
		intNDArray<octave_uint8> bytearr(dim_vector(1,len));
		const uint8_t *p=(uint8_t *)elem->GetByteValue()->GetPointer();
                for (uint32_t i=0; i<len; i++)
                  {
                     bytearr(i) = p[i];
                  }
		*ov=bytearr;
		if (chatty) {
			uint32_t i;
			char buf[8];
			octave_stdout  << '[' ;
			for (i=0; i<len; i++) {
				snprintf(buf,7,"%02X ",(const uint8_t)p[i]);
				octave_stdout  << buf << " " ;
			}
			octave_stdout  << "]\n";
		}
	} else {
		if(chatty) octave_stdout << "   ### VR not handled ###" << std::endl;
		return DICOM_NOTHING_ASSIGNED;
	}
	//free(keyword);
	return DICOM_OK;
}

void dumpSequence(octave_value *ov, gdcm::SequenceOfItems *seq, int chatty, int sequenceDepth) {
	octave_map om;
	octave_idx_type item_no = 0;
	for (gdcm::SequenceOfItems::Iterator sit=seq->Begin(); sit != seq->End(); sit++) {
		gdcm::DataSet ds=sit->GetNestedDataSet();
		octave_map subom;
		std::ostringstream item_name_buf;
		gdcm::Item mi = *sit;
		item_no ++;
		if (chatty) octave_stdout << "object " << item_no << " " << mi.GetTag() << std::endl;
		for (gdcm::DataSet::Iterator dit=ds.Begin(); dit != ds.End(); dit++) {
			std::string key("");
			octave_value subov;
			if( DICOM_OK==element2value(key, &subov, &(*dit), chatty, sequenceDepth)) {
				subom.assign(key.c_str(), subov);
			} else {
				if (0==key.length()) continue ;
				subom.assign(key.c_str(), octave_value("not assigned"));
			}
			if (chatty) octave_stdout << "object sub map " << subom.nfields() << std::endl;
		}
		item_name_buf << "Item_" << item_no;
		om.assign(item_name_buf.str().c_str(), octave_value(subom));
	}
	*ov=om;
}

void getFileModTime(char *timeStr, const char *filename) {
	struct tm* clock;				// create a time structure
	struct stat attrib;			// create a file attribute structure
	stat(filename, &attrib);		// get the attributes of afile.txt
	clock = gmtime(&(attrib.st_mtime));	// Get the last modified time and put it into the time structure
	char monthStr[4];
	switch(clock->tm_mon) {
		case  0: strcpy(monthStr,"Jan"); break;
		case  1: strcpy(monthStr,"Feb"); break;
		case  2: strcpy(monthStr,"Mar"); break;
		case  3: strcpy(monthStr,"Apr"); break;
		case  4: strcpy(monthStr,"May"); break;
		case  5: strcpy(monthStr,"Jun"); break;
		case  6: strcpy(monthStr,"Jul"); break;
		case  7: strcpy(monthStr,"Aug"); break;
		case  8: strcpy(monthStr,"Sep"); break;
		case  9: strcpy(monthStr,"Oct"); break;
		case 10: strcpy(monthStr,"Nov"); break;
		case 11: strcpy(monthStr,"Dec"); break;
	}
	snprintf(timeStr, TIME_STR_LEN, "%02i-%s-%i %02i:%02i:%02i",
		clock->tm_mday,monthStr,1900+clock->tm_year, 
		clock->tm_hour, clock->tm_min, clock->tm_sec);
	//clock->tm_year returns the year (since 1900)
	// clock->tm_mon returns the month (January = 0)
	// clock->tm_mday returns the day of the month
	// 18-Dec-2000 11:06:43
}

Matrix str2DoubleVec(const char* s){
	// count separators, hence elements
	int n=1;
	char *sl=(char *)s; 
	for (; *sl != '\0'; sl++) n = (*sl == '\\') ? n+1 : n;
	// create output args
	Matrix dv(n, 1);
	double *fv=dv.fortran_vec();
	// parse into output
	int i=0;
	for (sl=(char *)s; i<n ; i++, sl++) {
		fv[i]=strtod(sl,&sl);
	}
	return dv;
}


// this fn will malloc new space if the length of the supplied destination
// string is not sufficient. so, if the returned pointer is not to the same
// place as the supplied, the returned pointer should be freed.
// returned pointer, as the supplied one may be invalid.
// d_len: length of d.
char* byteval2string(char * d, int d_len, const gdcm::ByteValue *bv) {
	if(bv==NULL) { // make a null string, "" 
		*d='\0';
		return d;
	}
	int len = bv->GetLength();
	if ( len > d_len ) {
		d=(char *)malloc((len+1)*sizeof(char));
	}
	memcpy(d, bv->GetPointer(), len);
	d[len]='\0';
	return d;
}

// remove non-alphabet characters from a string.
// remove s following quote
// the destination string, d, must be malloc'd space.
// this fn will realloc if it is not big enough. so use
// returned pointer, as the supplied one may be invalid.
// d_len_p: pointer to length of d. is updated if required.
char* name2Keyword(char *d, int *d_len_p, const char* s) {
	char *f=(char*)s; //from (loop through source)
	int len=strlen(s);
	if ( len > *d_len_p ) {
		d=(char *)realloc(d,(len+1)*sizeof(char));
	}
	char *tl=(char*)d; // pointer to loop through the destination
	for (; *f != '\0' ; f++ ) {
		if ( (*f >= 'A' && *f <= 'Z') || (*f >= 'a' && *f <= 'z') ) {
			*tl++ = *f;
		} else if (*f=='\'' && *(f+1)=='s') {
			f++; // if quote followed by s, skip both chars
		} else if (*f==' ' && *(f+1) >= 'a' && *(f+1) <= 'z') {
			*tl++ = *++f - ('a'-'A') ; // space folowed by lower case char, cap char
		}
	}
	*tl = '\0';
	return d;
}

/*
%!shared testfile
%! testfile = urlwrite ( ...
%!   'http://sourceforge.net/p/octave/code/11601/tree/trunk/octave-forge/extra/dicom/dcm_examples/RD.15MV.DCM?format=raw', ...
%!   tempname() );

%!fail("dicominfo")

%!fail("dicominfo(1)")

%!test
%! s=dicominfo(testfile);
%! assert(s.PatientName,"PHANTOM^IsodoseComparison^^^");

%!test
%! s=dicominfo(testfile);
%! assert(s.ROIContourSequence.Item_1.ContourSequence.Item_1.ContourGeometricType,"POINT ");

%!test
%! if exist (testfile, 'file')
%!   delete (testfile);
%! endif
*/
