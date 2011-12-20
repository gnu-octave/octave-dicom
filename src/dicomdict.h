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
void load_dicom_dict(const char *filename);
const char * const get_current_dicom_dict();

void lookup_dicom_keyword(std::string & keyword, const gdcm::Tag & tag);
void lookup_dicom_tag(gdcm::Tag & tag, const std::string & keyword);
void lookup_dicom_entry(gdcm::DictEntry & entry, const gdcm::Tag & tag);
bool dicom_is_present(const std::string & keyword);
bool dicom_is_present(const gdcm::Tag & tag);

/** DICOM value representions that make sense going straight to strings.
  * contrast with some VRASCII types that hold numbers.
  * may take some dates and times out of this and handle differently */
#define VRSTRING (gdcm::VR::AE|gdcm::VR::AS|gdcm::VR::CS|gdcm::VR::DA\
	|gdcm::VR::DT|gdcm::VR::LO|gdcm::VR::LT|gdcm::VR::PN|gdcm::VR::SH\
	|gdcm::VR::ST|gdcm::VR::TM|gdcm::VR::UI|gdcm::VR::UT)

