---
layout: "default"
permalink: "/functions/9_dicomfind/"
pkg_name: "dicom"
pkg_version: "0.7.0"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomfind"
category: "Dicom Functions"
func_name: "dicomfind"
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
<dt class="deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">attrinfo</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomfind(<var class="var">filename</var>, <var class="var">attribute</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-_003d-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">attrinfo</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomfind(<var class="var">info</var>, <var class="var">attribute</var>)</code><a class="copiable-link" href="#index-_003d-1"></a></span></dt>
<dd><p>Find the location and value of an attribute in a dicom file or info structure.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">filename</var> - filename to open.
</p>
<p><var class="var">info</var> - dicominfo struct.
</p>
<p><var class="var">attribute</var> - attribute name to find.
</p>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">attrinfo</var> - a table with fields Location and Value for each matched attribute.
</p>
<p>The Location value will be the attribute position in dot notation to show its position
 in dicom info structure.
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> filename = file_in_loadpath(&quot;imdata/rtstruct.dcm&quot;);

 info = dicomfind(filename, &quot;ROINumber&quot;);

 </pre></div>
</dd></dl>