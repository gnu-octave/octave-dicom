---
layout: "default"
permalink: "/functions/26_imagesdicomparseDICOMDIR/"
pkg_name: "dicom"
pkg_version: "0.7.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - images.dicom.parseDICOMDIR"
category: "Dicom Functions"
func_name: "images.dicom.parseDICOMDIR"
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
<dt class="deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">images.dicom.parseDICOMDIR(<var class="var">filename</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dd><p>Parse a DICOMDIR file and return the information as a struct.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">filename</var> - filename to open.
</p>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">info</var> - A struct containing the directory information from a DICOMDIR file
</p>
<p>Th info structure will be an array of Patients, with an array of Studies with an
 array of Series with an array of Images.
</p> 
<p>Each element will contain a Payload field.
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> filename = file_in_loadpath(&quot;imdata/DICOMDIR&quot;);

 info = images.dicom.parseDICOMDIR(filename);

 </pre></div>
</dd></dl>