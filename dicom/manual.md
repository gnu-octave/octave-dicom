---
layout: "default"
permalink: "/manual/"
title: "Dicom Toolkit - Manual"
pkg_name: "dicom"
version: "0.6.1"
description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
navigation:
- id: "overview"
  name: "Overview"
  url: "/index"
- id: "Functions"
  name: "Function Reference"
  url: "/functions"
- id: "news"
  name: "News"
  url: "/news"
- id: "manual"
  name: "Manual"
- id: "Installing-and-loading-1"
  name: "&nbsp;&nbsp;"
  url: "/manual/#Installing-and-loading-1"
- id: "Basic-Usage-Overview-1"
  name: "&nbsp;&nbsp;"
  url: "/manual/#Basic-Usage-Overview-1"
- id: "Function-Reference-1"
  name: "&nbsp;&nbsp;"
  url: "/manual/#Function-Reference-1"
---
<div class="top-level-extent" id="Top">
<div class="nav-panel">
<p>
Next: <a href="#Installing-and-loading" accesskey="n" rel="next">Installing and loading</a> &nbsp; [<a href="#SEC_Contents" title="Table of contents" rel="contents">Contents</a>][<a href="#Index" title="Index" rel="index">Index</a>]</p>
</div>
<h1 class="top" id="Introduction"><span>Introduction<a class="copiable-link" href="#Introduction"></a></span></h1>
<p>The Dicom toolkit is a set of Dicom file I/O routines for GNU Octave
</p>
<div class="element-contents" id="SEC_Contents">
<h2 class="contents-heading">Table of Contents</h2>
<div class="contents">
<ul class="toc-numbered-mark">
  <li><a id="toc-Installing-and-loading-1" href="#Installing-and-loading">1 Installing and loading</a>
  <ul class="toc-numbered-mark">
    <li><a id="toc-Online-Direct-install" href="#Online-Direct-install">1.1 Online Direct install</a></li>
    <li><a id="toc-Off_002dline-install" href="#Off_002dline-install">1.2 Off-line install</a></li>
    <li><a id="toc-Loading" href="#Loading">1.3 Loading</a></li>
  </ul></li>
  <li><a id="toc-Basic-Usage-Overview-1" href="#Basic-Usage-Overview">2 Basic Usage Overview</a></li>
  <li><a id="toc-Function-Reference-1" href="#Function-Reference">3 Function Reference</a>
  <ul class="toc-numbered-mark">
    <li><a id="toc-Dicom-Functions-1" href="#Dicom-Functions">3.1 Dicom Functions</a>
    <ul class="toc-numbered-mark">
      <li><a id="toc-dicomanon" href="#dicomanon">3.1.1 dicomanon</a></li>
      <li><a id="toc-dicomdict" href="#dicomdict">3.1.2 dicomdict</a></li>
      <li><a id="toc-dicomdisp" href="#dicomdisp">3.1.3 dicomdisp</a></li>
      <li><a id="toc-dicomfind" href="#dicomfind">3.1.4 dicomfind</a></li>
      <li><a id="toc-dicominfo" href="#dicominfo">3.1.5 dicominfo</a></li>
      <li><a id="toc-dicomlookup" href="#dicomlookup">3.1.6 dicomlookup</a></li>
      <li><a id="toc-dicomread" href="#dicomread">3.1.7 dicomread</a></li>
      <li><a id="toc-dicomuid" href="#dicomuid">3.1.8 dicomuid</a></li>
      <li><a id="toc-dicomupdate" href="#dicomupdate">3.1.9 dicomupdate</a></li>
      <li><a id="toc-dicomwrite" href="#dicomwrite">3.1.10 dicomwrite</a></li>
      <li><a id="toc-images_002edicom_002edecodeUID" href="#images_002edicom_002edecodeUID">3.1.11 images.dicom.decodeUID</a></li>
      <li><a id="toc-images_002edicom_002eparseDICOMDIR" href="#images_002edicom_002eparseDICOMDIR">3.1.12 images.dicom.parseDICOMDIR</a></li>
      <li><a id="toc-isdicom" href="#isdicom">3.1.13 isdicom</a></li>
    </ul></li>
  </ul></li>
  <li><a id="toc-GNU-General-Public-License" href="#Copying">Appendix A GNU General Public License</a></li>
  <li><a id="toc-Index-1" href="#Index" rel="index">Index</a></li>
</ul>
</div>
</div>
<hr>
<div class="chapter-level-extent" id="Installing-and-loading">
<div class="nav-panel">
<p>
Next: <a href="#Basic-Usage-Overview" accesskey="n" rel="next">Basic Usage Overview</a>, Previous: <a href="#Top" accesskey="p" rel="prev">Introduction</a>, Up: <a href="#Top" accesskey="u" rel="up">Introduction</a> &nbsp; [<a href="#SEC_Contents" title="Table of contents" rel="contents">Contents</a>][<a href="#Index" title="Index" rel="index">Index</a>]</p>
</div>
<h2 class="chapter" id="Installing-and-loading-1"><span>1 Installing and loading<a class="copiable-link" href="#Installing-and-loading-1"></a></span></h2>
<a class="index-entry-id" id="index-Installing-and-loading"></a>
<p>The Dicom toolkit must be installed and then loaded to be used.
</p>
<p>It can be installed in <abbr class="acronym">GNU</abbr> Octave directly from octave-forge,
or can be installed in an off-line mode via a downloaded tarball.
</p>
<p>The toolkit has a dependency on the GDCM library (<a class="url" href="https://http://gdcm.sourceforge.net/">https://http://gdcm.sourceforge.net/</a>), so it must be installed in order
to successfully install the Dicom toolkit.
</p>
<p>For fedora: <code class="code">yum install gdcm-devel</code> 
</p>
<p>For ubuntu: <code class="code">apt install libgdcm2-dev</code> 
</p>
<p>The toolkit must be then be loaded once per each <abbr class="acronym">GNU</abbr> Octave session in order to use its functionality.
</p>
<ul class="mini-toc">
<li><a href="#Online-Direct-install" accesskey="1">Online Direct install</a></li>
<li><a href="#Off_002dline-install" accesskey="2">Off-line install</a></li>
<li><a href="#Loading" accesskey="3">Loading</a></li>
</ul>
<div class="section-level-extent" id="Online-Direct-install">
<h3 class="section"><span>1.1 Online Direct install<a class="copiable-link" href="#Online-Direct-install"></a></span></h3>
<a class="index-entry-id" id="index-Online-install"></a>
<p>With an internet connection available, the Dicom package can be installed from
octave-forge using the following command within <abbr class="acronym">GNU</abbr> Octave:
</p>
<div class="example">
<pre class="example-preformatted">pkg install -forge dicom
</pre></div>
<p>The latest released version of the toolkit will be downloaded and installed.
</p>
</div>
<div class="section-level-extent" id="Off_002dline-install">
<h3 class="section"><span>1.2 Off-line install<a class="copiable-link" href="#Off_002dline-install"></a></span></h3>
<a class="index-entry-id" id="index-Off_002dline-install"></a>
<p>With the Dicom toolkit package already downloaded, and in the current directory when running
<abbr class="acronym">GNU</abbr> Octave, the package can be installed using the following command within <abbr class="acronym">GNU</abbr> Octave:
</p>
<div class="example">
<pre class="example-preformatted">pkg install dicom-0.6.1.tar.gz
</pre></div>
</div>
<div class="section-level-extent" id="Loading">
<h3 class="section"><span>1.3 Loading<a class="copiable-link" href="#Loading"></a></span></h3>
<a class="index-entry-id" id="index-Loading"></a>
<p>Regardless of the method of installing the Dicom toolkit, in order to use its functions,
the toolkit must be loaded using the pkg load command:
</p>
<div class="example">
<pre class="example-preformatted">pkg load dicom
</pre></div>
<p>The toolkit must be loaded on each <abbr class="acronym">GNU</abbr> Octave session.
</p>
<hr>
</div>
</div>
<div class="chapter-level-extent" id="Basic-Usage-Overview">
<div class="nav-panel">
<p>
Next: <a href="#Function-Reference" accesskey="n" rel="next">Function Reference</a>, Previous: <a href="#Installing-and-loading" accesskey="p" rel="prev">Installing and loading</a>, Up: <a href="#Top" accesskey="u" rel="up">Introduction</a> &nbsp; [<a href="#SEC_Contents" title="Table of contents" rel="contents">Contents</a>][<a href="#Index" title="Index" rel="index">Index</a>]</p>
</div>
<h2 class="chapter" id="Basic-Usage-Overview-1"><span>2 Basic Usage Overview<a class="copiable-link" href="#Basic-Usage-Overview-1"></a></span></h2>
<a class="index-entry-id" id="index-Basic-Usage-Overview"></a>
<p>Dicom files consist of metadata and image data within a file. The Dicom toolkit provides functions to 
read and write dicom data.
</p>
<div class="example">
<pre class="example-preformatted">
%% read the meta information from a dicom file
&gt; info = dicominfo (file_in_loadpath('imdata/simple-test.dcm');
info =
  scalar structure containing the fields:
    Filename = a.dcm
    FileModDate = 04-Feb-2017 02:08:31
    FileMetaInformationVersion =
      0  1
    MediaStorageSOPClassUID = 1.2.840.10008.5.1.4.1.1.7
    MediaStorageSOPInstanceUID = 1.2.826.0.1.3680043.2.1143.4379544382488839209812957878553810312
    TransferSyntaxUID = 1.2.840.10008.1.2
    ImplementationClassUID = 1.2.826.0.1.3680043.2.1143.107.104.103.115.2.6.3
    ImplementationVersionName = GDCM 2.6.3
    SourceApplicationEntityTitle = GDCM
    ImageType = DERIVED \SECONDARY  
    SOPClassUID = 1.2.840.10008.5.1.4.1.1.7
    SOPInstanceUID = 1.2.826.0.1.3680043.2.1143.4379544382488839209812957878553810312
    StudyDate = 20170203
    StudyTime = 210831.360586 
    Modality = OT
    ConversionType = WSD 
    NominalScannedPixelSpacing =
       1
       1
    StudyInstanceUID = 1.2.826.0.1.3680043.2.1143.1282184104726305239898701219563360204
    SeriesInstanceUID = 1.2.826.0.1.3680043.2.1143.9385265081744858155080799316976198629
    SamplesPerPixel = 1
    PhotometricInterpretation = MONOCHROME1 
    Rows = 10
    Columns = 5
    BitsAllocated = 8
    BitsStored = 8
    HighBit = 7
    PixelRepresentation = 0
    RescaleIntercept = 0
    RescaleSlope =  1
    RescaleType = US
    PixelData = not assigned
%% read the image data
&gt; image = dicomread(file_in_loadpath('imdata/simple-test.dcm'))
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
<p>Help for each function can be displayed by  <code class="code">help thefunctionname</code>
</p>
<p>ie: 
</p><div class="example">
<pre class="example-preformatted">help dicominfo
</pre></div>
<hr>
</div>
<div class="chapter-level-extent" id="Function-Reference">
<div class="nav-panel">
<p>
Next: <a href="#Copying" accesskey="n" rel="next">GNU General Public License</a>, Previous: <a href="#Basic-Usage-Overview" accesskey="p" rel="prev">Basic Usage Overview</a>, Up: <a href="#Top" accesskey="u" rel="up">Introduction</a> &nbsp; [<a href="#SEC_Contents" title="Table of contents" rel="contents">Contents</a>][<a href="#Index" title="Index" rel="index">Index</a>]</p>
</div>
<h2 class="chapter" id="Function-Reference-1"><span>3 Function Reference<a class="copiable-link" href="#Function-Reference-1"></a></span></h2>
<a class="index-entry-id" id="index-Function-Reference"></a>
<p>The functions currently available in the Dicom toolkit are described below;
</p>
<ul class="mini-toc">
<li><a href="#Dicom-Functions" accesskey="1">Dicom Functions</a></li>
</ul>
<hr>
<div class="section-level-extent" id="Dicom-Functions">
<div class="nav-panel">
<p>
Up: <a href="#Function-Reference" accesskey="u" rel="up">Function Reference</a> &nbsp; [<a href="#SEC_Contents" title="Table of contents" rel="contents">Contents</a>][<a href="#Index" title="Index" rel="index">Index</a>]</p>
</div>
<h3 class="section" id="Dicom-Functions-1"><span>3.1 Dicom Functions<a class="copiable-link" href="#Dicom-Functions-1"></a></span></h3>
<a class="index-entry-id" id="index-Dicom-Functions"></a>
<ul class="mini-toc">
<li><a href="#dicomanon" accesskey="1">dicomanon</a></li>
<li><a href="#dicomdict" accesskey="2">dicomdict</a></li>
<li><a href="#dicomdisp" accesskey="3">dicomdisp</a></li>
<li><a href="#dicomfind" accesskey="4">dicomfind</a></li>
<li><a href="#dicominfo" accesskey="5">dicominfo</a></li>
<li><a href="#dicomlookup" accesskey="6">dicomlookup</a></li>
<li><a href="#dicomread" accesskey="7">dicomread</a></li>
<li><a href="#dicomuid" accesskey="8">dicomuid</a></li>
<li><a href="#dicomupdate" accesskey="9">dicomupdate</a></li>
<li><a href="#dicomwrite">dicomwrite</a></li>
<li><a href="#images_002edicom_002edecodeUID">images.dicom.decodeUID</a></li>
<li><a href="#images_002edicom_002eparseDICOMDIR">images.dicom.parseDICOMDIR</a></li>
<li><a href="#isdicom">isdicom</a></li>
</ul>
<div class="subsection-level-extent" id="dicomanon">
<h4 class="subsection"><span>3.1.1 dicomanon<a class="copiable-link" href="#dicomanon"></a></span></h4>
<a class="index-entry-id" id="index-dicomanon"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-dicomanon_0028file_005fin_002c"><span class="category-def">: </span><span><strong class="def-name">dicomanon(<var class="var">file_in</var>,</strong> <code class="def-code-arguments"><var class="var">file_out</var>)</code><a class="copiable-link" href="#index-dicomanon_0028file_005fin_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomanon_0028_005f_005f_005f_002c"><span class="category-def">: </span><span><strong class="def-name">dicomanon(___,</strong> <code class="def-code-arguments"><var class="var">name</var>, <var class="var">value</var>)</code><a class="copiable-link" href="#index-dicomanon_0028_005f_005f_005f_002c"></a></span></dt>
<dd>
<p>Anonymize a DICOM format file by removing or replacing specific fields.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">file_in</var> is filename to read from.<br>
<var class="var">file_out</var> is the filename to write to.<br>
<var class="var">name</var>, <var class="var">value</var> optional name/value properties.<br>
</p>
<p>Known property names are:
</p><dl class="table">
<dt>keep</dt>
<dd><p>The value is a cell array of names to not remove during the anonymize procedure.
</p></dd>
<dt>update</dt>
<dd><p>A structure of name/values to update rather than remove.
</p></dd>
</dl>
<h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p>None
</p>
<p><strong class="strong">See also:</strong> dicomread, dicomwrite, dicominfo.
</p></dd></dl>
</div>
<div class="subsection-level-extent" id="dicomdict">
<h4 class="subsection"><span>3.1.2 dicomdict<a class="copiable-link" href="#dicomdict"></a></span></h4>
<a class="index-entry-id" id="index-dicomdict"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-dicomdict-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">dictionary_name</var> =</code> <strong class="def-name">dicomdict</strong> <code class="def-code-arguments">(<code class="code">&quot;get&quot;</code>)</code><a class="copiable-link" href="#index-dicomdict-1"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomdict-2"><span class="category-def">: </span><span><strong class="def-name">dicomdict</strong> <code class="def-code-arguments">(<code class="code">&quot;factory&quot;</code>)</code><a class="copiable-link" href="#index-dicomdict-2"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomdict-3"><span class="category-def">: </span><span><strong class="def-name">dicomdict</strong> <code class="def-code-arguments">(<code class="code">&quot;set&quot;</code>, <var class="var">dictionary_name</var>)</code><a class="copiable-link" href="#index-dicomdict-3"></a></span></dt>
<dd>
<p>Get or set the active dicom data dictionary.
</p>
<p>The first usage returns the filename of the dictionary that is currently being used.
Using <code class="code">&quot;factory&quot;</code> resets the dictionary to the default.
Using <code class="code">&quot;set&quot;</code> allows setting the dictionary for future operations.
In this case, the dictionary file <var class="var">dictionary_name</var> can be anywhere in the path.
</p><h4 class="subsubheading" id="Inputs-1"><span>Inputs<a class="copiable-link" href="#Inputs-1"></a></span></h4>
<p><var class="var">code</var> -  string value of &rsquo;get&rsquo;, &rsquo;set&rsquo; or &rsquo;factory&rsquo;.
</p>
<p><var class="var">dictionary_name&rsquo;</var> - name of dictionary file to use
</p>
<h4 class="subsubheading" id="Outputs-1"><span>Outputs<a class="copiable-link" href="#Outputs-1"></a></span></h4>
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
</div>
<div class="subsection-level-extent" id="dicomdisp">
<h4 class="subsection"><span>3.1.3 dicomdisp<a class="copiable-link" href="#dicomdisp"></a></span></h4>
<a class="index-entry-id" id="index-dicomdisp"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-dicomdisp-1"><span class="category-def">: </span><span><strong class="def-name">dicomdisp</strong> <code class="def-code-arguments">(<var class="var">filename</var>)</code><a class="copiable-link" href="#index-dicomdisp-1"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomdisp-2"><span class="category-def">: </span><span><strong class="def-name">dicomdisp</strong> <code class="def-code-arguments">(<var class="var">filename</var>, [<var class="var">propertyname</var>, <var class="var">propertvalue</var> ...])</code><a class="copiable-link" href="#index-dicomdisp-2"></a></span></dt>
<dd><p>Read and display the metadata from a DICOM file.
</p>
<h4 class="subsubheading" id="Inputs-2"><span>Inputs<a class="copiable-link" href="#Inputs-2"></a></span></h4>
<p><var class="var">filename</var> - dicomfilename to display.<br>
<var class="var">propertyname</var>, <var class="var">propertvalue</var> - property pairs for options to the display function.
</p>
<p>Currently the only known property is &rsquo;dictionary&rsquo; to specify a non default dict to use.
</p><h4 class="subsubheading" id="Outputs-2"><span>Outputs<a class="copiable-link" href="#Outputs-2"></a></span></h4>
<p>None
</p>
<p><strong class="strong">See also:</strong> dicomread, dicominfo.
</p></dd></dl>
</div>
<div class="subsection-level-extent" id="dicomfind">
<h4 class="subsection"><span>3.1.4 dicomfind<a class="copiable-link" href="#dicomfind"></a></span></h4>
<a class="index-entry-id" id="index-dicomfind"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">attrinfo</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomfind(<var class="var">filename</var>, <var class="var">attribute</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-_003d-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">attrinfo</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomfind(<var class="var">info</var>, <var class="var">attribute</var>)</code><a class="copiable-link" href="#index-_003d-1"></a></span></dt>
<dd><p>Find the location and value of an attribute in a dicom file or info structure.
</p>
<h4 class="subsubheading" id="Inputs-3"><span>Inputs<a class="copiable-link" href="#Inputs-3"></a></span></h4>
<p><var class="var">filename</var> - filename to open.
</p>
<p><var class="var">info</var> - dicominfo struct.
</p>
<p><var class="var">attribute</var> - attribute name to find.
</p>
<h4 class="subsubheading" id="Outputs-3"><span>Outputs<a class="copiable-link" href="#Outputs-3"></a></span></h4>
<p><var class="var">attrinfo</var> - a table with fields Location and Value for each matched attribute.
</p>
<p>The Location value will be the attribute position in dot notation to show its position
 in dicom info structure.
</p>
<h4 class="subsubheading" id="Examples-1"><span>Examples<a class="copiable-link" href="#Examples-1"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> filename = file_in_loadpath(&quot;imdata/rtstruct.dcm&quot;);
 info = dicomfind(filename, &quot;ROINumber&quot;);
</pre></div>
</dd></dl>
</div>
<div class="subsection-level-extent" id="dicominfo">
<h4 class="subsection"><span>3.1.5 dicominfo<a class="copiable-link" href="#dicominfo"></a></span></h4>
<a class="index-entry-id" id="index-dicominfo"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-_003d-2"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicominfo (<var class="var">filename</var>)</code><a class="copiable-link" href="#index-_003d-2"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-_003d-3"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicominfo (<var class="var">filename</var>, <code class="code">&quot;dictionary&quot;</code>, <var class="var">dictionary-name</var>)</code><a class="copiable-link" href="#index-_003d-3"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicominfo-1"><span class="category-def">: </span><span><strong class="def-name">dicominfo</strong> <code class="def-code-arguments">(___, <var class="var">options</var>)</code><a class="copiable-link" href="#index-dicominfo-1"></a></span></dt>
<dd><p>Get all metadata from a DICOM file, excluding any actual image. 
<var class="var">info</var> is a nested struct containing the data. 
</p>
<p>If the <code class="code">dictionary</code> argument is used, the given <var class="var">dictionary-name</var> is used for this operation, 
otherwise, the dictionary set by <code class="code">dicomdict</code> is used.
</p>
<h4 class="subsubheading" id="Inputs-4"><span>Inputs<a class="copiable-link" href="#Inputs-4"></a></span></h4>
<p><var class="var">filename</var> - name of file to read.
</p>
<p><var class="var">&rsquo;dictionary&rsquo;</var> - string constant of &rsquo;dictionary&rsquo;.
</p>
<p><var class="var">dictionary-name</var> - filename of dictionary to use.
</p>
<p><var class="var">options</var> - a string in format of &rsquo;optionname=value&rsquo;, or property/value pair &rsquo;optionname&rsquo;, value:
<code class="code">truncate=n</code>
where n is the number of characters to limit the dump output display to <code class="code">n</code>
for each value. 
</p><h4 class="subsubheading" id="Outputs-4"><span>Outputs<a class="copiable-link" href="#Outputs-4"></a></span></h4>
<p><var class="var">info</var> - struct of fields read frome the dicom file.
</p>
<h4 class="subsubheading" id="Examples-2"><span>Examples<a class="copiable-link" href="#Examples-2"></a></span></h4>
<p>Read the metadata of a dicomfile:
</p>
<div class="example">
<pre class="example-preformatted">&gt; info = dicominfo(file_in_loadpath('imdata/simple-test.dcm')
info = 
scalar structure containing the fields:
Filename = a.dcm
FileModDate = 04-Feb-2017 02:08:31
FileMetaInformationVersion =
0  1
MediaStorageSOPClassUID = 1.2.840.10008.5.1.4.1.1.7
MediaStorageSOPInstanceUID = 1.2.826.0.1.3680043.2.1143.4379544382488839209812957878553810312
TransferSyntaxUID = 1.2.840.10008.1.2
ImplementationClassUID = 1.2.826.0.1.3680043.2.1143.107.104.103.115.2.6.3
ImplementationVersionName = GDCM 2.6.3
SourceApplicationEntityTitle = GDCM
ImageType = DERIVED \\SECONDARY  
SOPClassUID = 1.2.840.10008.5.1.4.1.1.7
SOPInstanceUID = 1.2.826.0.1.3680043.2.1143.4379544382488839209812957878553810312
StudyDate = 20170203
StudyTime = 210831.360586 
Modality = OT
ConversionType = WSD 
NominalScannedPixelSpacing =
1
1
StudyInstanceUID = 1.2.826.0.1.3680043.2.1143.1282184104726305239898701219563360204
SeriesInstanceUID = 1.2.826.0.1.3680043.2.1143.9385265081744858155080799316976198629
SamplesPerPixel = 1
PhotometricInterpretation = MONOCHROME1 
Rows = 10
Columns = 5
BitsAllocated = 8
BitsStored = 8
HighBit = 7
PixelRepresentation = 0
RescaleIntercept = 0
RescaleSlope =  1
RescaleType = US
PixelData = not assigned
</pre></div>
<p><strong class="strong">See also:</strong> dicomread, dicomdict.
</p></dd></dl>
</div>
<div class="subsection-level-extent" id="dicomlookup">
<h4 class="subsection"><span>3.1.6 dicomlookup<a class="copiable-link" href="#dicomlookup"></a></span></h4>
<a class="index-entry-id" id="index-dicomlookup"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-_003d-4"><span class="category-def">: </span><span><code class="def-type"><var class="var">keyword</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomlookup (<var class="var">group</var>, <var class="var">element</var>)</code><a class="copiable-link" href="#index-_003d-4"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-element_005d"><span class="category-def">: </span><span><code class="def-type">[<var class="var">group</var>,</code> <strong class="def-name"><var class="var">element</var>]</strong> <code class="def-code-arguments">= dicomlookup (<var class="var">keyword</var>)</code><a class="copiable-link" href="#index-element_005d"></a></span></dt>
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
<h4 class="subsubheading" id="Inputs-5"><span>Inputs<a class="copiable-link" href="#Inputs-5"></a></span></h4>
<p><var class="var">keyword</var> - string keyword name to look up a group, element value.<br>
<var class="var">group</var> - group value to look up (string or integer).<br>
<var class="var">element</var> - element value to look up (string or integer).<br>
</p>
<h4 class="subsubheading" id="Outputs-5"><span>Outputs<a class="copiable-link" href="#Outputs-5"></a></span></h4>
<p><var class="var">keyword</var> - string keyword name to looked up from a group, element value.<br>
<var class="var">group</var>, <var class="var">element</var> - group and element value looked up from keyword.<br>
</p>
<h4 class="subsubheading" id="Outputs-6"><span>Outputs<a class="copiable-link" href="#Outputs-6"></a></span></h4>
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
</div>
<div class="subsection-level-extent" id="dicomread">
<h4 class="subsection"><span>3.1.7 dicomread<a class="copiable-link" href="#dicomread"></a></span></h4>
<a class="index-entry-id" id="index-dicomread"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-_003d-5"><span class="category-def">: </span><span><code class="def-type"><var class="var">image</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomread (<var class="var">filename</var>)</code><a class="copiable-link" href="#index-_003d-5"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-_003d-6"><span class="category-def">: </span><span><code class="def-type"><var class="var">image</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomread (<var class="var">structure</var>)</code><a class="copiable-link" href="#index-_003d-6"></a></span></dt>
<dd>
<p>Load the image from a DICOM file. 
</p><h4 class="subsubheading" id="Inputs-6"><span>Inputs<a class="copiable-link" href="#Inputs-6"></a></span></h4>
<p><var class="var">filename</var> - a string giving the filename.*
</p>
<p><var class="var">structure</var> - a structure with a field <code class="code">Filename</code> (such as returned by <code class="code">dicominfo</code>).
</p>
<p><var class="var">image</var> - may be two or three dimensional, depending on the content of the file. 
</p>
<h4 class="subsubheading" id="Outputs-7"><span>Outputs<a class="copiable-link" href="#Outputs-7"></a></span></h4>
<p><var class="var">image</var> - An integer or float matrix will be returned, the number of bits will depend on the file. 
</p>
<h4 class="subsubheading" id="Examples-3"><span>Examples<a class="copiable-link" href="#Examples-3"></a></span></h4>
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
</div>
<div class="subsection-level-extent" id="dicomuid">
<h4 class="subsection"><span>3.1.8 dicomuid<a class="copiable-link" href="#dicomuid"></a></span></h4>
<a class="index-entry-id" id="index-dicomuid"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-_003d-7"><span class="category-def">: </span><span><code class="def-type"><var class="var">uuid</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicomuid ()</code><a class="copiable-link" href="#index-_003d-7"></a></span></dt>
<dd>
<p>Generate a DICOM unique id . 
</p>
<h4 class="subsubheading" id="Inputs-7"><span>Inputs<a class="copiable-link" href="#Inputs-7"></a></span></h4>
<p>None
</p><h4 class="subsubheading" id="Outputs-8"><span>Outputs<a class="copiable-link" href="#Outputs-8"></a></span></h4>
<p><var class="var">uuid</var> is a unique id string.
</p>
<h4 class="subsubheading" id="Examples-4"><span>Examples<a class="copiable-link" href="#Examples-4"></a></span></h4>
<p>Get a new uid:
</p>
<div class="example">
<pre class="example-preformatted">&gt; uid = dicomuid ()
uid = 1.2.826.0.1.3680043.2.1143.3114589836670200378351641061429967573
</pre></div>
</dd></dl>
</div>
<div class="subsection-level-extent" id="dicomupdate">
<h4 class="subsection"><span>3.1.9 dicomupdate<a class="copiable-link" href="#dicomupdate"></a></span></h4>
<a class="index-entry-id" id="index-dicomupdate"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-dicomupdate_0028fileinfo_002c"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var> =</code> <strong class="def-name">dicomupdate(<var class="var">fileinfo</var>,</strong> <code class="def-code-arguments"><var class="var">attribute</var>, <var class="var">value</var>)</code><a class="copiable-link" href="#index-dicomupdate_0028fileinfo_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomupdate_0028info_002c"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var> =</code> <strong class="def-name">dicomupdate(<var class="var">info</var>,</strong> <code class="def-code-arguments"><var class="var">attrinfo</var>)</code><a class="copiable-link" href="#index-dicomupdate_0028info_002c"></a></span></dt>
<dd><p>Update a dicom struct with new values
</p>
<h4 class="subsubheading" id="Inputs-8"><span>Inputs<a class="copiable-link" href="#Inputs-8"></a></span></h4>
<p><var class="var">info</var> - dicominfo struct.
</p>
<p><var class="var">attribute</var> - attribute name to find and change value of.
</p>
<p><var class="var">value</var> - attribute value to set.
</p>
<p><var class="var">attrinfo</var> - a table with fields Location and Value for each matched attribute to change.
</p>
<h4 class="subsubheading" id="Outputs-9"><span>Outputs<a class="copiable-link" href="#Outputs-9"></a></span></h4>
<p><var class="var">info</var> - dicominfo struct.
</p>
<h4 class="subsubheading" id="Examples-5"><span>Examples<a class="copiable-link" href="#Examples-5"></a></span></h4>
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
</div>
<div class="subsection-level-extent" id="dicomwrite">
<h4 class="subsection"><span>3.1.10 dicomwrite<a class="copiable-link" href="#dicomwrite"></a></span></h4>
<a class="index-entry-id" id="index-dicomwrite"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-dicomwrite_0028im_002c"><span class="category-def">: </span><span><strong class="def-name">dicomwrite(<var class="var">im</var>,</strong> <code class="def-code-arguments"><var class="var">filename</var>)</code><a class="copiable-link" href="#index-dicomwrite_0028im_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-dicomwrite_0028im_002c-1"><span class="category-def">: </span><span><strong class="def-name">dicomwrite(<var class="var">im</var>,</strong> <code class="def-code-arguments"><var class="var">filename</var>, <var class="var">info</var>)</code><a class="copiable-link" href="#index-dicomwrite_0028im_002c-1"></a></span></dt>
<dd>
<p>Write a DICOM format file to <var class="var">filename</var>.
</p>
<h4 class="subsubheading" id="Inputs-9"><span>Inputs<a class="copiable-link" href="#Inputs-9"></a></span></h4>
<p><var class="var">im</var> - image data or empty matrix, [], if only metadata save is required
</p>
<p><var class="var">filename</var> - filename to write dicom to. if [], then function runs in verbose trial mode.
</p>
<p><var class="var">info</var> - struct, like that produced by dicominfo
</p>
<h4 class="subsubheading" id="Examples-6"><span>Examples<a class="copiable-link" href="#Examples-6"></a></span></h4>
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
</div>
<div class="subsection-level-extent" id="images_002edicom_002edecodeUID">
<h4 class="subsection"><span>3.1.11 images.dicom.decodeUID<a class="copiable-link" href="#images_002edicom_002edecodeUID"></a></span></h4>
<a class="index-entry-id" id="index-decodeUID"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-_003d-8"><span class="category-def">: </span><span><code class="def-type"><var class="var">uidinfo</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">images.dicom.decodeUID(<var class="var">uid</var>)</code><a class="copiable-link" href="#index-_003d-8"></a></span></dt>
<dd><p>Look up information about a uid string
</p>
<h4 class="subsubheading" id="Inputs-10"><span>Inputs<a class="copiable-link" href="#Inputs-10"></a></span></h4>
<p><var class="var">uid</var> - dicom uid string.
</p>
<h4 class="subsubheading" id="Outputs-10"><span>Outputs<a class="copiable-link" href="#Outputs-10"></a></span></h4>
<p><var class="var">uidinfo</var> - a structure with fields of Value, Name and Type.
</p>
<p>Additional fields may be present in the case of a Transfer Syntax type uid.
</p>
<h4 class="subsubheading" id="Examples-7"><span>Examples<a class="copiable-link" href="#Examples-7"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> &gt; info = images.dicom.decodeUID(&quot;1.2.840.10008.1.1&quot;);
 info =
  scalar structure containing the fields:
    Value = 1.2.840.10008.1.1
    Name = Verification SOP Class
    Type = SOP Class
</pre></div>
</dd></dl>
</div>
<div class="subsection-level-extent" id="images_002edicom_002eparseDICOMDIR">
<h4 class="subsection"><span>3.1.12 images.dicom.parseDICOMDIR<a class="copiable-link" href="#images_002edicom_002eparseDICOMDIR"></a></span></h4>
<a class="index-entry-id" id="index-parseDICOMDIR"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-_003d-9"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">images.dicom.parseDICOMDIR(<var class="var">filename</var>)</code><a class="copiable-link" href="#index-_003d-9"></a></span></dt>
<dd><p>Parse a DICOMDIR file and return the information as a struct.
</p>
<h4 class="subsubheading" id="Inputs-11"><span>Inputs<a class="copiable-link" href="#Inputs-11"></a></span></h4>
<p><var class="var">filename</var> - filename to open.
</p>
<h4 class="subsubheading" id="Outputs-11"><span>Outputs<a class="copiable-link" href="#Outputs-11"></a></span></h4>
<p><var class="var">info</var> - A struct containing the directory information from a DICOMDIR file
</p>
<p>Th info structure will be an array of Patients, with an array of Studies with an
 array of Series with an array of Images.
</p>
<p>Each element will contain a Payload field.
</p>
<h4 class="subsubheading" id="Examples-8"><span>Examples<a class="copiable-link" href="#Examples-8"></a></span></h4>
<div class="example">
<pre class="example-preformatted"> filename = file_in_loadpath(&quot;imdata/DICOMDIR&quot;);
 info = images.dicom.parseDICOMDIR(filename);
</pre></div>
</dd></dl>
</div>
<div class="subsection-level-extent" id="isdicom">
<h4 class="subsection"><span>3.1.13 isdicom<a class="copiable-link" href="#isdicom"></a></span></h4>
<a class="index-entry-id" id="index-isdicom"></a>
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-isdicom-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">yesno</var> =</code> <strong class="def-name">isdicom</strong> <code class="def-code-arguments">(<var class="var">filename</var>)</code><a class="copiable-link" href="#index-isdicom-1"></a></span></dt>
<dd><p>Return true if <var class="var">filename</var> is a valid DICOM file.
</p>
<h4 class="subsubheading" id="Inputs-12"><span>Inputs<a class="copiable-link" href="#Inputs-12"></a></span></h4>
<p><var class="var">filename</var> - name of file to read.
</p>
<h4 class="subsubheading" id="Outputs-12"><span>Outputs<a class="copiable-link" href="#Outputs-12"></a></span></h4>
<p><var class="var">yesno</var> - logical value of true if filename is a dicom file.
</p>
<p><strong class="strong">See also:</strong> dicomdict, dicominfo, dicomread, dicomwrite.
</p></dd></dl>
<hr>
</div>
</div>
</div>
<div class="appendix-level-extent" id="Copying">
<div class="nav-panel">
<p>
Next: <a href="#Index" accesskey="n" rel="next">Index</a>, Previous: <a href="#Function-Reference" accesskey="p" rel="prev">Function Reference</a>, Up: <a href="#Top" accesskey="u" rel="up">Introduction</a> &nbsp; [<a href="#SEC_Contents" title="Table of contents" rel="contents">Contents</a>][<a href="#Index" title="Index" rel="index">Index</a>]</p>
</div>
<h2 class="appendix" id="GNU-General-Public-License"><span>Appendix A GNU General Public License<a class="copiable-link" href="#GNU-General-Public-License"></a></span></h2>
<a class="index-entry-id" id="index-warranty"></a>
<a class="index-entry-id" id="index-copyright"></a>
<div class="center">Version 3, 29 June 2007
</div>
<div class="display">
<pre class="display-preformatted">Copyright &copy; 2007 Free Software Foundation, Inc. <a class="url" href="http://fsf.org/">http://fsf.org/</a>
Everyone is permitted to copy and distribute verbatim copies of this
license document, but changing it is not allowed.
</pre></div>
<h3 class="heading" id="Preamble"><span>Preamble<a class="copiable-link" href="#Preamble"></a></span></h3>
<p>The GNU General Public License is a free, copyleft license for
software and other kinds of works.
</p>
<p>The licenses for most software and other practical works are designed
to take away your freedom to share and change the works.  By contrast,
the GNU General Public License is intended to guarantee your freedom
to share and change all versions of a program&mdash;to make sure it remains
free software for all its users.  We, the Free Software Foundation,
use the GNU General Public License for most of our software; it
applies also to any other work released this way by its authors.  You
can apply it to your programs, too.
</p>
<p>When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
them if you wish), that you receive source code or can get it if you
want it, that you can change the software or use pieces of it in new
free programs, and that you know you can do these things.
</p>
<p>To protect your rights, we need to prevent others from denying you
these rights or asking you to surrender the rights.  Therefore, you
have certain responsibilities if you distribute copies of the
software, or if you modify it: responsibilities to respect the freedom
of others.
</p>
<p>For example, if you distribute copies of such a program, whether
gratis or for a fee, you must pass on to the recipients the same
freedoms that you received.  You must make sure that they, too,
receive or can get the source code.  And you must show them these
terms so they know their rights.
</p>
<p>Developers that use the GNU GPL protect your rights with two steps:
(1) assert copyright on the software, and (2) offer you this License
giving you legal permission to copy, distribute and/or modify it.
</p>
<p>For the developers&rsquo; and authors&rsquo; protection, the GPL clearly explains
that there is no warranty for this free software.  For both users&rsquo; and
authors&rsquo; sake, the GPL requires that modified versions be marked as
changed, so that their problems will not be attributed erroneously to
authors of previous versions.
</p>
<p>Some devices are designed to deny users access to install or run
modified versions of the software inside them, although the
manufacturer can do so.  This is fundamentally incompatible with the
aim of protecting users&rsquo; freedom to change the software.  The
systematic pattern of such abuse occurs in the area of products for
individuals to use, which is precisely where it is most unacceptable.
Therefore, we have designed this version of the GPL to prohibit the
practice for those products.  If such problems arise substantially in
other domains, we stand ready to extend this provision to those
domains in future versions of the GPL, as needed to protect the
freedom of users.
</p>
<p>Finally, every program is threatened constantly by software patents.
States should not allow patents to restrict development and use of
software on general-purpose computers, but in those that do, we wish
to avoid the special danger that patents applied to a free program
could make it effectively proprietary.  To prevent this, the GPL
assures that patents cannot be used to render the program non-free.
</p>
<p>The precise terms and conditions for copying, distribution and
modification follow.
</p>
<h3 class="heading" id="TERMS-AND-CONDITIONS"><span>TERMS AND CONDITIONS<a class="copiable-link" href="#TERMS-AND-CONDITIONS"></a></span></h3>
<ol class="enumerate" start="0">
<li> Definitions.
<p>&ldquo;This License&rdquo; refers to version 3 of the GNU General Public License.
</p>
<p>&ldquo;Copyright&rdquo; also means copyright-like laws that apply to other kinds
of works, such as semiconductor masks.
</p>
<p>&ldquo;The Program&rdquo; refers to any copyrightable work licensed under this
License.  Each licensee is addressed as &ldquo;you&rdquo;.  &ldquo;Licensees&rdquo; and
&ldquo;recipients&rdquo; may be individuals or organizations.
</p>
<p>To &ldquo;modify&rdquo; a work means to copy from or adapt all or part of the work
in a fashion requiring copyright permission, other than the making of
an exact copy.  The resulting work is called a &ldquo;modified version&rdquo; of
the earlier work or a work &ldquo;based on&rdquo; the earlier work.
</p>
<p>A &ldquo;covered work&rdquo; means either the unmodified Program or a work based
on the Program.
</p>
<p>To &ldquo;propagate&rdquo; a work means to do anything with it that, without
permission, would make you directly or secondarily liable for
infringement under applicable copyright law, except executing it on a
computer or modifying a private copy.  Propagation includes copying,
distribution (with or without modification), making available to the
public, and in some countries other activities as well.
</p>
<p>To &ldquo;convey&rdquo; a work means any kind of propagation that enables other
parties to make or receive copies.  Mere interaction with a user
through a computer network, with no transfer of a copy, is not
conveying.
</p>
<p>An interactive user interface displays &ldquo;Appropriate Legal Notices&rdquo; to
the extent that it includes a convenient and prominently visible
feature that (1) displays an appropriate copyright notice, and (2)
tells the user that there is no warranty for the work (except to the
extent that warranties are provided), that licensees may convey the
work under this License, and how to view a copy of this License.  If
the interface presents a list of user commands or options, such as a
menu, a prominent item in the list meets this criterion.
</p>
</li><li> Source Code.
<p>The &ldquo;source code&rdquo; for a work means the preferred form of the work for
making modifications to it.  &ldquo;Object code&rdquo; means any non-source form
of a work.
</p>
<p>A &ldquo;Standard Interface&rdquo; means an interface that either is an official
standard defined by a recognized standards body, or, in the case of
interfaces specified for a particular programming language, one that
is widely used among developers working in that language.
</p>
<p>The &ldquo;System Libraries&rdquo; of an executable work include anything, other
than the work as a whole, that (a) is included in the normal form of
packaging a Major Component, but which is not part of that Major
Component, and (b) serves only to enable use of the work with that
Major Component, or to implement a Standard Interface for which an
implementation is available to the public in source code form.  A
&ldquo;Major Component&rdquo;, in this context, means a major essential component
(kernel, window system, and so on) of the specific operating system
(if any) on which the executable work runs, or a compiler used to
produce the work, or an object code interpreter used to run it.
</p>
<p>The &ldquo;Corresponding Source&rdquo; for a work in object code form means all
the source code needed to generate, install, and (for an executable
work) run the object code and to modify the work, including scripts to
control those activities.  However, it does not include the work&rsquo;s
System Libraries, or general-purpose tools or generally available free
programs which are used unmodified in performing those activities but
which are not part of the work.  For example, Corresponding Source
includes interface definition files associated with source files for
the work, and the source code for shared libraries and dynamically
linked subprograms that the work is specifically designed to require,
such as by intimate data communication or control flow between those
subprograms and other parts of the work.
</p>
<p>The Corresponding Source need not include anything that users can
regenerate automatically from other parts of the Corresponding Source.
</p>
<p>The Corresponding Source for a work in source code form is that same
work.
</p>
</li><li> Basic Permissions.
<p>All rights granted under this License are granted for the term of
copyright on the Program, and are irrevocable provided the stated
conditions are met.  This License explicitly affirms your unlimited
permission to run the unmodified Program.  The output from running a
covered work is covered by this License only if the output, given its
content, constitutes a covered work.  This License acknowledges your
rights of fair use or other equivalent, as provided by copyright law.
</p>
<p>You may make, run and propagate covered works that you do not convey,
without conditions so long as your license otherwise remains in force.
You may convey covered works to others for the sole purpose of having
them make modifications exclusively for you, or provide you with
facilities for running those works, provided that you comply with the
terms of this License in conveying all material for which you do not
control copyright.  Those thus making or running the covered works for
you must do so exclusively on your behalf, under your direction and
control, on terms that prohibit them from making any copies of your
copyrighted material outside their relationship with you.
</p>
<p>Conveying under any other circumstances is permitted solely under the
conditions stated below.  Sublicensing is not allowed; section 10
makes it unnecessary.
</p>
</li><li> Protecting Users&rsquo; Legal Rights From Anti-Circumvention Law.
<p>No covered work shall be deemed part of an effective technological
measure under any applicable law fulfilling obligations under article
11 of the WIPO copyright treaty adopted on 20 December 1996, or
similar laws prohibiting or restricting circumvention of such
measures.
</p>
<p>When you convey a covered work, you waive any legal power to forbid
circumvention of technological measures to the extent such
circumvention is effected by exercising rights under this License with
respect to the covered work, and you disclaim any intention to limit
operation or modification of the work as a means of enforcing, against
the work&rsquo;s users, your or third parties&rsquo; legal rights to forbid
circumvention of technological measures.
</p>
</li><li> Conveying Verbatim Copies.
<p>You may convey verbatim copies of the Program&rsquo;s source code as you
receive it, in any medium, provided that you conspicuously and
appropriately publish on each copy an appropriate copyright notice;
keep intact all notices stating that this License and any
non-permissive terms added in accord with section 7 apply to the code;
keep intact all notices of the absence of any warranty; and give all
recipients a copy of this License along with the Program.
</p>
<p>You may charge any price or no price for each copy that you convey,
and you may offer support or warranty protection for a fee.
</p>
</li><li> Conveying Modified Source Versions.
<p>You may convey a work based on the Program, or the modifications to
produce it from the Program, in the form of source code under the
terms of section 4, provided that you also meet all of these
conditions:
</p>
<ol class="enumerate" type="a" start="1">
<li> The work must carry prominent notices stating that you modified it,
and giving a relevant date.
</li><li> The work must carry prominent notices stating that it is released
under this License and any conditions added under section 7.  This
requirement modifies the requirement in section 4 to &ldquo;keep intact all
notices&rdquo;.
</li><li> You must license the entire work, as a whole, under this License to
anyone who comes into possession of a copy.  This License will
therefore apply, along with any applicable section 7 additional terms,
to the whole of the work, and all its parts, regardless of how they
are packaged.  This License gives no permission to license the work in
any other way, but it does not invalidate such permission if you have
separately received it.
</li><li> If the work has interactive user interfaces, each must display
Appropriate Legal Notices; however, if the Program has interactive
interfaces that do not display Appropriate Legal Notices, your work
need not make them do so.
</li></ol>
<p>A compilation of a covered work with other separate and independent
works, which are not by their nature extensions of the covered work,
and which are not combined with it such as to form a larger program,
in or on a volume of a storage or distribution medium, is called an
&ldquo;aggregate&rdquo; if the compilation and its resulting copyright are not
used to limit the access or legal rights of the compilation&rsquo;s users
beyond what the individual works permit.  Inclusion of a covered work
in an aggregate does not cause this License to apply to the other
parts of the aggregate.
</p>
</li><li> Conveying Non-Source Forms.
<p>You may convey a covered work in object code form under the terms of
sections 4 and 5, provided that you also convey the machine-readable
Corresponding Source under the terms of this License, in one of these
ways:
</p>
<ol class="enumerate" type="a" start="1">
<li> Convey the object code in, or embodied in, a physical product
(including a physical distribution medium), accompanied by the
Corresponding Source fixed on a durable physical medium customarily
used for software interchange.
</li><li> Convey the object code in, or embodied in, a physical product
(including a physical distribution medium), accompanied by a written
offer, valid for at least three years and valid for as long as you
offer spare parts or customer support for that product model, to give
anyone who possesses the object code either (1) a copy of the
Corresponding Source for all the software in the product that is
covered by this License, on a durable physical medium customarily used
for software interchange, for a price no more than your reasonable
cost of physically performing this conveying of source, or (2) access
to copy the Corresponding Source from a network server at no charge.
</li><li> Convey individual copies of the object code with a copy of the written
offer to provide the Corresponding Source.  This alternative is
allowed only occasionally and noncommercially, and only if you
received the object code with such an offer, in accord with subsection
6b.
</li><li> Convey the object code by offering access from a designated place
(gratis or for a charge), and offer equivalent access to the
Corresponding Source in the same way through the same place at no
further charge.  You need not require recipients to copy the
Corresponding Source along with the object code.  If the place to copy
the object code is a network server, the Corresponding Source may be
on a different server (operated by you or a third party) that supports
equivalent copying facilities, provided you maintain clear directions
next to the object code saying where to find the Corresponding Source.
Regardless of what server hosts the Corresponding Source, you remain
obligated to ensure that it is available for as long as needed to
satisfy these requirements.
</li><li> Convey the object code using peer-to-peer transmission, provided you
inform other peers where the object code and Corresponding Source of
the work are being offered to the general public at no charge under
subsection 6d.
</li></ol>
<p>A separable portion of the object code, whose source code is excluded
from the Corresponding Source as a System Library, need not be
included in conveying the object code work.
</p>
<p>A &ldquo;User Product&rdquo; is either (1) a &ldquo;consumer product&rdquo;, which means any
tangible personal property which is normally used for personal,
family, or household purposes, or (2) anything designed or sold for
incorporation into a dwelling.  In determining whether a product is a
consumer product, doubtful cases shall be resolved in favor of
coverage.  For a particular product received by a particular user,
&ldquo;normally used&rdquo; refers to a typical or common use of that class of
product, regardless of the status of the particular user or of the way
in which the particular user actually uses, or expects or is expected
to use, the product.  A product is a consumer product regardless of
whether the product has substantial commercial, industrial or
non-consumer uses, unless such uses represent the only significant
mode of use of the product.
</p>
<p>&ldquo;Installation Information&rdquo; for a User Product means any methods,
procedures, authorization keys, or other information required to
install and execute modified versions of a covered work in that User
Product from a modified version of its Corresponding Source.  The
information must suffice to ensure that the continued functioning of
the modified object code is in no case prevented or interfered with
solely because modification has been made.
</p>
<p>If you convey an object code work under this section in, or with, or
specifically for use in, a User Product, and the conveying occurs as
part of a transaction in which the right of possession and use of the
User Product is transferred to the recipient in perpetuity or for a
fixed term (regardless of how the transaction is characterized), the
Corresponding Source conveyed under this section must be accompanied
by the Installation Information.  But this requirement does not apply
if neither you nor any third party retains the ability to install
modified object code on the User Product (for example, the work has
been installed in ROM).
</p>
<p>The requirement to provide Installation Information does not include a
requirement to continue to provide support service, warranty, or
updates for a work that has been modified or installed by the
recipient, or for the User Product in which it has been modified or
installed.  Access to a network may be denied when the modification
itself materially and adversely affects the operation of the network
or violates the rules and protocols for communication across the
network.
</p>
<p>Corresponding Source conveyed, and Installation Information provided,
in accord with this section must be in a format that is publicly
documented (and with an implementation available to the public in
source code form), and must require no special password or key for
unpacking, reading or copying.
</p>
</li><li> Additional Terms.
<p>&ldquo;Additional permissions&rdquo; are terms that supplement the terms of this
License by making exceptions from one or more of its conditions.
Additional permissions that are applicable to the entire Program shall
be treated as though they were included in this License, to the extent
that they are valid under applicable law.  If additional permissions
apply only to part of the Program, that part may be used separately
under those permissions, but the entire Program remains governed by
this License without regard to the additional permissions.
</p>
<p>When you convey a copy of a covered work, you may at your option
remove any additional permissions from that copy, or from any part of
it.  (Additional permissions may be written to require their own
removal in certain cases when you modify the work.)  You may place
additional permissions on material, added by you to a covered work,
for which you have or can give appropriate copyright permission.
</p>
<p>Notwithstanding any other provision of this License, for material you
add to a covered work, you may (if authorized by the copyright holders
of that material) supplement the terms of this License with terms:
</p>
<ol class="enumerate" type="a" start="1">
<li> Disclaiming warranty or limiting liability differently from the terms
of sections 15 and 16 of this License; or
</li><li> Requiring preservation of specified reasonable legal notices or author
attributions in that material or in the Appropriate Legal Notices
displayed by works containing it; or
</li><li> Prohibiting misrepresentation of the origin of that material, or
requiring that modified versions of such material be marked in
reasonable ways as different from the original version; or
</li><li> Limiting the use for publicity purposes of names of licensors or
authors of the material; or
</li><li> Declining to grant rights under trademark law for use of some trade
names, trademarks, or service marks; or
</li><li> Requiring indemnification of licensors and authors of that material by
anyone who conveys the material (or modified versions of it) with
contractual assumptions of liability to the recipient, for any
liability that these contractual assumptions directly impose on those
licensors and authors.
</li></ol>
<p>All other non-permissive additional terms are considered &ldquo;further
restrictions&rdquo; within the meaning of section 10.  If the Program as you
received it, or any part of it, contains a notice stating that it is
governed by this License along with a term that is a further
restriction, you may remove that term.  If a license document contains
a further restriction but permits relicensing or conveying under this
License, you may add to a covered work material governed by the terms
of that license document, provided that the further restriction does
not survive such relicensing or conveying.
</p>
<p>If you add terms to a covered work in accord with this section, you
must place, in the relevant source files, a statement of the
additional terms that apply to those files, or a notice indicating
where to find the applicable terms.
</p>
<p>Additional terms, permissive or non-permissive, may be stated in the
form of a separately written license, or stated as exceptions; the
above requirements apply either way.
</p>
</li><li> Termination.
<p>You may not propagate or modify a covered work except as expressly
provided under this License.  Any attempt otherwise to propagate or
modify it is void, and will automatically terminate your rights under
this License (including any patent licenses granted under the third
paragraph of section 11).
</p>
<p>However, if you cease all violation of this License, then your license
from a particular copyright holder is reinstated (a) provisionally,
unless and until the copyright holder explicitly and finally
terminates your license, and (b) permanently, if the copyright holder
fails to notify you of the violation by some reasonable means prior to
60 days after the cessation.
</p>
<p>Moreover, your license from a particular copyright holder is
reinstated permanently if the copyright holder notifies you of the
violation by some reasonable means, this is the first time you have
received notice of violation of this License (for any work) from that
copyright holder, and you cure the violation prior to 30 days after
your receipt of the notice.
</p>
<p>Termination of your rights under this section does not terminate the
licenses of parties who have received copies or rights from you under
this License.  If your rights have been terminated and not permanently
reinstated, you do not qualify to receive new licenses for the same
material under section 10.
</p>
</li><li> Acceptance Not Required for Having Copies.
<p>You are not required to accept this License in order to receive or run
a copy of the Program.  Ancillary propagation of a covered work
occurring solely as a consequence of using peer-to-peer transmission
to receive a copy likewise does not require acceptance.  However,
nothing other than this License grants you permission to propagate or
modify any covered work.  These actions infringe copyright if you do
not accept this License.  Therefore, by modifying or propagating a
covered work, you indicate your acceptance of this License to do so.
</p>
</li><li> Automatic Licensing of Downstream Recipients.
<p>Each time you convey a covered work, the recipient automatically
receives a license from the original licensors, to run, modify and
propagate that work, subject to this License.  You are not responsible
for enforcing compliance by third parties with this License.
</p>
<p>An &ldquo;entity transaction&rdquo; is a transaction transferring control of an
organization, or substantially all assets of one, or subdividing an
organization, or merging organizations.  If propagation of a covered
work results from an entity transaction, each party to that
transaction who receives a copy of the work also receives whatever
licenses to the work the party&rsquo;s predecessor in interest had or could
give under the previous paragraph, plus a right to possession of the
Corresponding Source of the work from the predecessor in interest, if
the predecessor has it or can get it with reasonable efforts.
</p>
<p>You may not impose any further restrictions on the exercise of the
rights granted or affirmed under this License.  For example, you may
not impose a license fee, royalty, or other charge for exercise of
rights granted under this License, and you may not initiate litigation
(including a cross-claim or counterclaim in a lawsuit) alleging that
any patent claim is infringed by making, using, selling, offering for
sale, or importing the Program or any portion of it.
</p>
</li><li> Patents.
<p>A &ldquo;contributor&rdquo; is a copyright holder who authorizes use under this
License of the Program or a work on which the Program is based.  The
work thus licensed is called the contributor&rsquo;s &ldquo;contributor version&rdquo;.
</p>
<p>A contributor&rsquo;s &ldquo;essential patent claims&rdquo; are all patent claims owned
or controlled by the contributor, whether already acquired or
hereafter acquired, that would be infringed by some manner, permitted
by this License, of making, using, or selling its contributor version,
but do not include claims that would be infringed only as a
consequence of further modification of the contributor version.  For
purposes of this definition, &ldquo;control&rdquo; includes the right to grant
patent sublicenses in a manner consistent with the requirements of
this License.
</p>
<p>Each contributor grants you a non-exclusive, worldwide, royalty-free
patent license under the contributor&rsquo;s essential patent claims, to
make, use, sell, offer for sale, import and otherwise run, modify and
propagate the contents of its contributor version.
</p>
<p>In the following three paragraphs, a &ldquo;patent license&rdquo; is any express
agreement or commitment, however denominated, not to enforce a patent
(such as an express permission to practice a patent or covenant not to
sue for patent infringement).  To &ldquo;grant&rdquo; such a patent license to a
party means to make such an agreement or commitment not to enforce a
patent against the party.
</p>
<p>If you convey a covered work, knowingly relying on a patent license,
and the Corresponding Source of the work is not available for anyone
to copy, free of charge and under the terms of this License, through a
publicly available network server or other readily accessible means,
then you must either (1) cause the Corresponding Source to be so
available, or (2) arrange to deprive yourself of the benefit of the
patent license for this particular work, or (3) arrange, in a manner
consistent with the requirements of this License, to extend the patent
license to downstream recipients.  &ldquo;Knowingly relying&rdquo; means you have
actual knowledge that, but for the patent license, your conveying the
covered work in a country, or your recipient&rsquo;s use of the covered work
in a country, would infringe one or more identifiable patents in that
country that you have reason to believe are valid.
</p>
<p>If, pursuant to or in connection with a single transaction or
arrangement, you convey, or propagate by procuring conveyance of, a
covered work, and grant a patent license to some of the parties
receiving the covered work authorizing them to use, propagate, modify
or convey a specific copy of the covered work, then the patent license
you grant is automatically extended to all recipients of the covered
work and works based on it.
</p>
<p>A patent license is &ldquo;discriminatory&rdquo; if it does not include within the
scope of its coverage, prohibits the exercise of, or is conditioned on
the non-exercise of one or more of the rights that are specifically
granted under this License.  You may not convey a covered work if you
are a party to an arrangement with a third party that is in the
business of distributing software, under which you make payment to the
third party based on the extent of your activity of conveying the
work, and under which the third party grants, to any of the parties
who would receive the covered work from you, a discriminatory patent
license (a) in connection with copies of the covered work conveyed by
you (or copies made from those copies), or (b) primarily for and in
connection with specific products or compilations that contain the
covered work, unless you entered into that arrangement, or that patent
license was granted, prior to 28 March 2007.
</p>
<p>Nothing in this License shall be construed as excluding or limiting
any implied license or other defenses to infringement that may
otherwise be available to you under applicable patent law.
</p>
</li><li> No Surrender of Others&rsquo; Freedom.
<p>If conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot convey
a covered work so as to satisfy simultaneously your obligations under
this License and any other pertinent obligations, then as a
consequence you may not convey it at all.  For example, if you agree
to terms that obligate you to collect a royalty for further conveying
from those to whom you convey the Program, the only way you could
satisfy both those terms and this License would be to refrain entirely
from conveying the Program.
</p>
</li><li> Use with the GNU Affero General Public License.
<p>Notwithstanding any other provision of this License, you have
permission to link or combine any covered work with a work licensed
under version 3 of the GNU Affero General Public License into a single
combined work, and to convey the resulting work.  The terms of this
License will continue to apply to the part which is the covered work,
but the special requirements of the GNU Affero General Public License,
section 13, concerning interaction through a network will apply to the
combination as such.
</p>
</li><li> Revised Versions of this License.
<p>The Free Software Foundation may publish revised and/or new versions
of the GNU General Public License from time to time.  Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.
</p>
<p>Each version is given a distinguishing version number.  If the Program
specifies that a certain numbered version of the GNU General Public
License &ldquo;or any later version&rdquo; applies to it, you have the option of
following the terms and conditions either of that numbered version or
of any later version published by the Free Software Foundation.  If
the Program does not specify a version number of the GNU General
Public License, you may choose any version ever published by the Free
Software Foundation.
</p>
<p>If the Program specifies that a proxy can decide which future versions
of the GNU General Public License can be used, that proxy&rsquo;s public
statement of acceptance of a version permanently authorizes you to
choose that version for the Program.
</p>
<p>Later license versions may give you additional or different
permissions.  However, no additional obligations are imposed on any
author or copyright holder as a result of your choosing to follow a
later version.
</p>
</li><li> Disclaimer of Warranty.
<p>THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY
APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM &ldquo;AS IS&rdquo; WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND
PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
CORRECTION.
</p>
</li><li> Limitation of Liability.
<p>IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR
CONVEYS THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT
NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR
LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM
TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER
PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
</p>
</li><li> Interpretation of Sections 15 and 16.
<p>If the disclaimer of warranty and limitation of liability provided
above cannot be given local legal effect according to their terms,
reviewing courts shall apply local law that most closely approximates
an absolute waiver of all civil liability in connection with the
Program, unless a warranty or assumption of liability accompanies a
copy of the Program in return for a fee.
</p>
</li></ol>
<h3 class="heading" id="END-OF-TERMS-AND-CONDITIONS"><span>END OF TERMS AND CONDITIONS<a class="copiable-link" href="#END-OF-TERMS-AND-CONDITIONS"></a></span></h3>
<h3 class="heading" id="How-to-Apply-These-Terms-to-Your-New-Programs"><span>How to Apply These Terms to Your New Programs<a class="copiable-link" href="#How-to-Apply-These-Terms-to-Your-New-Programs"></a></span></h3>
<p>If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these
terms.
</p>
<p>To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
state the exclusion of warranty; and each file should have at least
the &ldquo;copyright&rdquo; line and a pointer to where the full notice is found.
</p>
<div class="example smallexample">
<pre class="example-preformatted"><var class="var">one line to give the program's name and a brief idea of what it does.</var>  
Copyright (C) <var class="var">year</var> <var class="var">name of author</var>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <a class="url" href="http://www.gnu.org/licenses/">http://www.gnu.org/licenses/</a>.
</pre></div>
<p>Also add information on how to contact you by electronic and paper mail.
</p>
<p>If the program does terminal interaction, make it output a short
notice like this when it starts in an interactive mode:
</p>
<div class="example smallexample">
<pre class="example-preformatted"><var class="var">program</var> Copyright (C) <var class="var">year</var> <var class="var">name of author</var> 
This program comes with ABSOLUTELY NO WARRANTY; for details type &lsquo;<samp class="samp">show w</samp>&rsquo;.
This is free software, and you are welcome to redistribute it
under certain conditions; type &lsquo;<samp class="samp">show c</samp>&rsquo; for details.
</pre></div>
<p>The hypothetical commands &lsquo;<samp class="samp">show w</samp>&rsquo; and &lsquo;<samp class="samp">show c</samp>&rsquo; should show
the appropriate parts of the General Public License.  Of course, your
program&rsquo;s commands might be different; for a GUI interface, you would
use an &ldquo;about box&rdquo;.
</p>
<p>You should also get your employer (if you work as a programmer) or school,
if any, to sign a &ldquo;copyright disclaimer&rdquo; for the program, if necessary.
For more information on this, and how to apply and follow the GNU GPL, see
<a class="url" href="http://www.gnu.org/licenses/">http://www.gnu.org/licenses/</a>.
</p>
<p>The GNU General Public License does not permit incorporating your
program into proprietary programs.  If your program is a subroutine
library, you may consider it more useful to permit linking proprietary
applications with the library.  If this is what you want to do, use
the GNU Lesser General Public License instead of this License.  But
first, please read <a class="url" href="http://www.gnu.org/philosophy/why-not-lgpl.html">http://www.gnu.org/philosophy/why-not-lgpl.html</a>.
</p>
<hr>
</div>
<div class="unnumbered-level-extent" id="Index">
<div class="nav-panel">
<p>
Previous: <a href="#Copying" accesskey="p" rel="prev">GNU General Public License</a>, Up: <a href="#Top" accesskey="u" rel="up">Introduction</a> &nbsp; [<a href="#SEC_Contents" title="Table of contents" rel="contents">Contents</a>][<a href="#Index" title="Index" rel="index">Index</a>]</p>
</div>
<h2 class="unnumbered" id="Index-1"><span>Index<a class="copiable-link" href="#Index-1"></a></span></h2>
 
<div class="printindex cp-printindex">
<table class="cp-letters-header-printindex"><tr><th>Jump to: &nbsp; </th><td><a class="summary-letter-printindex" href="#Index_cp_letter-B"><b>B</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-C"><b>C</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-D"><b>D</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-F"><b>F</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-I"><b>I</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-L"><b>L</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-O"><b>O</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-P"><b>P</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-W"><b>W</b></a>
 &nbsp; 
</td></tr></table>
<table class="cp-entries-printindex" border="0">
<tr><td></td><th class="entries-header-printindex">Index Entry</th><th class="sections-header-printindex">Section</th></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-B">B</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-Basic-Usage-Overview">Basic Usage Overview</a></td><td class="printindex-index-section"><a href="#Basic-Usage-Overview">Basic Usage Overview</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-C">C</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-copyright">copyright</a></td><td class="printindex-index-section"><a href="#Copying">Copying</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-D">D</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-decodeUID">decodeUID</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-Dicom-Functions">Dicom Functions</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomanon">dicomanon</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomdict">dicomdict</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomdisp">dicomdisp</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomfind">dicomfind</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicominfo">dicominfo</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomlookup">dicomlookup</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomread">dicomread</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomuid">dicomuid</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomupdate">dicomupdate</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-dicomwrite">dicomwrite</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-F">F</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-Function-Reference">Function Reference</a></td><td class="printindex-index-section"><a href="#Function-Reference">Function Reference</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-I">I</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-Installing-and-loading">Installing and loading</a></td><td class="printindex-index-section"><a href="#Installing-and-loading">Installing and loading</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-isdicom">isdicom</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-L">L</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-Loading">Loading</a></td><td class="printindex-index-section"><a href="#Installing-and-loading">Installing and loading</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-O">O</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-Off_002dline-install">Off-line install</a></td><td class="printindex-index-section"><a href="#Installing-and-loading">Installing and loading</a></td></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-Online-install">Online install</a></td><td class="printindex-index-section"><a href="#Installing-and-loading">Installing and loading</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-P">P</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-parseDICOMDIR">parseDICOMDIR</a></td><td class="printindex-index-section"><a href="#Dicom-Functions">Dicom Functions</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
<tr><th id="Index_cp_letter-W">W</th></tr>
<tr><td></td><td class="printindex-index-entry"><a href="#index-warranty">warranty</a></td><td class="printindex-index-section"><a href="#Copying">Copying</a></td></tr>
<tr><td colspan="3"><hr></td></tr>
</table>
<table class="cp-letters-footer-printindex"><tr><th>Jump to: &nbsp; </th><td><a class="summary-letter-printindex" href="#Index_cp_letter-B"><b>B</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-C"><b>C</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-D"><b>D</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-F"><b>F</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-I"><b>I</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-L"><b>L</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-O"><b>O</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-P"><b>P</b></a>
 &nbsp; 
<a class="summary-letter-printindex" href="#Index_cp_letter-W"><b>W</b></a>
 &nbsp; 
</td></tr></table>
</div>
 
</div>
</div>
