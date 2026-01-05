/*
 * Copyright John Donoghue, 2026
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

#define OCT_FN_NAME __dicom_stat__
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

static char* byteval2string (char * d, int d_len_p, const gdcm::ByteValue *bv);
static char* name2Keyword (char *d, int *d_len_p, const char* s);
static Matrix str2DoubleVec (const char*);
static void getFileModTime (char *timeStr, const char *filename);
static int element2value (std::string & varname, octave_value *ov, const gdcm::DataElement * elem, int chatty, int sequenceDepth) ;

static octave_map dicom_stat (const char filename[]);
static int dicom_truncate_numchar=40;

DEFUN_DLD (__dicom_stat__, args, nargout,
    "Private function")
{
  octave_value_list retval;  // create object to store return values
  if ( (1 != args.length ()) || (! args (0).is_string ()))
    {
      error(QUOTED(OCT_FN_NAME)": one arg required: dicom filename");
      return retval; 
    }

  std::string filename = args (0).string_value ();

  const std::string current_dict = get_current_dicom_dict ();
  load_dicom_dict (current_dict.c_str ()); // reset dictionary to initial value

  octave_map om = dicom_stat (filename.c_str ());
  retval(0) = om;

  return retval;
}

octave_map dicom_stat(const char filename[])
{
  // output struct
  octave_map om;
  // Instantiate the reader:
  gdcm::Reader reader;
  reader.SetFileName (filename);
  if (!reader.Read ())
    {
      error("Could not read: %s",filename);
      //TODO: set error state somehow so the main DEFUN_DLD function knows
      // KT this doesn't seem to be necessary. Presumably error() sets a flag that tells the interpreter it should abort
      return om; 
    }
  gdcm::File &file = reader.GetFile ();
  gdcm::DataSet &ds = file.GetDataSet ();
  gdcm::FileMetaInformation &hds = file.GetHeader ();
  
  om.assign ("Filenames", octave_value (filename));
  //char dateStr[TIME_STR_LEN+1];
  //getFileModTime (dateStr, filename);
  //om.assign ("FileModDate", octave_value (dateStr));

  om.assign ("StudyDateTime", octave_value(double(0)));
  om.assign ("SeriesDateTime", octave_value(double(0)));
  om.assign ("PatientName", octave_value(""));
  om.assign ("PatientSex", octave_value(""));
  om.assign ("Modality", octave_value(""));
  om.assign ("Rows", octave_value(0));
  om.assign ("Columns", octave_value(0));
  om.assign ("Channels", octave_value(0));
  om.assign ("Frames", octave_value(0));
  om.assign ("StudyDescription", octave_value(""));
  om.assign ("SeriesDescription", octave_value(""));
  om.assign ("StudyInstanceUID", octave_value(""));
  om.assign ("SeriesInstanceUID", octave_value(""));

  octave_value ov;
  std::string varname;

  const gdcm::DataElement row_elem = ds.GetDataElement(gdcm::Tag(0x0028,0x0010));
  if (element2value(varname, &ov, &row_elem, 0, 0) == DICOM_OK)
    om.assign ("Rows", ov);

  const gdcm::DataElement col_elem = ds.GetDataElement(gdcm::Tag(0x0028,0x0011));
  if (element2value(varname, &ov, &col_elem, 0, 0) == DICOM_OK)
    om.assign ("Columns", ov);

  const gdcm::DataElement sex_elem = ds.GetDataElement(gdcm::Tag(0x0010,0x0040));
  if (element2value(varname, &ov, &sex_elem, 0, 0) == DICOM_OK)
    om.assign ("PatientSex", ov);

  const gdcm::DataElement name_elem = ds.GetDataElement(gdcm::Tag(0x0010,0x0010));
  if (element2value(varname, &ov, &name_elem, 0, 0) == DICOM_OK)
    om.assign ("PatientName", ov);

  const gdcm::DataElement mod_elem = ds.GetDataElement(gdcm::Tag(0x0008,0x0060));
  if (element2value(varname, &ov, &mod_elem, 0, 0) == DICOM_OK)
    om.assign ("Modality", ov);
  
  const gdcm::DataElement frames_elem = ds.GetDataElement(gdcm::Tag(0x0028,0x0008));
  if (element2value(varname, &ov, &frames_elem, 0, 0) == DICOM_OK)
    om.assign ("Frames", ov);

  const gdcm::DataElement samples_elem = ds.GetDataElement(gdcm::Tag(0x0028,0x0002));
  if (element2value(varname, &ov, &samples_elem, 0, 0) == DICOM_OK)
    om.assign ("Channels", ov);

  const gdcm::DataElement study_elem = ds.GetDataElement(gdcm::Tag(0x0008,0x1030));
  if (element2value(varname, &ov, &study_elem, 0, 0) == DICOM_OK)
    om.assign ("StudyDescription", ov);

  const gdcm::DataElement series_elem = ds.GetDataElement(gdcm::Tag(0x0008,0x103E));
  if (element2value(varname, &ov, &series_elem, 0, 0) == DICOM_OK)
    om.assign ("SeriesDescription", ov);

  const gdcm::DataElement study_inst_elem = ds.GetDataElement(gdcm::Tag(0x0020,0x000D));
  if (element2value(varname, &ov, &study_inst_elem, 0, 0) == DICOM_OK)
    om.assign ("StudyInstanceUID", ov);

  const gdcm::DataElement series_inst_elem = ds.GetDataElement(gdcm::Tag(0x0020,0x000E));
  if (element2value(varname, &ov, &series_inst_elem, 0, 0) == DICOM_OK)
    om.assign ("SeriesInstanceUID", ov);


  std::string date_time = "";
  const gdcm::DataElement study_date_elem = ds.GetDataElement(gdcm::Tag(0x0008,0x0020));
  if (element2value(varname, &ov, &study_date_elem, 0, 0) == DICOM_OK)
    date_time = date_time + ov.string_value();
  const gdcm::DataElement study_time_elem = ds.GetDataElement(gdcm::Tag(0x0008,0x0030));
  if (element2value(varname, &ov, &study_time_elem, 0, 0) == DICOM_OK)
    date_time = date_time + ov.string_value();

  om.assign ("StudyDateTime", octave_value(date_time));

  date_time = "";
  const gdcm::DataElement series_date_elem = ds.GetDataElement(gdcm::Tag(0x0008,0x0021));
  if (element2value(varname, &ov, &series_date_elem, 0, 0) == DICOM_OK)
    date_time = date_time + ov.string_value();
  const gdcm::DataElement series_time_elem = ds.GetDataElement(gdcm::Tag(0x0008,0x0031));
  if (element2value(varname, &ov, &series_time_elem, 0, 0) == DICOM_OK)
    date_time = date_time + ov.string_value();

  om.assign ("SeriesDateTime", octave_value(date_time));

  return om;
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
          const int chatty)
{
  if (elem->IsEmpty())
    return DICOM_NOTHING_ASSIGNED;

  // save (but slow?) implementation that uses GDCM functions
  gdcm::Element<vrtype,gdcm::VM::VM1_n> el;
  el.Set( elem->GetValue() );
  // possible optimisation here. If there's only 1 element, maybe we can
  // save time by not making array. However, because all octave values are
  // arrays, maybe this doesn't matter. the "else" statement below works
  // always in any case.
  // If you want to try this optimisation, put #if 1 below
  {
    octaveArrayType val(dim_vector (el.GetLength(),1));
    for (unsigned i = 0; i < el.GetLength(); ++i)
    val(i)  = el.GetValue(i); 
    *ov=val;
    if (chatty)
      octave_stdout << '[' << val << "]\n";
  }
  return DICOM_OK;
}

/* Helper functions for elemement2value for integer and real types.
   They simply call element2simplevalueHelper2 with the appropriate template arguments.
*/
template <gdcm::VR::VRType vrtype>
static inline 
int element2intvalueHelper(octave_value *ov, const gdcm::DataElement * elem,
         const int chatty)
{
  typedef typename gdcm::VRToType<vrtype >::Type valueType;
  return element2simplevalueHelper2<vrtype,valueType,intNDArray<octave_int<valueType> > >(ov, elem, chatty);
}


template <gdcm::VR::VRType vrtype>
static inline 
int element2realvalueHelper(octave_value *ov, const gdcm::DataElement * elem,
         const int chatty)
{
  typedef typename gdcm::VRToType<vrtype >::Type valueType;
  return element2simplevalueHelper2<vrtype,valueType,Array<valueType> >(ov, elem, chatty);
}

int element2value(std::string & varname, octave_value *ov, const gdcm::DataElement * elem, 
        int chatty, int sequenceDepth)
{
  // get dicom dictionary
  //static const gdcm::Global& g = gdcm::Global::GetInstance();
  //static const gdcm::Dicts &dicts = g.GetDicts();
  const gdcm::Tag tag = elem->GetTag ();   
  gdcm::VR vr = elem->GetVR (); // value representation

  // skip "Group Length" tags. note: these are deprecated in DICOM 2008
  if(tag.GetElement() == (uint16_t)0) 
    return DICOM_NOTHING_ASSIGNED;
  //const gdcm::DictEntry dictEntry = dicts.GetDictEntry(tag,(const char*)0);

  // find dictionary entry for name and to possibly adjust the VR
  gdcm::DictEntry dictEntry ;
  if (!dicom_is_present(tag))
    {
      char fallbackVarname[64];
      snprintf (fallbackVarname, 63, "Private_%04x_%04x", tag.GetGroup(), tag.GetElement());
      varname = std::string (fallbackVarname);
    }
  else
    {
      lookup_dicom_entry (dictEntry, tag);
      varname = dictEntry.GetName ();
      const gdcm::VR dictvr = dictEntry.GetVR(); // value representation. ie DICOM   
      if (!vr.Compatible(vr))
        {
          warning (QUOTED(OCT_FN_NAME)": %s has different VR from dictionary. Using VR from file.", varname.c_str());
        }
      // find vr from dictionary if it was not in the file
      // lines copied from gdcmPrinter::PrintDataElement
      if (vr == gdcm::VR::INVALID)
        {
          vr = dictvr;
        }
      else if (vr == gdcm::VR::UN && vr != gdcm::VR::INVALID)
        {
          // File is explicit, but still prefer vr from dict when UN
          vr = dictvr;
        }
      else // cool the file is Explicit !
        {
          // keep vr from file
        }
    }

  if(chatty)
    {
      octave_stdout << std::setw(2*sequenceDepth) << "" << std::setw(0) ;
      octave_stdout << tag << ":" << vr << ":" << varname << ":" ;
      // TODO: error if var name repeated. 
    }
#define strValBufLen 511
  char strValBuf[strValBufLen+1];
  char* strVal=strValBuf;
  
  if ( vr & gdcm::VR::VRASCII)
    {
      strVal = byteval2string (strValBuf, strValBufLen, elem->GetByteValue ()); 
      if(chatty)
        {
          if (dicom_truncate_numchar > 0)
            {
              octave_stdout << '[' << std::string(strVal).substr(0,dicom_truncate_numchar) 
                << ( ((int)strlen(strVal)>dicom_truncate_numchar) ? "..." : "") << ']' << std::endl;
            }
          else
            {
              octave_stdout << '[' << strVal << ']' << std::endl;
            }
        }
      if (vr & VRSTRING)
        {
          //all straight to string types
          *ov = std::string(strVal); // TODO: error if om already has member with this name
        }
      else if (vr & gdcm::VR::IS)
        {
          // Integer String. spec tallies with signed 32 bit int
          *ov = (int32_t)atoi(strVal); 
        }
      else if (vr & gdcm::VR::DS)
        {
          // Double String. vector separated by '/'
          Matrix vec=str2DoubleVec(strVal);
          *ov=vec;
        }
      else
        {
          if(chatty)
            octave_stdout << "   ### string type not handled ###" << std::endl;
          return DICOM_NOTHING_ASSIGNED;
        }
      if (strVal != strValBuf)
        free(strVal); // long string. malloc'd instead of using buf, now needs free'ng
    }
  else if (vr & gdcm::VR::SQ)
    {
      if(chatty)
        octave_stdout << " reading sequence.\n";

      gdcm::SmartPointer<gdcm::SequenceOfItems> sqi = elem->GetValueAsSQ ();

      // ignoring sequences here for stat function
      *ov = octave_map ();
    }
  else if (vr & gdcm::VR::AT)
    {
      // attribute tag
      if (!elem->GetByteValue())
        {
          if(chatty)
            octave_stdout  << "NULL\n";
          return DICOM_NOTHING_ASSIGNED;
	}

      intNDArray<octave_uint16> uint16pair(dim_vector(1,2));
      uint16_t *p = (uint16_t *)elem->GetByteValue()->GetPointer();
      uint16pair(0) = p[0];
      uint16pair(1) = p[1];
      *ov=uint16pair;
      if (chatty)
        {
          char buf[16];
          snprintf (buf, 15, "[(%04X,%04X)]\n", p[0], p[1]);
          octave_stdout << buf;
         }
    }
  else if (vr & gdcm::VR::FL)
    {
      // float
      return element2realvalueHelper<gdcm::VR::FL>(ov, elem, chatty);
    }
  else if (vr & gdcm::VR::FD)
    {
      // double
      return element2realvalueHelper<gdcm::VR::FD>(ov, elem, chatty);
    }
  else if (vr & gdcm::VR::UL)
    {
      // unsigned long
      return element2intvalueHelper<gdcm::VR::UL>(ov, elem, chatty);
    }
  else if (vr & gdcm::VR::SL)
    {
      // signed long
      return element2intvalueHelper<gdcm::VR::SL>(ov, elem, chatty);
    }
  else if (vr & gdcm::VR::US)
    {
      // unsigned short
      return element2intvalueHelper<gdcm::VR::US>(ov, elem, chatty);
    }
  else if (vr & gdcm::VR::SS)
    {
      // signed short
      return element2intvalueHelper<gdcm::VR::SS>(ov, elem, chatty);
    }
  else if (vr & gdcm::VR::OB)
    {
      // other byte
      if (tag == gdcm::Tag(0x7FE0,0x0010))
        { // PixelData
          if(chatty)
            octave_stdout  << "skipping, leave for dicomread\n";
          return DICOM_NOTHING_ASSIGNED;
        }
      if (!elem->GetByteValue())
        {
          if(chatty)
            octave_stdout  << "NULL\n";
          return DICOM_NOTHING_ASSIGNED;
	}
      const uint32_t len = elem->GetByteValue()->GetLength();
      intNDArray<octave_uint8> bytearr(dim_vector(1,len));
      const uint8_t *p = (uint8_t *)elem->GetByteValue()->GetPointer();
      for (uint32_t i = 0; i < len; i++)
        {
          bytearr(i) = p[i];
        }
      *ov = bytearr;
      if (chatty)
        {
          uint32_t i;
          char buf[8];
          octave_stdout  << '[';
          for (i = 0; i < len; i++)
            {
              snprintf (buf, 7, "%02X ", (const uint8_t)p[i]);
              octave_stdout  << buf << " " ;
            }
          octave_stdout  << "]\n";
        }
    }
  else
    {
      if(chatty)
        octave_stdout << "   ### VR not handled ###" << std::endl;
      return DICOM_NOTHING_ASSIGNED;
    }
  //free(keyword);
  return DICOM_OK;
}

void getFileModTime (char *timeStr, const char *filename)
{
  struct tm* clock;       // create a time structure
  struct stat attrib;     // create a file attribute structure
  stat (filename, &attrib);    // get the attributes of afile.txt
  clock = gmtime (&(attrib.st_mtime)); // Get the last modified time and put it into the time structure
  char monthStr[4];
  switch(clock->tm_mon)
    {
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
  snprintf (timeStr, TIME_STR_LEN, "%02i-%s-%i %02i:%02i:%02i",
    clock->tm_mday,monthStr,1900+clock->tm_year, 
    clock->tm_hour, clock->tm_min, clock->tm_sec);
  //clock->tm_year returns the year (since 1900)
  // clock->tm_mon returns the month (January = 0)
  // clock->tm_mday returns the day of the month
  // 18-Dec-2000 11:06:43
}

Matrix str2DoubleVec (const char* s)
{
  // count separators, hence elements
  int n=1;
  char *sl=(char *)s; 
  for (; *sl != '\0'; sl++)
    n = (*sl == '\\') ? n+1 : n;
  // create output args
  Matrix dv(n, 1);
  double *fv=dv.fortran_vec();
  // parse into output
  int i=0;
  for (sl=(char *)s; i<n ; i++, sl++)
    {
      fv[i] = strtod (sl,&sl);
    }
  return dv;
}


// this fn will malloc new space if the length of the supplied destination
// string is not sufficient. so, if the returned pointer is not to the same
// place as the supplied, the returned pointer should be freed.
// returned pointer, as the supplied one may be invalid.
// d_len: length of d.
char* byteval2string (char * d, int d_len, const gdcm::ByteValue *bv)
{
  if (bv == NULL)
    { 
      // make a null string, "" 
      *d='\0';
      return d;
    }
  int len = bv->GetLength ();
  if (len > d_len)
    {
      d=(char *)malloc ((len+1)*sizeof(char));
    }
  memcpy (d, bv->GetPointer(), len);
  d[len]='\0';
  return d;
}

// remove non-alphabet characters from a string.
// remove s following quote
// the destination string, d, must be malloc'd space.
// this fn will realloc if it is not big enough. so use
// returned pointer, as the supplied one may be invalid.
// d_len_p: pointer to length of d. is updated if required.
char* name2Keyword (char *d, int *d_len_p, const char* s)
{
  char *f=(char*)s; //from (loop through source)
  int len=strlen(s);
  if (len > *d_len_p)
    {
      d = (char *)realloc (d,(len+1)*sizeof(char));
    }
  char *tl=(char*)d; // pointer to loop through the destination
  for (; *f != '\0' ; f++ )
    {
      if ( (*f >= 'A' && *f <= 'Z') || (*f >= 'a' && *f <= 'z') )
        {
          *tl++ = *f;
        }
      else if (*f == '\'' && *(f+1)=='s')
        {
          f++; // if quote followed by s, skip both chars
        }
      else if (*f == ' ' && *(f+1) >= 'a' && *(f+1) <= 'z')
        {
          *tl++ = *++f - ('a'-'A') ; // space folowed by lower case char, cap char
        }
     }
  *tl = '\0';
  return d;
}

/*
%!shared testfile
%! testfile = file_in_loadpath("imdata/simpleImageWithIcon.dcm");

%!fail("dicominfo")

%!fail("dicominfo(1)")

%!test
%! s=dicominfo(testfile);
%! assert(s.PatientName,"GDCM^Patient");

%!test
%! s=dicominfo(testfile);
%! assert(s.IconImageSequence.Item_1.PhotometricInterpretation,"MONOCHROME2 ");
*/
