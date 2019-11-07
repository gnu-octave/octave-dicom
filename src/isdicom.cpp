/*
 * The GNU Octave dicom package is Copyright Andy Buckle 2010
 * Contact: blondandy using the sf.net system, 
 * <https://sourceforge.net/sendmessage.php?touser=1760416>
 *
 * Changes (C) 2018 John Donoghue <john.donoghue.ieee.org>
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

#include <octave/oct.h>
#include <octave/ov-struct.h>
#include <octave/file-stat.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gdcmImageReader.h"
using namespace gdcm;

static bool
isdicom (const std::string& filename)
{
  if (!filename.length ())
    return false;

  gdcm::Reader reader;
  reader.SetFileName (filename.c_str ());

  // gdcm::Reader.Read() will return false if the file does not exists but
  // also prints to stderr so we check it first.
  return OCTAVE__FILE_STAT (filename).exists () && reader.Read ();
}

DEFUN_DLD (isdicom, args, ,
"-*- texinfo -*- \n\
@deftypefn {Loadable Function} {} isdicom (@var{filename}) \n\
Return true if @var{filename} is a valid DICOM file.\n\
\n\
@seealso{dicomdict, dicominfo, dicomread, dicomwrite} \n\
@end deftypefn")
{
  const octave_idx_type nargin = args.length ();
  if (nargin != 1)
    {
      print_usage ();
      return octave_value_list ();
    }
  if (! args(0).is_string ())
    {
      error ("isdicom: FILENAME must be a string");
      return octave_value_list ();
    }
  // Do NOT have this function accept a cell array of strings.
  //
  // This function should return true or false if its input is valid input
  // for a function that reads a DICOM file.  While it is tempting, and very
  // easy to implement, to have this function accept a cell array of strings
  // and return a boolNDArray, a cell array is not valid and will cause any
  // function that uses it to first check if it's a cell array.  It will also
  // prevent it from being used by imformats and any other image IO function
  // in Octave core
  const std::string filename = args(0).string_value ();
  
  return octave_value (isdicom (filename));
}

/*
%!test
%! fpath = tempname ();
%! urlwrite ("http://imagej.nih.gov/ij/images/CT%20Scan.dcm", fpath);
%! assert (isdicom (fpath), true);

%!test
%! assert (isdicom (which ("gray")), false);
%! assert (isdicom (""), false);

%!fail ("isdicom")

%!fail ("isdicom(1)")
*/
