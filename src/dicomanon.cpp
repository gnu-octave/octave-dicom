/*
 * Copyright John Donoghue, 2020
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

#include <octave/oct.h>
#include <octave/ov-struct.h>
#include <octave/Cell.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gdcmReader.h>
#include <gdcmWriter.h>
#include <gdcmAnonymizer.h>
#include <gdcmDictEntry.h>
#include <gdcmDefs.h>
#include <gdcmUIDGenerator.h>

#include "dicomdict.h"
              
#define OCT_FN_NAME dicomanon
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

bool lookup_tag (const std::string & keyword, gdcm::Tag &tag)
{
  gdcm::DictEntry entry;
  if (!dicom_is_present(keyword))
    {
      return false;
    }
  lookup_dicom_tag (tag, keyword);
  lookup_dicom_entry (entry, tag);
  return true;
}

DEFUN_DLD (dicomanon, args, nargout,
    "-*- texinfo -*- \n\
@deftypefn {Loadable Function} {} dicomanon(@var{file_in}, @var{file_out})\n\
@deftypefnx {Loadable Function} {} dicomanon(___, @var{name}, @var{value})\n\
\n\
Anonymize a DICOM format file by removing or replacing specific fields.\n\
\n\
@subsubheading Inputs\n\
@var{file_in} is filename to read from.@*\n\
@var{file_out} is the filename to write to.@*\n\
@var{name}, @var{value} optional name/value properties.@*\n\
\n\
Known property names are:\n\
@table @asis\n\
@item keep\n\
The value is a cell array of names to not remove during the anonymize procedure.\n\
@item update\n\
A structure of name/values to update rather than remove.\n\
@end table\n\
@subsubheading Outputs\n\
None\n\
\n\
@seealso{dicomread, dicomwrite, dicominfo}\n\
@end deftypefn \n\
")
{
  octave_value_list retval;  // create object to store return values
  if (2 > args.length())
    {
      error (QUOTED(OCT_FN_NAME)": should have at least 2 arguments");
      return retval; 
    }

  if (!  (args(0).is_string () && args(1).is_string ()))
    {
      error (QUOTED(OCT_FN_NAME)": first and second argument should be a string filename");
      return retval; 
    }

  std::vector<gdcm::Tag> remove_tags = 
    gdcm::Anonymizer::GetBasicApplicationLevelConfidentialityProfileAttributes ();

  // remove (0x0008, 0x0018) as we MUST have it to create the file
  remove(remove_tags.begin(), remove_tags.end(), gdcm::Tag(0x0008, 0x0018));

  std::vector< std::pair<gdcm::Tag, std::string> > replace_tags;
  gdcm::UIDGenerator uid;
  const char *u = uid.Generate();
  replace_tags.push_back(std::pair<gdcm::Tag, std::string>(gdcm::Tag(0x0008, 0x0018), u));

  if (args.length() % 2 !=  0)
    {
      error (QUOTED(OCT_FN_NAME)": expected name/value pairs after filenames");
      return retval; 
    }

  for (int i=2; i<args.length(); i+=2)
    {
      if (!  args(i).is_string ())
        {
          error (QUOTED(OCT_FN_NAME)": expected propty name at input %d", (i+1));
          return retval; 
        }

      std::string name = args(i).string_value ();
      if (name == "keep")
        {
          if (! args(i+1).OV_ISCELL())
            {
              error (QUOTED(OCT_FN_NAME)": expected fields value to be a cell aray");
              return retval; 
            }
          Cell values = args(i+1).cell_value();
          gdcm::Tag tag(0x0008, 0x0018);
          for (octave_idx_type idx = 0; idx < values.numel (); idx++)
            {
              name = values(idx).string_value();
              if (lookup_tag(name, tag))
                {
                  remove(remove_tags.begin(), remove_tags.end(), tag);
                }
            }
        }
      else if (name == "update")
        {
          if (! args(i+1).OV_ISMAP())
            {
              error (QUOTED(OCT_FN_NAME)": expected update value to be a struct");
              return retval; 
            }

          octave_scalar_map values = args(i+1).scalar_map_value();
          gdcm::Tag tag(0x0008, 0x0018);
          for (octave_scalar_map::iterator it = values.begin(); it != values.end(); it++)
            {
              name = values.key(it);
              
	      octave_value value = values.contents(it);
              if (lookup_tag(name, tag))
                {
                  remove(remove_tags.begin(), remove_tags.end(), tag);
                  // add to update
                  replace_tags.push_back(std::pair<gdcm::Tag, std::string>(tag, value.string_value()));
                }
            }
        }
      else
        {
          error (QUOTED(OCT_FN_NAME)": unknown property name '%s'", name.c_str());
          return retval; 
        }
    }

  std::string infilename = args(0).string_value ();
  std::string outfilename = args(1).string_value ();

  gdcm::Reader reader;
  reader.SetFileName( infilename.c_str() );

  if (!reader.Read ())
    {
      error (QUOTED(OCT_FN_NAME)": Could not read DICOM file: %s",infilename.c_str());
      return retval;
    }

  gdcm::File &file = reader.GetFile();

  gdcm::Anonymizer anon;
  anon.SetFile( file );

  // remove tags we want to remove
  std::vector<gdcm::Tag>::const_iterator it = remove_tags.begin();

  for (; it != remove_tags.end(); ++it)
    {
      gdcm::Tag tag = *it;
      if (! anon.Remove (tag))
        {
          warning (QUOTED(OCT_FN_NAME)": Failed to remove: %04x %04x",tag.GetGroup(), tag.GetElement());
        }
    }

  std::vector< std::pair<gdcm::Tag, std::string> >::const_iterator it2 = replace_tags.begin();

  // update tages we want to update
  for(; it2 != replace_tags.end(); ++it2)
    {
      gdcm::Tag tag = it2->first;
      if (! anon.Replace (tag, it2->second.c_str()))
        {
          warning (QUOTED(OCT_FN_NAME)": Failed to replace: %04x %04x",tag.GetGroup(), tag.GetElement());
        }
    }

  gdcm::FileMetaInformation &fmi = file.GetHeader();
  fmi.Clear();

  gdcm::Writer writer;

  writer.SetFileName( outfilename.c_str() );
  writer.SetFile( file );

  if (! writer.Write())
    {
      error (QUOTED(OCT_FN_NAME)": Could not write DICOM file: %s", outfilename.c_str());

      return retval;
    }

  return retval;
}

/*
%!shared testfile1, testfile2
%! testfile1 = tempname();
%! testfile2 = tempname();
%! wdata = uint8 (10*rand (10,10));
%! s.PatientName = "John";
%! s.PatientAge = "20";
%! dicomwrite (wdata, testfile1, s);

%!fail ("dicomanon");
%!fail ("dicomanon (1, 1)");
%!fail ("dicomanon (testfile1, 1)");
%!fail ("dicomanon (testfile1, testfile2, 'a')");

%!test
%! info = dicominfo(testfile1);
%! assert (isfield(info, "PatientName"));
%! assert (isfield(info, "PatientAge"));
%! assert (info.PatientName, "John");
%! assert (info.PatientAge, "20");

%!test
%! dicomanon(testfile1, testfile2);
%! info2 = dicominfo(testfile2);
%! assert (!isfield(info2, "PatientName"));
%! assert (!isfield(info2, "PatientAge"));

%!test
%! dicomanon(testfile1, testfile2, "keep", {"PatientAge"});
%! info3 = dicominfo(testfile2);
%! assert (isfield(info3, "PatientAge"));
%! assert (info3.PatientAge, "20");
%! assert (!isfield(info3, "PatientName"));

%!test
%! attrs.PatientAge = "21";
%! dicomanon(testfile1, testfile2, "update", attrs);
%! info5 = dicominfo(testfile2);
%! assert (info5.PatientAge, "21");
%! assert (!isfield(info5, "PatientName"));

%!test
%! attrs.PatientAge = "21";
%! dicomanon(testfile1, testfile2, "update", attrs, "keep", {'PatientName'});
%! info6 = dicominfo(testfile2);
%! assert (info6.PatientAge, "21");
%! assert (isfield(info6, "PatientName"));

%!test
%! if exist (testfile1, 'file')
%!   delete (testfile1);
%! endif
%! if exist (testfile2, 'file')
%!   delete (testfile2);
%! endif
*/
