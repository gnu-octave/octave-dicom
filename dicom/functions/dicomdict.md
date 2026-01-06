---
layout: "default"
permalink: "/functions/9_dicomdict/"
pkg_name: "dicom"
pkg_version: "0.7.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicomdict"
category: "Dicom Functions"
func_name: "dicomdict"
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
<dt class="deftypefn def-line" id="index-dicomdict"><span class="category-def">: </span><span><code class="def-type"><var class="var">dictionary_name</var> =</code> <strong class="def-name">dicomdict</strong> <code class="def-code-arguments">(<code class="code">&quot;get&quot;</code>)</code><a class="copiable-link" href="#index-dicomdict"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-dicomdict-1"><span class="category-def">: </span><span><strong class="def-name">dicomdict</strong> <code class="def-code-arguments">(<code class="code">&quot;factory&quot;</code>)</code><a class="copiable-link" href="#index-dicomdict-1"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-dicomdict-2"><span class="category-def">: </span><span><strong class="def-name">dicomdict</strong> <code class="def-code-arguments">(<code class="code">&quot;set&quot;</code>, <var class="var">dictionary_name</var>)</code><a class="copiable-link" href="#index-dicomdict-2"></a></span></dt>
<dd>
<p>Get or set the active dicom data dictionary.
</p>
<p>The first usage returns the filename of the dictionary that is currently being used.
Using <code class="code">&quot;factory&quot;</code> resets the dictionary to the default.
Using <code class="code">&quot;set&quot;</code> allows setting the dictionary for future operations.
In this case, the dictionary file <var class="var">dictionary_name</var> can be anywhere in the path.
</p><h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">code</var> -  string value of &rsquo;get&rsquo;, &rsquo;set&rsquo; or &rsquo;factory&rsquo;.
</p>
<p><var class="var">dictionary_name&rsquo;</var> - name of dictionary file to use
</p> 
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">dictionary_name&rsquo;</var> - name of dictionary file currently set for dictionaty
</p> 
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<p>Get current dicom dict path:
</p>
<div class="example">
<pre class="example-preformatted">&gt; f = dicomdict('get')
f = octavedicom.dic
</pre></div>

<p>Set a new dictionary:
</p>
<div class="example">
<pre class="example-preformatted">&gt; dicomdict('set', 'anewdictfile.txt')
</pre></div>


<p><strong class="strong">See also:</strong> dicomread, dicomwrite.
</p></dd></dl>