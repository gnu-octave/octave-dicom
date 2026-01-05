---
layout: "default"
permalink: "/functions/22_imagesdicomdecodeUID/"
pkg_name: "dicom"
pkg_version: "0.7.0"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - images.dicom.decodeUID"
category: "Dicom Functions"
func_name: "images.dicom.decodeUID"
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
<dt class="deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">uidinfo</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">images.dicom.decodeUID(<var class="var">uid</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dd><p>Look up information about a uid string
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">uid</var> - dicom uid string.
</p>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">uidinfo</var> - a structure with fields of Value, Name and Type.
</p>
<p>Additional fields may be present in the case of a Transfer Syntax type uid.
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> &gt; info = images.dicom.decodeUID(&quot;1.2.840.10008.1.1&quot;);
 info =
  scalar structure containing the fields:
    Value = 1.2.840.10008.1.1
    Name = Verification SOP Class
    Type = SOP Class
 </pre></div>
</dd></dl>