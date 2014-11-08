/*
 * The GNU Octave dicom package is Copyright Andy Buckle 2010
 * Contact: blondandy using the sf.net system, 
 * <https://sourceforge.net/sendmessage.php?touser=1760416>
 *
 * Many thanks to Judd Storrs, who wrote most of the code in this
 * file. Anything ugly or wrong, I added later. Andy
 *
 * Minor changes Copyright Kris Thielemans 2011:
 *  make dicomdict('get') and dicomdict('set',filename) work properly and add doc-string
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
#include "octave/load-path.h"

#include "gdcmDict.h"
#include "gdcmVR.h"
#include "gdcmVM.h"

#include "dicomdict.h"

#define OCT_FN_NAME dicomdict
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

const char * factory_dicom_dict_filename="octavedicom.dic";
static std::string dic_filename(factory_dicom_dict_filename);

std::map<gdcm::Tag, std::string> tagmap ;
std::map<std::string, gdcm::Tag> keymap ;
std::map<std::string, gdcm::DictEntry> dict ;

void insert(const char *k, const gdcm::Tag t, const gdcm::DictEntry e) {
	keymap[k] = t ;
	tagmap[t] = k ;
	dict[k] = e ;
}


DEFUN_DLD (OCT_FN_NAME, args, nargout,
		"-*- texinfo -*- \n\
@deftypefn {Loadable Function} {@var{dictionary_name} =} "QUOTED(OCT_FN_NAME)" (@code{get}) \n\
@deftypefnx {Loadable Function} {} "QUOTED(OCT_FN_NAME)" (@code{factory}) \n\
@deftypefnx {Loadable Function} {} "QUOTED(OCT_FN_NAME)" (@code{set}, @var{dictionary_name}) \n\
The first usage returns the filename of the dictionary that is currently being used.\n\
Using @code{factory} resets the dictionary to the default.\n\
Using @code{set} allows setting the dictionary for future operations.\n\
In this case, the dictionary file @var{dictionary_name} can be anywhere in the path.\n\
\n\
@seealso{dicomread, dicomwrite}\n\
@end deftypefn \n") {
	octave_value_list retval;  // create object to store return values
	if (args.length()>2 || args.length()<1) {
		error(QUOTED(OCT_FN_NAME)": takes 1 or 2 arguments, got %i.",args.length ());
		return retval; 
	}
	charMatrix arg0mat = args(0).char_matrix_value ();
	if (arg0mat.rows()!=1) {
		error(QUOTED(OCT_FN_NAME)": first arg should be a single row string of chars.");
		return retval; 
	}
	std::string arg0str = arg0mat.row_as_string (0);
	if (args.length()==1) {
		if (arg0str == std::string("get")) { // TODO: consider making args not case-sensitive
			retval(0)=octave_value(dic_filename);
			return retval;
		} else if (arg0str == std::string("factory")) {
			dic_filename=std::string(factory_dicom_dict_filename);
// 			if (octave_dicom_dict == NULL) {
// 				octave_dicom_dict = new OctaveDicomDict(); //TODO where should this be freed?
// 				printf("init OctaveDicomDict before loading factory\n");
// 			}
			load_dicom_dict(dic_filename.c_str());
			
			return retval;
		} else {
			error(QUOTED(OCT_FN_NAME)": single arg must either be 'get' or 'factory'.",args.length ());
			return retval; 
		}
	}
	//must be 2 args
	charMatrix arg1mat = args(1).char_matrix_value ();
	if (arg1mat.rows()!=1) {
		error(QUOTED(OCT_FN_NAME)": second arg should be a single row string of chars.");
		return retval; 
	}
	std::string arg1str = arg1mat.row_as_string (0);
	if (arg0str != std::string("set")) {
		error(QUOTED(OCT_FN_NAME)": when 2 args are given, the first must be 'set'.",args.length ());
		return retval; 
	}
	load_dicom_dict(arg1str.c_str());
	//if (octave_dicom_dict == NULL) octave_dicom_dict = new OctaveDicomDict(); //TODO where should this be freed?
	//octave_dicom_dict.load_file(arg1str.c_str()); // second arg is filename
	return retval;
}

// Map from VR strings to gdcm Value Representations.
typedef std::map<std::string, gdcm::VR> vr_map ;
const vr_map::value_type vrData[] = {
  vr_map::value_type("AE", gdcm::VR::AE),
  vr_map::value_type("AS", gdcm::VR::AS),
  vr_map::value_type("AT", gdcm::VR::AT),
  vr_map::value_type("CS", gdcm::VR::CS),
  vr_map::value_type("DA", gdcm::VR::DA),
  vr_map::value_type("DS", gdcm::VR::DS),
  vr_map::value_type("DT", gdcm::VR::DT),
  vr_map::value_type("FD", gdcm::VR::FD),
  vr_map::value_type("FL", gdcm::VR::FL),
  vr_map::value_type("IS", gdcm::VR::IS),
  vr_map::value_type("LO", gdcm::VR::LO),
  vr_map::value_type("LT", gdcm::VR::LT),
  vr_map::value_type("OB", gdcm::VR::OB),
  vr_map::value_type("OF", gdcm::VR::OF),
  vr_map::value_type("OW", gdcm::VR::OW),
  vr_map::value_type("PN", gdcm::VR::PN),
  vr_map::value_type("SH", gdcm::VR::SH),
  vr_map::value_type("SL", gdcm::VR::SL),
  vr_map::value_type("SQ", gdcm::VR::SQ),
  vr_map::value_type("SS", gdcm::VR::SS),
  vr_map::value_type("ST", gdcm::VR::ST),
  vr_map::value_type("TM", gdcm::VR::TM),
  vr_map::value_type("UI", gdcm::VR::UI),
  vr_map::value_type("UL", gdcm::VR::UL),
  vr_map::value_type("UN", gdcm::VR::UN),
  vr_map::value_type("US", gdcm::VR::US),
  vr_map::value_type("UT", gdcm::VR::UT),
  vr_map::value_type("OB/OW", gdcm::VR::OB_OW),
  vr_map::value_type("OW/OB", gdcm::VR::OB_OW),
  vr_map::value_type("US/SS", gdcm::VR::US_SS),
  vr_map::value_type("SS/US", gdcm::VR::US_SS),
  vr_map::value_type("OW/SS/US", gdcm::VR::US_SS_OW),
  vr_map::value_type("OW/US/SS", gdcm::VR::US_SS_OW),
  vr_map::value_type("SS/OW/US", gdcm::VR::US_SS_OW),
  vr_map::value_type("SS/US/OW", gdcm::VR::US_SS_OW),
  vr_map::value_type("US/OW/SS", gdcm::VR::US_SS_OW),
  vr_map::value_type("US/SS/OW", gdcm::VR::US_SS_OW)
};
const int vrDataLength = sizeof vrData / sizeof vrData[0];
static vr_map vrMap(vrData, vrData+vrDataLength) ;


// Map from VM strings to gdcm Value Multipicities.
typedef std::map<std::string, gdcm::VM> vm_map ;
const vm_map::value_type vmData[] = {
  vm_map::value_type("0", gdcm::VM::VM0),
  vm_map::value_type("1", gdcm::VM::VM1),
  vm_map::value_type("2", gdcm::VM::VM2),
  vm_map::value_type("3", gdcm::VM::VM3),
  vm_map::value_type("4", gdcm::VM::VM4),
  vm_map::value_type("5", gdcm::VM::VM5),
  vm_map::value_type("6", gdcm::VM::VM6),
  vm_map::value_type("8", gdcm::VM::VM8),
  vm_map::value_type("9", gdcm::VM::VM9),
  vm_map::value_type("10", gdcm::VM::VM10),
  vm_map::value_type("12", gdcm::VM::VM12),
  vm_map::value_type("16", gdcm::VM::VM16),
  vm_map::value_type("18", gdcm::VM::VM18),
  vm_map::value_type("24", gdcm::VM::VM24),
  vm_map::value_type("28", gdcm::VM::VM28),
  vm_map::value_type("32", gdcm::VM::VM32),
  vm_map::value_type("35", gdcm::VM::VM35),
  vm_map::value_type("99", gdcm::VM::VM99),
  vm_map::value_type("256", gdcm::VM::VM256),
  vm_map::value_type("1-2", gdcm::VM::VM1_2),
  vm_map::value_type("1-3", gdcm::VM::VM1_3),
  vm_map::value_type("1-4", gdcm::VM::VM1_4),
  vm_map::value_type("1-5", gdcm::VM::VM1_5),
  vm_map::value_type("1-8", gdcm::VM::VM1_8),
  vm_map::value_type("1-32", gdcm::VM::VM1_32),
  vm_map::value_type("1-99", gdcm::VM::VM1_99),
  vm_map::value_type("1-n", gdcm::VM::VM1_n),
  vm_map::value_type("2-2n", gdcm::VM::VM2_2n),
  vm_map::value_type("2-n", gdcm::VM::VM2_n),
  vm_map::value_type("3-4", gdcm::VM::VM3_4),
  vm_map::value_type("3-3n", gdcm::VM::VM3_3n),
  vm_map::value_type("3-n", gdcm::VM::VM3_n),
  vm_map::value_type("4-4n", gdcm::VM::VM4_4n),
  vm_map::value_type("6-6n", gdcm::VM::VM6_6n),
  vm_map::value_type("7-7n", gdcm::VM::VM7_7n),
  vm_map::value_type("30-30n", gdcm::VM::VM30_30n),
  vm_map::value_type("47-47n", gdcm::VM::VM47_47n)
};
const int vmDataLength = sizeof vmData / sizeof vmData[0];
static vm_map vmMap(vmData, vmData+vmDataLength) ;


// A simple iterator for tag ranges
// (e.g. "12xx" goes from 0x1200 to 0x12ff)
class tag_range_iter {
private:
  uint16_t val ;
  uint16_t mask ;

public:
  tag_range_iter(const std::string tag) : val(0x0000), mask(0x0000)
  {
    char tmp[] = "...." ;
    for ( size_t i = 0 ; i<4 ; i++ )
      {
	mask<<=4;
	if ( tag[i] == 'x' || tag[i] == 'X' )
	  {
	    tmp[i] = '0' ;
	    mask |= 0x000f ;
	  }
	else
	  {
	    tmp[i] = tag[i] ;
	  }
      }
    unsigned int v ;
    sscanf(tmp, "%4x", &v) ;
    val = v ;
  }

  bool operator++()
  {
    if (val==(val|mask))
      {
	return false ;
      }
    val = (~mask&val)|((((mask&val)|(~mask))+1)&mask) ;
    return true ;
  }
  
  const uint16_t value() {
    return val ;
  }
};

const char * const get_current_dicom_dict() {
  return dic_filename.c_str();
}

void load_dicom_dict(const char * filename) {
	// reset, if required
	if (tagmap.size()>0) {
		tagmap.clear() ;
		keymap.clear() ;
		dict.clear() ;
	}

	// find dic if it is anywhere in the search path (same path as for m-files etc)
#ifndef NOT_OCT
	const std::string resolved_filename=load_path::find_file(std::string(filename)) ;
#else
        // for debugging: if not running in octave, find_file always returns ""
	// so we just use the original filename
	const std::string resolved_filename(filename);
#endif

	std::ifstream fin(resolved_filename.c_str());
	if (!fin) {
		error( "Failed to open dic" ) ;
		return ;
	}

	// Process each line
	size_t linenumber = 0 ;
	while (!fin.eof()) {
		std::string line ;
		getline(fin,line) ;
		linenumber++ ;

		// Skip any line that start with "#" without complaining
		if ( line[0] == '#' ) continue ;

		// Skip lines that don't start with "(xxxx,xxxx)"
		if ( (line.size() < 11)
			|| (line[0] != '(')
			|| (line[5] != ',')
			|| (line[10] != ')') )	{
			continue ;
		}

		char tgroup[4] ;
		char telem[4] ;
		char tvr[8] ;
		char key[128] ;
		char tvm[8] ;

		// Tokenize line
		if ( sscanf(line.c_str(), "(%4s,%4s) %8s %128s %8s", tgroup, telem, tvr, key, tvm ) != 5 ) {
			continue ;
		}

		// Convert VR
		gdcm::VR vr = vrMap[tvr] ;

		// Convert VM
		gdcm::VM vm = vmMap[tvm] ;

		// Warn if keyword cannot be used in octave
		if ( ! valid_identifier(key) ) {
			std::cerr << "WARNING: Invalid identifier '" << key << "'" << std::endl << std::flush ;
		}

		gdcm::DictEntry entry ;      
		entry.SetVR (vr) ;
		entry.SetVM (vm) ;
		entry.SetName (key) ;

		gdcm::Tag tag ;
		tag_range_iter group(tgroup) ;      
		do	{
			tag.SetGroup(group.value()) ;
			tag_range_iter elem(telem) ;
			do	{
				tag.SetElement(elem.value()) ;
				insert(key,tag,entry) ;
			} while ( ++elem ) ;
		} while ( ++group ) ;
	}
	// save filename
	dic_filename = resolved_filename;
}

void lookup_dicom_keyword(std::string & keyword, const gdcm::Tag & tag) {
	if (0==tagmap.size()) load_dicom_dict(factory_dicom_dict_filename); // init if necessary
	keyword = tagmap[tag];
}

void lookup_dicom_tag(gdcm::Tag & tag, const std::string & keyword) {
	if (0==tagmap.size()) load_dicom_dict(factory_dicom_dict_filename); // init if necessary
	tag = gdcm::Tag(keymap[keyword]);
}

void lookup_dicom_entry(gdcm::DictEntry & entry, const gdcm::Tag & tag) {
	if (0==tagmap.size()) load_dicom_dict(factory_dicom_dict_filename); // init if necessary
	entry = gdcm::DictEntry(dict[tagmap[tag]]);
}

bool dicom_is_present(const std::string & keyword){
	if (0==tagmap.size()) load_dicom_dict(factory_dicom_dict_filename); // init if necessary
	return keymap.count(keyword)>(std::vector<std::string>::size_type)0 ;
}

bool dicom_is_present(const gdcm::Tag & tag){
	if (0==tagmap.size()) load_dicom_dict(factory_dicom_dict_filename); // init if necessary
	return tagmap.count(tag)>(std::vector<gdcm::Tag>::size_type)0 ;
}

/*
%!test
%! addpath('../inst'); % so it can find the dictionary
%! dicomdict factory ;
%! [f n e]=fileparts(dicomdict('get'));
%! assert([n e],"octavedicom.dic");
%! % todo
*/
