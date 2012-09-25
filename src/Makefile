#   The GNU Octave dicom package is Copyright Andy Buckle 2010
#   Contact: blondandy using the sf.net system, 
#   <https://sourceforge.net/sendmessage.php?touser=1760416>
#   
#   The GNU Octave dicom package is free software: you can redistribute 
#   it and/or modify it under the terms of the GNU General Public 
#   License as published by the Free Software Foundation, either 
#   version 3 of the License, or (at your option) any later version.
#   
#   The GNU Octave dicom packag is distributed in the hope that it 
#   will be useful, but WITHOUT ANY WARRANTY; without even the 
#   implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
#   PURPOSE.  See the GNU General Public License for more details.
#   
#   Please see the file, "COPYING" for further details of GNU General 
#   Public License version 3.
  
 

INCDIRFLAGS =	
LIBDIRFLAGS =	-L/usr/local/lib
GDCMLIBFLAGS =	-lgdcmcharls  -lgdcmjpeg8 -lgdcmjpeg12 \
-lgdcmjpeg16 -lgdcmCommon \
-lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF  
DEBUGFLAG =	-g


all: dicominfo.oct dicomdict.oct dicomread.oct dicomlookup.oct _gendicomdict.oct dicomwrite.oct


dicomlookup.oct: dicomdict.o dicomlookup.o
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) $(DEBUGFLAG) dicomlookup.o dicomdict.o -o dicomlookup.oct

dicomdict.oct: dicomdict.o
	mkoctfile $(INCDIRFLAGS) $(DEBUGFLAG) dicomdict.o

dicominfo.oct: dicominfo.o dicomdict.o
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) $(DEBUGFLAG) dicominfo.o dicomdict.o -o dicominfo.oct

dicomwrite.oct: dicomwrite.o dicomdict.o
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) $(DEBUGFLAG) dicomwrite.o dicomdict.o -o dicomwrite.oct

dicomread.oct: dicomread.cpp
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) $(DEBUGFLAG) dicomread.cpp

_gendicomdict.oct: _gendicomdict.cpp
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) $(DEBUGFLAG) _gendicomdict.cpp


dicominfo.o: dicomdict.h dicominfo.cpp
	mkoctfile -c $(INCDIRFLAGS) $(DEBUGFLAG) dicominfo.cpp

dicomwrite.o: dicomdict.h dicomwrite.cpp
	mkoctfile -c $(INCDIRFLAGS) $(DEBUGFLAG) dicomwrite.cpp

dicomdict.o: dicomdict.h dicomdict.cpp
	mkoctfile -c $(INCDIRFLAGS) $(DEBUGFLAG) dicomdict.cpp

dicomlookup.o: dicomdict.h dicomlookup.cpp
	mkoctfile -c $(INCDIRFLAGS) $(DEBUGFLAG) dicomlookup.cpp

clean:
	rm -f *.o *.oct *~

test: test_dicominfo test_dicomdict test_dicomread test_dicomlookup test_isdicom
	
test_dicominfo: dicominfo.oct
	octave -q --eval "test dicominfo.cpp"

test_dicomdict: dicomdict.oct
	octave -q --eval "test dicomdict.cpp"

test_dicomread: dicomread.oct
	octave -q --eval "test dicomread.cpp"

test_dicomlookup: dicomlookup.oct
	octave -q --eval "test dicomlookup.cpp"

test_isdicom: 
	octave -q --eval "test ../isdicom.m"
