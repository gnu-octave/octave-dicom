/*
 * Copyright John Donoghue, 2023
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

#include <gdcmUIDs.h>
#include <gdcmSOPClassUIDToIOD.h>
#include <gdcmTransferSyntax.h>
using namespace gdcm;

#define DICOM_ERR -1
#define DICOM_OK 0

#define OCT_FN_NAME __dicom_decodeuid__
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

DEFUN_DLD (__dicom_decodeuid__, args, nargout,
    "-*- texinfo -*- \n\
@deftypefn {} {} __dicom_decodeuid__ () \n\
Internal undocumented function\n\
@end deftypefn \n\
")
{
  octave_value_list retval;  // create object to store return values

  if (1 != args.length ())
    {
      print_usage ();
      return retval; 
    }

  if (! args(0).is_string())
    {
      error (QUOTED(OCT_FN_NAME)": uid should be a string");
      return retval; 
    }

  octave_map om;
  gdcm::UIDs uid;

  if (! uid.SetFromUID(args(0).string_value().c_str()))
    {
      om.assign("Value", octave_value(""));
      om.assign("Name", octave_value(""));
      om.assign("Type", octave_value(""));
      retval(0) = om;
      return retval;
    }

  om.assign("Value", octave_value(uid.GetString()));
  om.assign("Name", octave_value(uid.GetName()));

  gdcm::TransferSyntax::TSType tt = gdcm::TransferSyntax::GetTSType(args(0).string_value().c_str());
  if (tt != gdcm::TransferSyntax::TSType::TS_END)
  {
    gdcm::TransferSyntax t(tt);

    om.assign("Type", octave_value("Transfer Syntax"));
    om.assign("Compressed", octave_value(t.IsEncapsulated()));
    om.assign("LossyCompression", octave_value(t.IsLossy()));
    om.assign("Endian", octave_value(t.GetSwapCode() == gdcm::SwapCode::BigEndian ? "ieee-be" : "ieee-le"));
    om.assign("VR", octave_value(t.IsExplicit() ? "Explicit" : "Implicit"));
  }
  /*else if(gdcm::SOPClassUIDToIOD::GetIOD(uid))
  {
    om.assign("Type", octave_value("SOP Class"));
  }*/
  else
    om.assign("Type", octave_value("SOP Class"));

  retval(0) = om;

  return retval;
}

/*
%!fail("__dicom_decodeuid__")
%!fail("__dicom_decodeuid__(1)")

%!test
%! s = __dicom_decodeuid__("1.0");
%! assert(isstruct(s))
%! assert(s.Value, "")
%! assert(s.Name, "")

%!test
%! s = __dicom_decodeuid__("1.2.840.10008.1.1");
%! assert(isstruct(s))
%! assert(s.Value, "1.2.840.10008.1.1")
%! assert(s.Name, "Verification SOP Class")
%! assert(s.Type, "SOP Class")
*/
