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

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include "octave/oct.h"

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

#define OCT_FN_NAME _gendicomdict
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

char* name2Keyword(char *d, int *d_len_p, const char* s);

// This takes around 70 min to run on my laptop
// This is a very dirty way to make a dictionary
// It is transparently independent of Matlab
// TODO: needs to make tags with xx in that give ranges
DEFUN_DLD (OCT_FN_NAME, args, nargout,
		"extract data from gdcm libs to make a dict for octave") {
	octave_value_list retval;  // create object to store return values
	if (args.length () != 0) {
		error(QUOTED(OCT_FN_NAME)": no arguments required, got %i.", (int)args.length ());
		return retval; 
	}
	
	// get dicom dictionary
	const gdcm::Global &g = gdcm::Global::GetInstance();
	const gdcm::Dicts &dicts = g.GetDicts();
	const char *strowner = 0;
	
	uint16_t gi, ei;
	
	int buflen=32;
	char * keybuf=(char *)malloc(buflen*sizeof(char));
	
	// TODO: use Keywords instead of names
	// TODO: option to write to file instead of terminal
	std::ofstream dic;
	dic.open("octavedicom.dic"); //TODO: check for IO problems
	dic << std::resetiosflags(std::ios_base::showbase);
	dic << std::setiosflags(std::ios::uppercase) ;
	dic << std::setbase(16) << std::setfill('0');
	octave_stdout.precision(2);
	for(gi=0x0; gi<0xFFFF; gi++) {
		if(0== gi%64) {
			octave_stdout << ((double)gi)/((double)0xFFFF) << "  " ; //progress
		}
		for(ei=0x1; ei<0xFFFF; ei++) { // ei starts at 1. 0 is group length tags
			const gdcm::Tag tag(gi, ei);
			if (tag.IsIllegal()) continue;
			const gdcm::DictEntry dictEntry = dicts.GetDictEntry(tag,strowner) ;
			if(gdcm::VR::INVALID==dictEntry.GetVR()) continue ;
			if(!strcmp("Private Creator",dictEntry.GetName())) continue ; //TODO: for these dicominfo will do something...
			// gdcm::DictEntry::GetKeyword() seems to always return ""
			if (strlen(dictEntry.GetName()) == 0) continue;
			std::stringstream ss;
			ss << dictEntry.GetVR()  ;
			if (' '==ss.str()[2]) { // change "OB or OW" to "OB/OW"
				ss.str(ss.str().substr(0,2)+'/'+ss.str().substr(6,2));
			}
			const char *tagName=dictEntry.GetName();
			keybuf=name2Keyword(keybuf,&buflen,tagName);
			dic << '(' << std::setw(4) << gi ;
			dic << std::setw(1) << "," ;
			dic << std::setw(4) << ei;
			dic << std::setw(1) << ")\t" ;
			dic << ss.str() << '\t' ;
			dic << keybuf << '\t' ;
			dic << dictEntry.GetVM() << '\n' ;
		}
	}
	dic.close();
	free(keybuf);
	octave_stdout << '\n' ;
	return retval;
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
