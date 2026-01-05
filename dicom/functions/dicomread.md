---
layout: "default"
permalink: "/functions/9_dicomread/"
pkg_name: "dicom"
pkg_version: "0.7.0"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomread"
category: "Dicom Functions"
func_name: "dicomread"
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
<dt class="deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">image</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomread (<var class="var">filename</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-_003d-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">image</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomread (<var class="var">structure</var>)</code><a class="copiable-link" href="#index-_003d-1"></a></span></dt>
<dd>
<p>Load the image from a DICOM file. 
</p><h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">filename</var> - a string giving the filename.*
</p>
<p><var class="var">structure</var> - a structure with a field <code class="code">Filename</code> (such as returned by <code class="code">dicominfo</code>).
</p>
<p><var class="var">image</var> - may be two or three dimensional, depending on the content of the file. 
</p>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">image</var> - An integer or float matrix will be returned, the number of bits will depend on the file. 
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<p>Load the image data of a dcm file:
</p>
<div class="example">
<pre class="example-preformatted">&gt; image = dicomread(file_in_loadpath('imdata/simple-test.dcm'))
image =
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
  0  0  0  0  0
</pre></div>


<p><strong class="strong">See also:</strong> dicominfo.
</p></dd></dl>