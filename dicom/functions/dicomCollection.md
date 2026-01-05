---
layout: "default"
permalink: "/functions/15_dicomCollection/"
pkg_name: "dicom"
pkg_version: "0.7.0"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomCollection"
category: "Dicom Functions"
func_name: "dicomCollection"
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
<dt class="deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">collection</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomCollection(<var class="var">directory</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-_003d-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">collection</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomCOLLECTION(<var class="var">DICOMFILE</var>, <var class="var">attribute</var>)</code><a class="copiable-link" href="#index-_003d-1"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-_003d-2"><span class="category-def">: </span><span><code class="def-type"><var class="var">collection</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomCOLLECTION(_, <var class="var">propertyname</var>, <var class="var">propertyvalue</var> &hellip;)</code><a class="copiable-link" href="#index-_003d-2"></a></span></dt>
<dd><p>Read a directory or DICOMFILE and return a table or struct of the referenced files.:w
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">directory</var> - directory to read.
</p>
<p><var class="var">DICOMFILE</var> - dicom file rto read.
</p>
<p><var class="var">propertyname</var>, <var class="var">propertyvalue</var> - Optional property name/value pairs.
</p>
<p>Known properties:
</p>
<dl class="table">
<dt>IncludeSubFolders</dt>
<dd><p>Boolean if set to true will also look in sub folders when looking for dicom files
 </p></dd>
<dt>DisplayWaitbar</dt>
<dd><p>Boolean currently ignored in Octave
 </p></dd>
</dl>

<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">collection</var> - a table with fields Location and Value for each matched attribute.
</p>
<p>collection will be a struct array or a table if a struct2table function exists, with fields of:
</p>
<dl class="table">
<dt>Filenames</dt>
<dt>StudyDateTime</dt>
<dt>SeriesDateTime</dt>
<dt>PatientName</dt>
<dt>PatientSex</dt>
<dt>Modality</dt>
<dt>Rows</dt>
<dt>Columns</dt>
<dt>Channels</dt>
<dt>Frames</dt>
<dt>StudyDescription</dt>
<dt>SeriesDescription</dt>
<dt>StudyInstanceUID</dt>
<dt>SeriesInstanceUID</dt>
</dl>

<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> [dirname, ~] = fileparts(file_in_loadpath(&quot;imdata/rtstruct.dcm&quot;));

 collection = dicomCollection(dirname);

 </pre></div>
</dd></dl>