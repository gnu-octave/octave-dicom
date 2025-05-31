---
layout: "default"
permalink: "/functions/10_dicomwrite/"
pkg_name: "dicom"
pkg_version: "0.6.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomwrite"
category: "Dicom Functions"
func_name: "dicomwrite"
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
<dt class="deftypefn" id="index-dicomwrite_0028im_002c"><span class="category-def">: </span><span><strong class="def-name">dicomwrite(<var class="var">im</var>,</strong> <code class="def-code-arguments"><var class="var">filename</var>)</code><a class="copiable-link" href="#index-dicomwrite_0028im_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomwrite_0028im_002c-1"><span class="category-def">: </span><span><strong class="def-name">dicomwrite(<var class="var">im</var>,</strong> <code class="def-code-arguments"><var class="var">filename</var>, <var class="var">info</var>)</code><a class="copiable-link" href="#index-dicomwrite_0028im_002c-1"></a></span></dt>
<dd>
<p>Write a DICOM format file to <var class="var">filename</var>.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">im</var> - image data or empty matrix, [], if only metadata save is required
</p>
<p><var class="var">filename</var> - filename to write dicom to. if [], then function runs in verbose trial mode.
</p>
<p><var class="var">info</var> - struct, like that produced by dicominfo
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<p>Create a dicom file using default info, and the supplied data:
</p><div class="example">
<pre class="example-preformatted">&gt; wdata = uint8 (10*rand (10,10));
&gt; dicomwrite (wdata, 'test.dcm');
</pre></div>

<p>Create a dicom file using data and meta info:
</p>
<div class="example">
<pre class="example-preformatted">&gt; wdata = dicomread(file_in_loadpath('imdata/CT-MONO2-16-ankle.dcm');
&gt; info = dicominfo(file_in_loadpath('imdata/CT-MONO2-16-ankle.dcm');
&gt; dicomwrite(wdata, info);
</pre></div>


<p><strong class="strong">See also:</strong> dicomread, dicominfo.
</p></dd></dl>