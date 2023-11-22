## Copyright (C) 2022 John Donoghue <john.donoghue@ieee.org>
##
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation; either version 3 of the License, or (at your option) any later
## version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {@var{uidinfo}} = images.dicom.decodeUID(@var{uid})
## Look up information about a uid string
##
## @subsubheading Inputs
## @var{uid} - dicom uid string.
##
## @subsubheading Outputs
## @var{uidinfo} - a structure with fields of Value, Name and Type.
##
## Additional fields may be present in the case of a Transfer Syntax type uid.
##
## @subsubheading Examples
## @example
## > info = images.dicom.decodeUID("1.2.840.10008.1.1");
## info =
##  scalar structure containing the fields:
##    Value = 1.2.840.10008.1.1
##    Name = Verification SOP Class
##    Type = SOP Class
## @end example
## @end deftypefn

function info = decodeUID(uid)
  if ! ischar(uid)
    error ("Expected uid as a string");
  endif

  info = __dicom_decodeuid__(uid);
endfunction

%!fail("images.dicom.decodeUID")
%!fail("images.dicom.decodeUID(1)")

%!test
%! s = images.dicom.decodeUID("1.0");
%! assert(isstruct(s))
%! assert(s.Value, "")
%! assert(s.Name, "")

%!test
%! s = images.dicom.decodeUID("1.2.840.10008.1.1");
%! assert(isstruct(s))
%! assert(s.Value, "1.2.840.10008.1.1")
%! assert(s.Name, "Verification SOP Class")
%! assert(s.Type, "SOP Class")

%!test
%! s = images.dicom.decodeUID("1.2.840.10008.1.2.1");
%! assert(isstruct(s))
%! assert(s.Value, "1.2.840.10008.1.2.1")
%! assert(s.Name, "Explicit VR Little Endian")
%! assert(s.Type, "Transfer Syntax")
%! assert(s.Endian, "ieee-le")
%! assert(s.Compressed, false)
