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
 
#include <stdlib.h> //for calloc, free
#include <stdio.h>  //for printf

#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
#include <string>
#include <map>

#include "octave/oct.h"
#include "octave/ov-struct.h"

#include "gdcmSystem.h"
#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmAttribute.h"
#include "gdcmDataSet.h"
#include "gdcmGlobal.h"
#include "gdcmDicts.h"
#include "gdcmDict.h"
#include "gdcmCSAHeader.h"
#include "gdcmPrivateTag.h"
#include "gdcmVR.h"
#include "gdcmSequenceOfItems.h"
 
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
Octave_map dump(const char filename[], int chatty);
void dumpDataSet(Octave_map *om, const gdcm::DataSet *ds, int chatty, int sequenceDepth);
void getFileModTime(char *timeStr, const char *filename);
void dumpElement(Octave_map *om, const gdcm::DataElement * elem, int chatty, int sequenceDepth);
void dumpSequence(octave_value *ov, gdcm::SequenceOfItems *seq, int chatty, int sequenceDepth);
int element2value(std::string & varname, octave_value *ov, const gdcm::DataElement * elem, int chatty, int sequenceDepth) ;

int dicom_truncate_numchar=40;

#ifdef NOT_OCT
int main( int argc, const char* argv[] ) {
	dump(argv[1], 1 /* chatty on */ ); // 1 cmd line arg: dicom filename
	return 0;
}
#else
DEFUN_DLD (OCT_FN_NAME, args, nargout,
		"return some info from a dicom file: 1 arg: dicom filename") {
	octave_value_list retval;  // create object to store return values
	if ( 0 == args.length()) {
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
	
	Octave_map om=dump(filename.c_str(),chatty);
	retval(0)=om;
	return retval;
}
#endif


Octave_map dump(const char filename[], int chatty) {
	// output struct
	Octave_map om;
	// Instantiate the reader:
	gdcm::Reader reader;
	reader.SetFileName( filename );
	if( !reader.Read() ) {
		error("Could not read: %s",filename);
		return om; //TODO: set error state somehow so the main DEFUN_DLD function knows
	}
	gdcm::File &file = reader.GetFile();
	gdcm::DataSet &ds = file.GetDataSet();
	gdcm::FileMetaInformation &hds=file.GetHeader();
	
	om.assign("Filename",filename);
	char dateStr[TIME_STR_LEN+1];
	getFileModTime(dateStr, filename);
	om.assign("FileModDate", dateStr);
	if(chatty) octave_stdout << "# file info\nFilename:" 
		<< filename << "\nFileModDate:" << dateStr << '\n';
	
	if(chatty) octave_stdout << "# header\n" ;
	dumpDataSet(&om, &hds, chatty, 0);
	if(chatty) octave_stdout << "# metadata\n" ;
	dumpDataSet(&om, &ds, chatty, 0);
	
	return om;
}

void dumpDataSet(Octave_map *om, const gdcm::DataSet *ds, int chatty, int sequenceDepth) {
	
	const gdcm::DataSet::DataElementSet DES=ds->GetDES(); // gdcm::DataSet::DataElementSet is a std::set
	gdcm::DataSet::Iterator it;
	
	for ( it=DES.begin() ; it != DES.end(); it++ ) {		
		dumpElement(om, &(*it), chatty, sequenceDepth);
	}
}

void dumpElement(Octave_map *om, const gdcm::DataElement * elem, 
				int chatty, int sequenceDepth) {
	std::string varname;
	octave_value ov;
	if(DICOM_OK==element2value(varname, &ov, elem, chatty, sequenceDepth)) {
		om->assign(varname.c_str(), ov);
	} else {
		if (0==varname.length()) return ;
		om->assign(varname.c_str(), "not assigned");
	}
}

int element2value(std::string & varname, octave_value *ov, const gdcm::DataElement * elem, 
				int chatty, int sequenceDepth) {
	// get dicom dictionary
	//static const gdcm::Global& g = gdcm::Global::GetInstance();
	//static const gdcm::Dicts &dicts = g.GetDicts();
	
	const gdcm::Tag tag = elem->GetTag();
		
	// skip "Group Length" tags. note: these are deprecated in DICOM 2008
	if(tag.GetElement() == (uint16_t)0 || elem->GetByteValue() == NULL) return DICOM_NOTHING_ASSIGNED;
	//const gdcm::DictEntry dictEntry = dicts.GetDictEntry(tag,(const char*)0);
	gdcm::DictEntry dictEntry ;
	if (!is_present(tag)) {
		char fallbackVarname[64];
		snprintf(fallbackVarname,63,"Private_%04X_%04X",tag.GetGroup(),tag.GetElement());
		varname=std::string(fallbackVarname);
		*ov=std::string("");
		warning(QUOTED(OCT_FN_NAME)": %s", fallbackVarname);
		return DICOM_NOTHING_ASSIGNED; //TODO maybe could carry on, if we know the VR
	}
	lookup_entry(dictEntry, tag);
	const gdcm::VR vr= dictEntry.GetVR(); // value representation. ie DICOM type.
	varname=dictEntry.GetName();
	
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
	} else if (vr & gdcm::VR::UL) { 
		uint32_t ulval ; 
		memcpy(&ulval, elem->GetByteValue()->GetPointer(), 4);
		*ov=ulval;
		if(chatty) octave_stdout << '[' << ulval << ']' << std::endl;
	} else if (vr & gdcm::VR::SQ) {
		if(chatty) octave_stdout << " reading sequence. "; // \n provided in dumpSequence fn
		gdcm::SmartPointer<gdcm::SequenceOfItems> sqi = elem->GetValueAsSQ();
		dumpSequence(ov, sqi, chatty, sequenceDepth+1);
	} else if (vr & gdcm::VR::AT) { // attribute tag
		intNDArray<octave_uint16> uint16pair(dim_vector(1,2));
		octave_uint16 *fv=uint16pair.fortran_vec();
		uint16_t *p=(uint16_t *)elem->GetByteValue()->GetPointer();
		memcpy(fv,p,4);
		*ov=uint16pair;
		if (chatty) {
			char buf[16];
			snprintf(buf,15,"[(%04X,%04X)]\n",p[0],p[1]);
			octave_stdout << buf  ;
		}
	} else if (vr & gdcm::VR::US) {// unsigned short
		uint16_t usval ; 
		memcpy(&usval, elem->GetByteValue()->GetPointer(), 2);
		*ov=usval;
		if(chatty) octave_stdout << '[' << usval << "]\n";
	} else if (vr & gdcm::VR::OB) {// other byte
		if (tag==gdcm::Tag(0x7FE0,0x0010)) { // PixelData
			if(chatty) octave_stdout  << "skipping, leave for dicomread\n";
			return DICOM_NOTHING_ASSIGNED;
		}
		const uint32_t len=elem->GetByteValue()->GetLength();
		intNDArray<octave_uint8> bytearr(dim_vector(1,len));
		octave_uint8 *fv=bytearr.fortran_vec(); 
		const char *p=elem->GetByteValue()->GetPointer();
		memcpy(fv,p , len);
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
	const octave_idx_type nDataSet=seq->GetNumberOfItems(); // objects in sequence
	if (chatty) octave_stdout << nDataSet << " object" << ((nDataSet==1)?"":"s") << std::endl;
	char item_name_buf[16];
	Octave_map om;
	for (octave_idx_type j=1; j<=nDataSet; j++ ) {
		const gdcm::DataSet::DataElementSet des=seq->GetItem(j).GetNestedDataSet().GetDES() ;
		Octave_map subom;
		for (gdcm::DataSet::Iterator it=des.begin(); it != des.end(); it++) {
			std::string key("");
			octave_value subov;
			if( DICOM_OK==element2value(key, &subov, &(*it), chatty, sequenceDepth)) {
				subom.assign(key.c_str(), subov);
			} else {
				if (0==key.length()) continue ;
				subom.assign(key.c_str(), "not assigned");
			}
		}
		snprintf(item_name_buf,15,"Item_%i",j);
		om.assign(item_name_buf, subom);
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
