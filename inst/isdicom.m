%% Copyright (C) 2012 Adam H Aitkenhead
%%
%% This program is free software; you can redistribute it and/or modify
%% it under the terms of the GNU General Public License as published by
%% the Free Software Foundation; either version 3 of the License, or
%% (at your option) any later version.
%%
%% This program is distributed in the hope that it will be useful,
%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%% GNU General Public License for more details.
%%
%% You should have received a copy of the GNU General Public License
%% along with this program; If not, see <http://www.gnu.org/licenses/>.

%% -*- texinfo -*-
%% @deftypefn {Function File} {@var{output} = } isdicom(@var{filename})
%% Check whether a file is DICOM or not.
%%
%% @var{filename} is a string giving the path to a file which is to be
%% checked whether it is DICOM or not.  To check multiple files,
%% @var{filename} can be in the form of a cell array of strings.
%%
%% Files are checked to see whether they are DICOM in two stages:
%% [i] Standard Dicom files have a 128 byte header, followed by 4 bytes
%% containing 'DICM', followed by the actual data. The first method
%% checks for the presence of this 'DICM' text.
%% [ii] However, some non-standard DICOM files omit the header and the
%% 'DICM' flag. If 'DICM' is not found then the first 4 bytes of the file
%% are inspected check if they correspond to any tag in the DICOM
%% dictionary.
%% @end deftypefn

function [output] = isdicom(filename)

  %-------------------
  % Check if filename is a cell
  %-------------------
  if iscell(filename)~=1
    filename = {filename};
  end

  filecount = numel(filename);


  %-------------------
  % Initialise the output:
  %-------------------
  output = false(filecount,1);

  for loopF = 1:filecount

    %-------------------
    % Check the filesize
    %-------------------
    fid = fopen(filename{loopF});
    fseek(fid,0,1);
    filesize = ftell(fid);

    %-------------------
    % Check using method 1
    %-------------------
    % Check if the 4 bytes at the end of the header contain 'DICM':
    if filesize>=132
      fseek(fid,128,-1);                        % Skip the 128 byte header
      DICMflag = char(fread(fid,4,'uint8')');   % Read the next 4 bytes
      if strcmp(DICMflag,'DICM')==1
        output(loopF) = 1;
      end
    end

    %-------------------
    % Check using method 2
    %-------------------
    % If the header is missing, then check if the first few bytes correspond to
    % any tag in the DICOM dictionary:
    if output(loopF)==0 && filesize>=4
      if exist('tagnums','var')==0
        [tag,~,~,~] = READ_dicom_dict;                        % Read the DICOM dictionary
        tagnums     = reshape(hex2dec(char(tag)),size(tag));  % Convert the tags to numeric values
      end
      fseek(fid,0,-1);
      DICMtag = fread(fid,2,'uint16')';                       % Read the first tag of the file being checked
      loopT   = 0;
      while output(loopF)==0 && loopT<size(tagnums,1)         % Check if the tag is in the dictionary
        loopT = loopT + 1;
        if DICMtag(1)==tagnums(loopT,1) && DICMtag(2)==tagnums(loopT,2)
          output(loopF) = 1;
        end
      end
    end

    % Close the file
    fclose(fid);

  end %loopF

end %function


function [tag,vr,keyword,vm] = READ_dicom_dict
  % Read the DICOM dictionary

  dictpath = dicomdict('get');

  fid  = fopen(dictpath);
  data = textscan(fid,'%s','Delimiter','\n');
  fclose(fid);

  %Remove any blank lines:
  data = data{:}(logical(~strcmp(data{:},'')));

  %Remove any comment lines:
  datakeep = false(numel(data),1);
  for loopD = 1:numel(data)
    if numel(regexp(data{loopD}(1),'[#%]'))<1
      datakeep(loopD) = 1;
    end
  end
  data = data(datakeep);

  %Read the tag,vr,keyword,vm:
  tag     = cell(numel(data),2);
  vr      = cell(numel(data),1);
  keyword = cell(numel(data),1);
  vm      = cell(numel(data),1);
  for loopD = 1:numel(data)
    spaceIND       = regexp(data{loopD},'\s');
    tag{loopD,1}   = data{loopD}(2:5);
    tag{loopD,2}   = data{loopD}(7:10);
    vr{loopD}      = data{loopD}(spaceIND(1)+1:spaceIND(2)-1);
    keyword{loopD} = data{loopD}(spaceIND(2)+1:spaceIND(3)-1);
    vm{loopD}      = data{loopD}(spaceIND(3)+1:end);
  end

  %Ensure all tags contain only hexadecimal ('1234567890abcdef' or '1234567890ABCDEF') characters:
  keepIND    = true(numel(data),1);
  keepSTRING = '[1234567890abcdefABCDEF]';
  for loopD = 1:numel(data)
    if numel(regexp(tag{loopD,1},keepSTRING))<4 || numel(regexp(tag{loopD,2},keepSTRING))<4
      keepIND(loopD) = 0;
    end
  end
  tag     = tag(keepIND,:);
  vr      = vr(keepIND,:);
  keyword = keyword(keepIND,:);
  vm      = vm(keepIND,:);

end %function

%!test
%! addpath('../inst'); % so it can find the dictionary, and m files
%! assert(isdicom('../dcm_examples/RD.15MV.DCM'));
%!test
%! addpath('../inst');
%! assert(~isdicom('../inst/isdicom.m'));

