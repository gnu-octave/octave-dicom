---
layout: "default"
permalink: "/functions/7_isdicom/"
pkg_name: "dicom"
pkg_version: "0.6.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - isdicom"
category: "Dicom Functions"
func_name: "isdicom"
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
- id: "news"
  name: "News"
  url: "/news"
- id: "manual"
  name: "Manual"
  url: "/manual"
---
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-isdicom"><span class="category-def">: </span><span><code class="def-type"><var class="var">yesno</var> =</code> <strong class="def-name">isdicom</strong> <code class="def-code-arguments">(<var class="var">filename</var>)</code><a class="copiable-link" href="#index-isdicom"></a></span></dt>
<dd><p>Return true if <var class="var">filename</var> is a valid DICOM file.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">filename</var> - name of file to read.
</p>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">yesno</var> - logical value of true if filename is a dicom file.
</p>

<p><strong class="strong">See also:</strong> dicomdict, dicominfo, dicomread, dicomwrite.
</p></dd></dl>