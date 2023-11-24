## Copyright (C) 2023 John Donoghue <john.donoghue@ieee.org>
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
## @deftypefn {} {@var{info}} = images.dicom.parseDICOMDIR(@var{filename})
## Parse a DICOMDIR file and return the information as a struct.
##
## @subsubheading Inputs
## @var{filename} - filename to open.
##
## @subsubheading Outputs
## @var{info} - A struct containing the directory information from a DICOMDIR file
##
## Th info structure will be an array of Patients, with an array of Studies with an
## array of Series with an array of Images.
## 
## Each element will contain a Payload field.
##
## @subsubheading Examples
## @example
## filename = file_in_loadpath("imdata/DICOMDIR");
##
## info = images.dicom.parseDICOMDIR(filename);
##
## @end example
## @end deftypefn

function finfo = parseDICOMDIR(filename)
  finfo = {};

  if ischar(filename) 
    info = dicominfo(filename);
  else
    error ("Expected first argument as a filename");
  endif

  dinfo = dicominfo(filename);
  if isfield(dinfo, "DirectoryRecordSequence")
    dinfo = dinfo.DirectoryRecordSequence;

    fields = fieldnames(dinfo);
    len = length(fields);

    for idx=1:len
      inf = dinfo.(fields{idx});
      if strcmpi(inf.DirectoryRecordType, "PATIENT ")
        if isempty(finfo)
          finfo.Patients = [];
        endif
        payload = struct(
          "OffsetOfTheNextDirectoryRecord", inf.OffsetOfTheNextDirectoryRecord,
          "RecordInUseFlag", inf.RecordInUseFlag,
          "OffsetOfReferencedLowerLevelDirectoryEntity", inf.OffsetOfReferencedLowerLevelDirectoryEntity,
          "DirectoryRecordType", "PATIENT",
          "PatientName", inf.PatientName,
          "PatientID", inf.PatientID
        );

        finfo.Patients(end+1) = struct('Payload', payload, 'Studies', []);
      elseif strcmpi(inf.DirectoryRecordType, "STUDY ")
        if !isempty(finfo)
          payload = struct(
            "OffsetOfTheNextDirectoryRecord", inf.OffsetOfTheNextDirectoryRecord,
            "RecordInUseFlag", inf.RecordInUseFlag,
            "OffsetOfReferencedLowerLevelDirectoryEntity", inf.OffsetOfReferencedLowerLevelDirectoryEntity,
            "DirectoryRecordType", "STUDY",
            "StudyDate", inf.StudyDate,
            "StudyTime", inf.StudyTime,
            "AccessionNumber", inf.AccessionNumber,
            "StudyDescription", inf.StudyDescription,
            "StudyID", inf.StudyID
          );

          finfo.Patients(end).Studies(end+1) = struct('Payload', payload, 'Series', []);
        endif
      elseif strcmpi(inf.DirectoryRecordType, "SERIES")
        if !isempty(finfo) && !isempty(finfo.Patients(end).Studies)
          payload = struct(
            "OffsetOfTheNextDirectoryRecord", inf.OffsetOfTheNextDirectoryRecord,
            "RecordInUseFlag", inf.RecordInUseFlag,
            "OffsetOfReferencedLowerLevelDirectoryEntity", inf.OffsetOfReferencedLowerLevelDirectoryEntity,
            "DirectoryRecordType", "SERIES",
            "Modality", inf.Modality,
            "SeriesInstanceUID", inf.SeriesInstanceUID,
            "SeriesNumber", inf.SeriesNumber
          );

          finfo.Patients(end).Studies(end).Series(end+1) = struct('Payload', payload, 'Images', []);
        endif
      elseif strcmpi(inf.DirectoryRecordType, "IMAGE ")
        if !isempty(finfo) && !isempty(finfo.Patients(end).Studies) && !isempty(finfo.Patients(end).Studies(end).Series)
          payload = struct(
            "OffsetOfTheNextDirectoryRecord", inf.OffsetOfTheNextDirectoryRecord,
            "RecordInUseFlag", inf.RecordInUseFlag,
            "OffsetOfReferencedLowerLevelDirectoryEntity", inf.OffsetOfReferencedLowerLevelDirectoryEntity,
            "DirectoryRecordType", "IMAGE",
            "ReferencedFileID", inf.ReferencedFileID,
            "ReferencedSOPClassUIDInFile", inf.ReferencedSOPClassUIDInFile,
            "ReferencedSOPInstanceUIDInFile", inf.ReferencedSOPInstanceUIDInFile,
            "ReferencedTransferSyntaxUIDInFile", inf.ReferencedTransferSyntaxUIDInFile,
            "InstanceNumber", inf.InstanceNumber
          );

          finfo.Patients(end).Studies(end).Series(end).Images(end+1) = struct('Payload', payload);
        endif
 
      endif
    endfor
  else
    error ("Not a DICOMDIR file");
  endif
endfunction

%!fail ('images.dicom.parseDICOMDIR()')
%!fail ('images.dicom.parseDICOMDIR(1)')
%!fail ('images.dicom.parseDICOMDIR(file_in_loadpath("imdata/rtstruct.dcm"))')
