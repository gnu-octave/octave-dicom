---
layout: "default"
permalink: "/functions/11_dicomupdate/"
pkg_name: "dicom"
pkg_version: "0.6.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomupdate"
category: "Dicom Functions"
func_name: "dicomupdate"
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
<dt class="deftypefn" id="index-dicomupdate_0028fileinfo_002c"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var> =</code> <strong class="def-name">dicomupdate(<var class="var">fileinfo</var>,</strong> <code class="def-code-arguments"><var class="var">attribute</var>, <var class="var">value</var>)</code><a class="copiable-link" href="#index-dicomupdate_0028fileinfo_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomupdate_0028info_002c"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var> =</code> <strong class="def-name">dicomupdate(<var class="var">info</var>,</strong> <code class="def-code-arguments"><var class="var">attrinfo</var>)</code><a class="copiable-link" href="#index-dicomupdate_0028info_002c"></a></span></dt>
<dd><p>Update a dicom struct with new values
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">info</var> - dicominfo struct.
</p>
<p><var class="var">attribute</var> - attribute name to find and change value of.
</p>
<p><var class="var">value</var> - attribute value to set.
</p>
<p><var class="var">attrinfo</var> - a table with fields Location and Value for each matched attribute to change.
</p>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">info</var> - dicominfo struct.
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> filename = file_in_loadpath(&quot;imdata/rtstruct.dcm&quot;);
 info = dicominfo(filename);

 % update specific values
 finfo = dicomfind(info, &quot;ROINumber&quot;);
 finfo.Value{1} = 10;
 info = dicomupdate(info, finfo);
 
 % update all matching
 info = dicomupdate(info, &quot;ROINumber&quot;, 100);

 </pre></div>
</dd></dl>