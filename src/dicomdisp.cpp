/*
 * Copyright John Donoghue, 2019:
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

#define OCT_FN_NAME dicomdisp
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

#ifdef NOT_OCT
# define octave_stdout std::cout
# define error	printf
#endif

void dump(const char filename[]);
void dumpDataSet(const gdcm::DataSet *ds, int sequenceDepth, uint64_t &offset);
void dumpElement(const gdcm::DataElement * elem, int sequenceDepth,uint64_t &offset);
void dumpSequence(gdcm::SequenceOfItems *seq, int sequenceDepth, uint64_t &offset);

static int dicom_truncate_numchar=35;

#ifdef NOT_OCT
int main( int argc, const char* argv[] ) 
{
  dump(argv[1]); // 1 cmd line arg: dicom filename
  return 0;
}
#else
DEFUN_DLD (dicomdisp, args, nargout,
"-*- texinfo -*- \n\
 @deftypefn {Loadable Function} {} dicomdisp (@var{filename}) \n\
 @deftypefnx {Loadable Function} {} dicomdisp (@var{filename}, [@var{propertyname}, @var{propertvalue} ...]) \n\
 Read and display the metadata from a DICOM file.\n\
 \n\
 @var{filename} - dicomfilename to display.@*\n \
 @var{propertyname}, @var{propertvalue} - property pairs for options to the display function.\n \
 \n \
 Currently the only known property is 'dictionary' to specify a non default dict to use.\n \
 @seealso{dicomread, dicominfo} \n\
 @end deftypefn\n\
 ")
{
  octave_value_list retval;
  if ( (args.length() < 1) || (! args(0).is_string ()))
    {
      error(QUOTED(OCT_FN_NAME)": one arg required: dicom filename");
      return retval; 
    }
  charMatrix ch = args(0).char_matrix_value ();
  if (ch.rows()!=1)
    {
      error(QUOTED(OCT_FN_NAME)": arg should be a filename, 1 row of chars");
      return retval; 
    }

  std::string filename = ch.row_as_string (0);

  // save current dictionary for the case that we're using a different dictionary while reading
  const std::string current_dict = get_current_dicom_dict();

  // parse any additional args
  for (int i=1; i<args.length(); i++)
    {
      charMatrix chex = args(i).char_matrix_value();
      if (chex.rows()!=1)
        {
          error(QUOTED(OCT_FN_NAME)": arg should be a string, 1 row of chars");
          load_dicom_dict(current_dict.c_str()); // reset dictionary to initial value
          return retval; 
        }

      std::string argex = chex.row_as_string (0);
      if (!argex.compare("dictionary") || !argex.compare("Dictionary"))
        {
          if (i+1==args.length())
            {
              error(QUOTED(OCT_FN_NAME)": Dictionary needs another argument");
              load_dicom_dict(current_dict.c_str()); // reset dictionary to initial value
              return retval;
            }

          if (!args(i+1).is_string())
            {
              error(QUOTED(OCT_FN_NAME)": Dictionary needs a string argument");
              load_dicom_dict(current_dict.c_str()); // reset dictionary to initial value
              return retval;
            }
          std::string dictionary = args(i+1).string_value();
          load_dicom_dict(dictionary.c_str());
          // ignore dictionary argument for further arg processing
          ++i;
        }
      else
        {
          warning(QUOTED(OCT_FN_NAME)": arg not understood: %s", argex.c_str());
        }
    }
	
  dump(filename.c_str());

  // reset dictionary to initial value
  load_dicom_dict(current_dict.c_str());
  return retval;
}
#endif


void dump(const char filename[])
{
  // Instantiate the reader:
  gdcm::Reader reader;
  reader.SetFileName( filename );

  if( !reader.Read() )
    {
      error("Could not read: %s",filename);
      return; 
    }

  gdcm::File &file = reader.GetFile();
  gdcm::DataSet &ds = file.GetDataSet();
  gdcm::FileMetaInformation &hds=file.GetHeader();
  uint64_t offset = 128 + 4;

  octave_stdout << "File:" << filename << "\n";
  octave_stdout << "Location Level Tag         VR Size     Name                           Data\n";
  octave_stdout << "------------------------------------------------------------------------------------------\n";
  dumpDataSet(&hds, 0, offset);
  dumpDataSet(&ds, 0, offset);
}

void dumpDataSet(const gdcm::DataSet *ds, int sequenceDepth, uint64_t &offset)
{
  const gdcm::DataSet::DataElementSet DES=ds->GetDES();
  gdcm::DataSet::Iterator it;
	
  for ( it=DES.begin() ; it != DES.end(); it++ )
    {
      dumpElement(&(*it), sequenceDepth, offset);
    }
}

void dumpElement(const gdcm::DataElement * elem, int sequenceDepth, uint64_t &offset)
{
  const gdcm::Tag tag = elem->GetTag();		
  gdcm::VR vr = elem->GetVR();

  gdcm::DictEntry dictEntry ;
  std::string varname;

  if (!dicom_is_present(tag))
    {
      char fallbackVarname[64];
      snprintf(fallbackVarname,63,"Private_%04x_%04x",tag.GetGroup(),tag.GetElement());
      varname=std::string(fallbackVarname);

      if(tag.GetElement() == (uint16_t)0)
        {
          varname="Group Tag";
        }
    }
  else
    {
      lookup_dicom_entry(dictEntry, tag);
      varname=dictEntry.GetName();
      const gdcm::VR dictvr= dictEntry.GetVR(); 

      if (!vr.Compatible(vr))
        {
          warning(QUOTED(OCT_FN_NAME)": %s has different VR from dictionary. Using VR from file.", varname.c_str());
        }
      
     // find vr from dictionary if it was not in the file
     // lines copied from gdcmPrinter::PrintDataElement
      if ( vr == gdcm::VR::INVALID )
        {
          vr = dictvr;
        }
      // File is explicit, but still prefer vr from dict when UN
      else if ( vr == gdcm::VR::UN && dictvr != gdcm::VR::INVALID )
        {
          vr = dictvr;
        }
      else // cool the file is Explicit !
        {
          // keep vr from file
        }
    }

  if (vr.Compatible(gdcm::VR::OW) && vr.Compatible(gdcm::VR::OB))
    {
      vr = gdcm::VR::OW;
    }

  octave_stdout << std::right << std::setfill('0') << std::setw(8)
                << std::dec << offset << std::setw(0) << std::setfill(' ');
  octave_stdout << std::setw(6) << sequenceDepth << std::setw(0) ;
  octave_stdout << " " << tag << " " << vr << " ";
  if(elem->GetVL().IsUndefined())
    {
      octave_stdout << "      ";
    }
  else
    {
      octave_stdout << std::setw(6) << elem->GetVL() << std::setw(0) ; // size
    }
  octave_stdout << " - " << std::setw(30) << std::left << varname << std::setw(0) << " " ;

  if (!elem->GetVL().IsUndefined())
    {
      offset += uint32_t(elem->GetVL());
    }

  if (tag==gdcm::Tag(0x7FE0,0x0010))
    { // PixelData
      octave_stdout  << "[]";
    }
  else if(vr & gdcm::VR::PN)
    {
      octave_stdout  << "[Anonymized]";
    }
  else if( gdcm::VR::IsASCII(vr))
    {
      #define strValBufLen 511

      char strValBuf[strValBufLen+1];
      const gdcm::ByteValue *bv = elem->GetByteValue();
      uint32_t len = bv ? uint32_t(bv->GetLength()) : 0;

      if ( len > strValBufLen )
        {
          len = strValBufLen;
        }
      if(len)
        {
          memcpy (strValBuf, bv->GetPointer(), len);
        }
      strValBuf[len]='\0';

      if (dicom_truncate_numchar>0)
        {
          octave_stdout << '['
             << std::string(strValBuf).substr(0,dicom_truncate_numchar) 
             << ( ((int)strlen(strValBuf)>dicom_truncate_numchar) ? "..." : "")
             << ']';
        }
      else
        {
          octave_stdout << '[' << strValBuf << ']';
        }
    }
  else if( gdcm::VR::IsBinary(vr))
    {
      octave_stdout  << "*Binary*";
    }
  else
    {
      octave_stdout  << "?";
    }

  offset += 4+(2*gdcm::VR::GetLength(vr));

  octave_stdout  << "\n";

  if (vr & gdcm::VR::SQ)
    {
      gdcm::SmartPointer<gdcm::SequenceOfItems> sqi = elem->GetValueAsSQ();
      if (sqi)
        dumpSequence(sqi, sequenceDepth+1, offset);
    }
}

void dumpSequence(gdcm::SequenceOfItems *seq, int sequenceDepth, uint64_t &offset)
{
  for (gdcm::SequenceOfItems::Iterator sit=seq->Begin(); sit != seq->End(); sit++)
    {
      gdcm::DataSet ds=sit->GetNestedDataSet();
      gdcm::Item mi = *sit;
      for (gdcm::DataSet::Iterator dit=ds.Begin(); dit != ds.End(); dit++)
        {
          dumpElement(&(*dit), sequenceDepth, offset);
        }
    }
}

/*
%!shared testfile
%! testfile = tempname ();

%!fail("dicomdisp");

%!fail("dicomdisp(1)");

%!test
%! wdata = uint8 (10*rand (10,10));
%! dicomwrite (wdata, testfile);
%! dicomdisp(testfile);

%!test
%! if exist (testfile, 'file')
%!   delete (testfile);
%! endif
*/
