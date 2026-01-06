## Copyright (C) 2026 John Donoghue <john.donoghue@ieee.org>
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
## @deftypefn {} {@var{collection}} = dicomCollection(@var{directory})
## @deftypefnx {} {@var{collection}} = dicomCOLLECTION(@var{DICOMDIR}, @var{attribute})
## @deftypefnx {} {@var{collection}} = dicomCOLLECTION(_, @var{propertyname}, @var{propertyvalue} @dots{})
## Read a directory or DICOMDIR file and return a table or struct of the referenced files.
##
## @subsubheading Inputs
## @var{directory} - directory to read.
##
## @var{DICOMDIR} - dicom DICOMDIR file to read.
##
## @var{propertyname}, @var{propertyvalue} - Optional property name/value pairs.
##
## Known properties:
##
## @table @asis
## @item IncludeSubFolders
## Boolean if set to true will also look in sub-folders when looking for dicom files
## @item DisplayWaitbar
## Boolean currently ignored in Octave
## @end table
##
## @subsubheading Outputs
## @var{collection} - a table with fields Location and Value for each matched attribute.
##
## collection will be a struct array or a table if a struct2table function exists, with fields of:
##
## @table @asis
## @item Filenames
## @item StudyDateTime
## @item SeriesDateTime
## @item PatientName
## @item PatientSex
## @item Modality
## @item Rows
## @item Columns
## @item Channels
## @item Frames
## @item StudyDescription
## @item SeriesDescription
## @item StudyInstanceUID
## @item SeriesInstanceUID
## @end table
##
## @subsubheading Examples
## @example
## [dirname, ~] = fileparts(file_in_loadpath("imdata/rtstruct.dcm"));
##
## collection = dicomCollection(dirname);
##
## @end example
## @end deftypefn

function collection = dicomCollection(dirname, varargin)
  if nargin < 1 || !ischar(dirname)
    error("dicomCollection: Expected first argument as DICOMDIR or directory name string")
  endif

  collection = [];
  IncludeSubFolders = false;

  # properties
  if nargin >= 2
    nvarargin = length(varargin);
    if mod (nvarargin, 2) != 0
      error ("dicomCollection: expected property name, value pairs");
    endif
    if !iscellstr (varargin (1:2:nvarargin))
      error ("dicomCollection: expected property names to be strings");
    endif
 
    for i = 1:2:nvarargin
      propname = tolower (varargin{i});
      propvalue = varargin{i+1};

      if strcmp(propname, "includesubfolders")
        if propvalue
          IncludeSubFolders = true;
        endif
      elseif strcmp(propname, "displaywaitbar")
        # siliently ignored
      else
        error ("dicomCollection: Unknown input property '%s'", propname);
      endif
    endfor
  endif

  if exist ("isfolder") == 0
    # old octave versions dont have isfolder
    isfolder = @isdir;
  endif

  # now process stuff
  if isfolder(dirname)
    # process folder
    collection = process_dicom_dir(dirname, IncludeSubFolders);
  elseif isfile(dirname)
    # process file
    collection = process_dicom_file(dirname);
  else
    error ("dicomCollection: Expected '%s' to be a directname or filename", dirname);
  endif

  if isempty(collection)
    # no collection ? create empty struct
    collection = struct(
      'Filenames', {}, 'StudyDateTime', {}, 'SeriesDateTime', {},
      'PatientName', {}, 'PatientSex', {}, 'Modality',  {},
      'Rows', {}, 'Columns', {}, 'Channels', {}, 'Frames', {},
      'StudyDescription', {}, 'SeriesDescription', {},
      'StudyInstanceUID', {}, 'SeriesInstanceUID', {}
    );
  endif

  if exist("struct2table") > 1
    # if we have a way to create a table, do that
    collection = struct2table(collection);
  endif

endfunction 

function date_str = format_dicom_date_time(dt, tm)
  # convert a dt and tm string into a more standard format 
  # YYYY-MM-DD hh:mm:ss.sss
  date_str = "";
  if !isempty(dt)
    dt = strrep(dt, ".", "");
    date_str = datestr(datevec (dt, "yyyymmdd"), "yyyy-mm-dd");
  endif

  if !isempty(tm)
    tm = strrep(tm, ":", "");
    if !isempty(date_str)
      date_str = [date_str " " datestr(datevec (tm, "HHMMSS.FFF"), "HH:MM:SS.FFF")];
    else
      date_str = datestr(datevec (tm, "HHMMSS.FFF"), "HH:MM:SS.FFF");
    endif
  endif
endfunction

function collection = process_dicom_dir(dirname, includesubdirs)
  # Scan for and process directories
  # This can be called recursivly
  collection = [];
  lst = dir([dirname "/*"]);
  for idx = 1:length(lst)
    # name, folder, isdir
    f = lst(idx);
    if ! f.isdir
      name = fullfile(dirname, f.name);
      if strcmp(f.name, "DICOMDIR") == 0 && isdicom(name)
        info = __dicom_stat__(name);
        collection = [collection info];
      endif
    elseif includesubdirs
      name = fullfile(dirname, f.name);
      collection2 = process_dicom_dir(name, includesubdirs);
      if !isempty(collection2)
        collection = [collection collection2];
      endif
    endif
  endfor
endfunction

function f = get_field_or_default(st, name, deft={})
  # get value from field (if field exists) or use def value
  if isfield(st, name)
    f = st.(name);
  else
    f = deft;
  endif
endfunction

function collection = process_dicom_file(dirname)
  # Process a DICOMDIR file and create structs similar as we do for
  # the normal dcm files 
  #
  # NOTE: this function currently assumes that records
  # will be in some order of PATIENT STUDY SERIES IMAGE 
  # Otherwise we may have data in wrong place ? 

  collection = [];
  [currpath, ~] = fileparts(dirname);
  info = dicominfo(dirname);
  if isfield(info, "DirectoryRecordSequence")
    # initial defalt values struct
    rec = struct(
      'Filenames', '', 'StudyDateTime', '', 'SeriesDateTime', '',
      'PatientName', '', 'PatientSex', '', 'Modality', '',
      'Rows', '', 'Columns', '', 'Channels', 1, 'Frames', 1,
      'StudyDescription', '', 'SeriesDescription', '',
      'StudyInstanceUID', '', 'SeriesInstanceUID', ''
    );
 
    # loop through at the dir entries of the file to create 
    # records
    dirs = fieldnames(info.DirectoryRecordSequence);
    for idx = 1:length(dirs)
      d = info.DirectoryRecordSequence.(dirs{idx});

      switch (strrep(d.DirectoryRecordType, " ", ""))
        case 'IMAGE'
          rec.Rows = d.Rows;
          rec.Columns = d.Columns;
          rec.Filenames = fullfile(currpath, strrep(d.ReferencedFileID, "\\", "/"));
          collection = [collection rec];
        case 'PATIENT'
          rec.PatientName = get_field_or_default(d, 'PatientName', "");
          rec.PatientSex = get_field_or_default(d, 'PatientSex', "");
        case 'STUDY'
          rec.StudyInstanceUID = get_field_or_default(d, "StudyInstanceUID", "");
          rec.StudyDescription = get_field_or_default(d, "StudyDescription", "");
          # TODO: format study date/time correctly
          # 20100806 010434.000
          #rec.StudyDateTime = get_field_or_default(d, "StudyDate", ""); 
          #rec.StudyDateTime = get_field_or_default(d, "StudyTime", ""); 
          rec.StudyDateTime = format_dicom_date_time(get_field_or_default(d, "StudyDate", ""), get_field_or_default(d, "StudyTime", ""));
        case 'SERIES'
          rec.SeriesInstanceUID = get_field_or_default(d, "SeriesInstanceUID", "");
          rec.SeriesDescription = get_field_or_default(d, "SeriesDescription", "");
          rec.Modalilty = get_field_or_default(d, "Modalilty", "");
          #rec.SeriesDateTime = get_field_or_default(d, "SeriesDate", ""); 
          #rec.SeriesDateTime = get_field_or_default(d, "SeriesTime", ""); 
          rec.SeriesDateTime = format_dicom_date_time(get_field_or_default(d, "SeriesDate", ""), get_field_or_default(d, "SeriesTime", ""));
        otherwise
          # ignore whatever it is
      endswitch
    endfor
  endif
endfunction

%!test
%! [dirname, ~] = fileparts(file_in_loadpath("imdata/rtstruct.dcm"));
%! collection = dicomCollection(dirname);
%! assert(length(collection),5)

# TODO: package and add a DICOMDIR test

