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
  
 

INCDIRFLAGS =	-I/usr/local/include/gdcm-2.0/
LIBDIRFLAGS =	-L/usr/local/lib
GDCMLIBFLAGS =	-lgdcmcharls -lgdcmzlib -lgdcmjpeg8 -lgdcmjpeg12 \
-lgdcmjpeg16 -lgdcmexpat -lgdcmopenjpeg -lgdcmCommon \
-lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF  


all: dicominfo.oct dicomdict.oct dicomread.oct dicomlookup.oct _gendicomdict.oct


dicomlookup.oct: dicomdict.o dicomlookup.o
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) dicomlookup.o dicomdict.o -o dicomlookup.oct

dicomdict.oct: dicomdict.o
	mkoctfile $(INCDIRFLAGS) dicomdict.o

dicominfo.oct: dicominfo.o dicomdict.o
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) dicominfo.o dicomdict.o -o dicominfo.oct

dicomread.oct: dicomread.cpp
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) dicomread.cpp

_gendicomdict.oct: _gendicomdict.cpp
	mkoctfile $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) _gendicomdict.cpp


dicominfo.o: dicomdict.h dicominfo.cpp
	mkoctfile -c $(INCDIRFLAGS) dicominfo.cpp

dicomdict.o: dicomdict.h dicomdict.cpp
	mkoctfile -c $(INCDIRFLAGS) dicomdict.cpp

dicomlookup.o: dicomdict.h dicomlookup.cpp
	mkoctfile -c $(INCDIRFLAGS) dicomlookup.cpp

clean:
	rm *.o *.oct *~
