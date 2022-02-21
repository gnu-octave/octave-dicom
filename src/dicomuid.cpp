/*
 * Copyright John Donoghue, 2017-2020
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

#include "octave/oct.h"
#include <octave/ov-struct.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gdcmUIDGenerator.h>
using namespace gdcm;

#define DICOM_ERR -1
#define DICOM_OK 0

#define OCT_FN_NAME dicomuid
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

DEFUN_DLD (dicomuid, args, nargout,
    "-*- texinfo -*- \n\
@deftypefn {Loadable Function} @var{uuid} = dicomuid () \n\
\n\
Generate a DICOM unique id . \n\
\n \
@var{uuid} is a unique id string.\n\
\n\
@end deftypefn \n\
")
{
  octave_value_list retval;  // create object to store return values

  // no args required
  if (0 != args.length ())
    {
      print_usage ();
      return retval; 
    }

  gdcm::UIDGenerator generator;

  retval = octave_value (generator.Generate ());

  return retval;
}

/*
%!fail ("dicomuuid (1)");
%!fail ("dicominfo ([])");
%!fail ("dicominfo ('string')");

%!test
%! id = dicomuid ();
%! assert (ischar (id));
%! assert (length(id) > 0);

%!test
%! id1 = dicomuid ();
%! id2 = dicomuid();
%! assert (strcmp (id1, id2), false);
*/
