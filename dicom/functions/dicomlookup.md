---
layout: "default"
permalink: "/functions/11_dicomlookup/"
pkg_name: "dicom"
pkg_version: "0.7.0"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomlookup"
category: "Dicom Functions"
func_name: "dicomlookup"
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
<dt class="deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">keyword</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomlookup (<var class="var">group</var>, <var class="var">element</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-element_005d"><span class="category-def">: </span><span><code class="def-type">[<var class="var">group</var>,</code> <strong class="def-name"><var class="var">element</var>]</strong> <code class="def-code-arguments">= dicomlookup (<var class="var">keyword</var>)</code><a class="copiable-link" href="#index-element_005d"></a></span></dt>
<dd>
<p>Lookup an attribute in the DICOM data dictionary.
</p>
<p><var class="var">keyword</var> = dicomlookup (<var class="var">group</var>, <var class="var">element</var>) will look in the current dicom 
dictionary for a specified <var class="var">group</var> and <var class="var">element</var> tag and returns string name 
of the attribute.
</p>
<p>[<var class="var">group</var>, <var class="var">element</var>] = dicomlookup (<var class="var">keyword</var>) will look in the current dicom 
dictionary for a specified <var class="var">keyword</var> string and returns the <var class="var">group</var> and <var class="var">element</var>
 for keyword.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">keyword</var> - string keyword name to look up a group, element value.<br>
<var class="var">group</var> - group value to look up (string or integer).<br>
<var class="var">element</var> - element value to look up (string or integer).<br>
</p>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">keyword</var> - string keyword name to looked up from a group, element value.<br>
<var class="var">group</var>, <var class="var">element</var> - group and element value looked up from keyword.<br>
</p>
<h4 class="subsubheading" id="Outputs-1"><span>Outputs<a class="copiable-link" href="#Outputs-1"></a></span></h4>
<p>Look up tag name for 0x10 0x10:
</p>
<div class="example">
<pre class="example-preformatted">&gt; name = dicomlookup(0x10,0x10)
name = PatientName

</pre></div>

<p>Look up tag group and element value:
</p>
<div class="example">
<pre class="example-preformatted">&gt; [grp, elm] = dicomlookup('TransferSyntaxUID')
grp = 2
elm = 16
</pre></div>

<p><strong class="strong">See also:</strong> dicomdict.
</p></dd></dl>