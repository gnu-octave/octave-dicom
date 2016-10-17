AC_PREREQ([2.67])
AC_INIT([Octave-Forge dicom package], [0.1.1+])

AC_PROG_SED

AC_PROG_CXX
AC_LANG(C++)

dnl
dnl GDCM headers are in a version specific path.  They use CMake and provide
dnl a project.cmake config file which has the correct flags to use.  The
dnl CMAKE_FIND_PACKAGE macro is provided by CMake.
dnl
CMAKE_FIND_PACKAGE([GDCM], [CXX])

## Remove -rdynamics which mkoctfile does not handle.
GDCM_LIBS=$(echo "$GDCM_LIBS" | $SED "s,-rdynamic,,")

GDCM_LIBS="$GDCM_LIBS -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF"

## set temporarily to search for the headers only
OLD_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $GDCM_CXXFLAGS"

## All the GDCM headers we use (we should probably have something less ugly)
GDCM_HEADERS=$(grep -hoP '(?<=^#include ")gdcm.*(?=")' *.cpp *.h)
AC_CHECK_HEADERS($GDCM_HEADERS, ,
  AC_MSG_ERROR([Unable to find GDCM headers (do you have CMake installed?)]),
)

## restore
CPPFLAGS=$OLD_CPPFLAGS

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
