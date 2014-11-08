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

MKOCTFILE ?= mkoctfile -Wall
OCTAVE ?= octave

INCDIRFLAGS =	-I/usr/local/include/gdcm-2.4
LIBDIRFLAGS =	-L/usr/local/lib
GDCMLIBFLAGS =	-lgdcmcharls  -lgdcmjpeg8 -lgdcmjpeg12 \
-lgdcmjpeg16 -lgdcmCommon \
-lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF  
DEBUGFLAG =	-g

need_dict = dicominfo.oct dicomwrite.oct dicomlookup.oct

## isdicom is different because it's on ..inst/
test_files = dicominfo.cpp dicomdict.cpp dicomread.cpp dicomlookup.cpp

all: $(need_dict) dicomdict.oct dicomread.oct _gendicomdict.oct

%.o: %.cpp
	$(MKOCTFILE) $(INCDIRFLAGS) $(DEBUGFLAG) -c $<

$(need_dict): %.oct: %.cpp dicomdict.o
	$(MKOCTFILE) $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) $(DEBUGFLAG) $^

%.oct: %.cpp
	$(MKOCTFILE) $(INCDIRFLAGS) $(LIBDIRFLAGS) $(GDCMLIBFLAGS) $(DEBUGFLAG) $<

clean:
	rm -f *.o *.oct *~

test:
	for file in $(test_files); do \
		$(OCTAVE) -q --eval "test $$file"; \
	done
	$(OCTAVE) -q --eval "test ../inst/isdicom.m"

