---
layout: "default"
permalink: "/functions/9_dicominfo/"
pkg_name: "dicom"
pkg_version: "0.7.1"
pkg_description: "Digital communications in medicine (DICOM) file io.   Depends on Grassroots DICOM (GDCM). This package is not available   in repos for all distros."
title: "Dicom Toolkit - dicominfo"
category: "Dicom Functions"
func_name: "dicominfo"
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
<dt class="deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicominfo (<var class="var">filename</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-_003d-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">info</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">dicominfo (<var class="var">filename</var>, <code class="code">&quot;dictionary&quot;</code>, <var class="var">dictionary-name</var>)</code><a class="copiable-link" href="#index-_003d-1"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-dicominfo"><span class="category-def">: </span><span><strong class="def-name">dicominfo</strong> <code class="def-code-arguments">(___, <var class="var">options</var>)</code><a class="copiable-link" href="#index-dicominfo"></a></span></dt>
<dd><p>Get all metadata from a DICOM file, excluding any actual image. 
<var class="var">info</var> is a nested struct containing the data. 
</p>
<p>If the <code class="code">dictionary</code> argument is used, the given <var class="var">dictionary-name</var> is used for this operation, 
otherwise, the dictionary set by <code class="code">dicomdict</code> is used.
</p>
<h4 class="subsubheading" id="Inputs"><span>Inputs<a class="copiable-link" href="#Inputs"></a></span></h4>
<p><var class="var">filename</var> - name of file to read.
</p>
<p><var class="var">&rsquo;dictionary&rsquo;</var> - string constant of &rsquo;dictionary&rsquo;.
</p>
<p><var class="var">dictionary-name</var> - filename of dictionary to use.
</p>
<p><var class="var">options</var> - a string in format of &rsquo;optionname=value&rsquo;, or property/value pair &rsquo;optionname&rsquo;, value:
<code class="code">truncate=n</code>
where n is the number of characters to limit the dump output display to <code class="code">n</code>for each value. 
</p><h4 class="subsubheading" id="Outputs"><span>Outputs<a class="copiable-link" href="#Outputs"></a></span></h4>
<p><var class="var">info</var> - struct of fields read frome the dicom file.
</p>
<h4 class="subsubheading" id="Examples"><span>Examples<a class="copiable-link" href="#Examples"></a></span></h4>
<p>Read the metadata of a dicomfile:
</p>
<div class="example">
<pre class="example-preformatted"> &gt; info = dicominfo(file_in_loadpath('imdata/simple-test.dcm')
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
 </pre></div>
 

<p><strong class="strong">See also:</strong> dicomread, dicomdict.
</p></dd></dl>