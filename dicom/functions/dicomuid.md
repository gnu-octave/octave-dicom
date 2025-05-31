---
layout: "default"
permalink: "/functions/8_dicomuid/"
pkg_name: "dicom"
pkg_version: "0.6.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomuid"
category: "Dicom Functions"
func_name: "dicomuid"
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
<dt class="deftypefn" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">uuid</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomuid ()</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dd>
<p>Generate a DICOM unique id . 
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p>None
</p><h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">uuid</var> is a unique id string.
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<p>Get a new uid:
</p>
<div class="example">
<pre class="example-preformatted">&gt; uid = dicomuid ()
uid = 1.2.826.0.1.3680043.2.1143.3114589836670200378351641061429967573
</pre></div>

</dd></dl>