---
layout: "default"
permalink: "/functions/9_dicomanon/"
pkg_name: "dicom"
pkg_version: "0.7.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomanon"
category: "Dicom Functions"
func_name: "dicomanon"
navigation:
- id: "overview"
  name: "Overview"
  url: "/index"
- id: "Functions"
  name: "Function Reference"
  url: "/functions"
- id: "15_DicomFunctions"
  name: "&nbsp;&nbsp;Dicom Functions"
  url: "/functions/#15_DicomFunctions"
  subitems:
- id: "news"
  name: "News"
  url: "/news"
- id: "manual"
  name: "Manual"
  url: "/manual"
---
<dl class="first-deftypefn def-block">
<dt class="deftypefn def-line" id="index-dicomanon_0028file_005fin_002c"><span class="category-def">: </span><span><strong class="def-name">dicomanon(<var class="var">file_in</var>,</strong> <code class="def-code-arguments"><var class="var">file_out</var>)</code><a class="copiable-link" href="#index-dicomanon_0028file_005fin_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-dicomanon_0028_005f_005f_005f_002c"><span class="category-def">: </span><span><strong class="def-name">dicomanon(___,</strong> <code class="def-code-arguments"><var class="var">name</var>, <var class="var">value</var>)</code><a class="copiable-link" href="#index-dicomanon_0028_005f_005f_005f_002c"></a></span></dt>
<dd>
<p>Anonymize a DICOM format file by removing or replacing specific fields.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">file_in</var> is filename to read from.<br>
<var class="var">file_out</var> is the filename to write to.<br>
<var class="var">name</var>, <var class="var">value</var> optional name/value properties.<br>
</p>
<p>Known property names are:
</p><dl class="table">
<dt>keep</dt>
<dd><p>The value is a cell array of names to not remove during the anonymize procedure.
</p></dd>
<dt>update</dt>
<dd><p>A structure of name/values to update rather than remove.
</p></dd>
</dl>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p>None
</p>

<p><strong class="strong">See also:</strong> dicomread, dicomwrite, dicominfo.
</p></dd></dl>