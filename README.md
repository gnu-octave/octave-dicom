Introduction
============

The DICOM package provides Digital communications in medicine (DICOM) 
file io by using the Grassroots DICOM (GDCM) libraries for GNU Octave.

Requirements
============

* libgdcm 2.0.16 or greater


Installing
==========

To install, run the octave package manager:

1. If running Windows, the package may already be installed:
   type pkg list, to view the installed packages.

2. To install from source forge:
   pkg install -forge dicom

3. To install from a local tarball.
   pkg install dicom-XXXXXXX.tar.gz
   Where XXXXXXX is the version of the the downloaded tarball.

Usage:
======

1. Load the package.
   pkg load dicom
   (Required each time Octave is started)

2. Use the function calls dicominfo, dicomread and dicomwrite to read and write DICOM files.

Documentation
==============

See the function list for dicom on [octave forge](https://octave.sourceforge.io/dicom/overview.html) for function documentation.

Known limitations and bugs
==========================

None provided
