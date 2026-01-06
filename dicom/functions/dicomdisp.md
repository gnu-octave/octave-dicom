---
layout: "default"
permalink: "/functions/9_dicomdisp/"
pkg_name: "dicom"
pkg_version: "0.7.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomdisp"
category: "Dicom Functions"
func_name: "dicomdisp"
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
<dt class="deftypefn def-line" id="index-dicomdisp"><span class="category-def">: </span><span><strong class="def-name">dicomdisp</strong> <code class="def-code-arguments">(<var class="var">filename</var>)</code><a class="copiable-link" href="#index-dicomdisp"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-dicomdisp-1"><span class="category-def">: </span><span><strong class="def-name">dicomdisp</strong> <code class="def-code-arguments">(<var class="var">filename</var>, [<var class="var">propertyname</var>, <var class="var">propertvalue</var> ...])</code><a class="copiable-link" href="#index-dicomdisp-1"></a></span></dt>
<dd><p>Read and display the metadata from a DICOM file.
</p> 
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">filename</var> - dicomfilename to display.<br>
  <var class="var">propertyname</var>, <var class="var">propertvalue</var> - property pairs for options to the display function.
</p>  
<p>Currently the only known property is &rsquo;dictionary&rsquo; to specify a non default dict to use.
  </p><h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p>None
</p> 

<p><strong class="strong">See also:</strong> dicomread, dicominfo.
 </p></dd></dl>