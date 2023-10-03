#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
### Copyright (C) 2017-2022 John Donoghue <john.donoghue@ieee.org>
###
### This program is free software; you can redistribute it and/or
### modify it under the terms of the GNU General Public License as
### published by the Free Software Foundation; either version 3 of the
### License, or (at your option) any later version.
###
### This program is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
### General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with this program; if not, see
### <http://www.gnu.org/licenses/>.

AC_PREREQ([2.67])
AC_INIT([Octave-Forge dicom package], [0.5.1])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AH_TOP([#include "undef-ah-octave.h"])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET
target_arch="${target_cpu}-${target_os}"

AC_PROG_SED
AC_PROG_GREP
AC_CHECK_PROG([UNIQ],[uniq],[uniq],[cat])
AC_CHECK_PROG([SORT],[sort],[sort],[cat])

AC_CHECK_TOOL([MKOCTFILE], [mkoctfile], [none])
if [ test "x$MKOCTFILE" = "xnone" ]; then
  AC_MSG_ERROR([*** 'mkoctfile' not found.])
fi
AC_CHECK_TOOL([OCTAVE_CONFIG], [octave-config] ,[$MKOCTFILE])

# try get around possible spaces in the path
if test "X${IGNORE_MINGW_PATH_MODIFICATION}" == "X"; then
case $host_os in
  mingw*)
    # try demangle spaces in escaped input strings
    MKOCTFILE=`echo $MKOCTFILE | $SED "s,\\\\\ ,?,g"`
    OCTAVE_CONFIG=`echo $OCTAVE_CONFIG | $SED "s,\\\\\ ,?,g"`
    ;;
  *)
    ;;
esac
fi

AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG(C++)

# check can access std c files
AC_CHECK_HEADERS([string.h])

## octave API tests
save_CXX="$CXX"
save_CXXFLAGS="$CXXFLAGS"
save_LIBS="$LIBS"
save_LDFLAGS="$LDFLAGS"

CXX=`${MKOCTFILE} -p CXX`
OCTINCLUDEDIR="${OCTINCLUDEDIR:-`$MKOCTFILE -p OCTINCLUDEDIR`}/.."
OCTLIBDIR=${OCTLIBDIR:-`$MKOCTFILE -p OCTLIBDIR`}

if test "X${IGNORE_MINGW_PATH_MODIFICATION}" == "X"; then
  MSYSTEM="${MSYSTEM}"
else
  MSYSTEM="none"
fi

case X$MSYSTEM in
  XMINGW64*)
    OCTAVE_HOME=`${MKOCTFILE} -p OCTAVE_HOME | $SED 's,\\\\,/,g'`
    # change \ to / and replace octave home part with mingw part
    OCTINCLUDEDIR=`echo $OCTINCLUDEDIR | $SED -e 's,\\\\,/,g' -e "s,${OCTAVE_HOME},/mingw64,g"`
    OCTLIBDIR=`echo $OCTLIBDIR | $SED -e 's,\\\\,/,g' -e "s,${OCTAVE_HOME},/mingw64,g"`
    ;;
  XMINGW32*)
    OCTAVE_HOME=`${MKOCTFILE} -p OCTAVE_HOME | $SED 's,\\\\,/,g'`
    # change \ to / and replace octave home part with mingw part
    OCTINCLUDEDIR=`echo $OCTINCLUDEDIR | $SED -e 's,\\\\,/,g' -e "s,${OCTAVE_HOME},/mingw32,g"`
    OCTLIBDIR=`echo $OCTLIBDIR | $SED -e 's,\\\\,/,g -e "s,${OCTAVE_HOME},/mingw32,g"'`
    ;;
  *)
    ;;
esac

CXXFLAGS="-I$OCTINCLUDEDIR $CXXFLAGS"
LDFLAGS="-L$OCTLIBDIR $LDFLAGS"
LIBS="-loctinterp $LIBS"

# need to use interpreter->get_load_path in dev version of octave,
# prior to that methods of load_path were static
AC_CACHE_CHECK(
  [interpreter get_load_path],
  [octave_cv_interpreter_get_load_path],
  [AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([
      #include <octave/oct.h>
      #include <octave/octave.h>
      #include <octave/interpreter.h>
      #include <octave/load-path.h>
      ],
      [
        octave::load_path &p = octave::interpreter::the_interpreter ()->get_load_path ();
      ])],
    [octave_cv_interpreter_get_load_path=yes],
    [octave_cv_interpreter_get_load_path=no])
  ])
if test "$octave_cv_interpreter_get_load_path" = "yes" ; then
  AC_DEFINE(HAVE_OCTAVE_LOAD_PATH,[1],[Whether we have the interpreter load path])
fi

OF_OCTAVE_LIST_ALT_SYMS([

[dnl
  [is_map],
  [isstruct],
  [[octave_value ().isstruct ();]],
  [OV_ISMAP],
  [],
  []
],

[dnl
  [is_cell],
  [iscell],
  [[octave_value ().iscell ();]],
  [OV_ISCELL],
  [],
  []
],

[dnl
  [valid_identifier],
  [octave::valid_identifier],
  [[octave::valid_identifier("");]],
  [OCTAVE__VALID_IDENTIFIER],
  [#include <octave/utils.h>],
  [#include <octave/utils.h>]
],

[dnl
  [file_stat],
  [octave::sys::file_stat],
  [[octave::sys::file_stat();]],
  [OCTAVE__FILE_STAT],
  [#include <octave/file-stat.h>],
  [#include <octave/file-stat.h>]
]

],[oct-alt-includes.h])

CXX=$save_CXX
CXXFLAGS=$save_CXXFLAGS
LIBS="$save_LIBS"
LDFLAGS="$save_LDFLAGS"

dnl
dnl GDCM headers are in a version specific path.  They use CMake and
dnl provide a project.cmake config file which has the correct flags to
dnl use.  The CMAKE_FIND_PACKAGE macro is provided by CMake. It has
dnl been slightly changed to make cmake consider architecture
dnl dependent directories.
dnl
CMAKE_FIND_PACKAGE([GDCM], [CXX], , , , [target_arch])

if test "$GDCM_CXXFLAGS$GDCM_LIBS" != "" ; then
   # if have cmake detection, see if can find include files

   save_CPPFLAGS=$CPPFLAGS
   CPPFLAGS="$CPPFLAGS $GDCM_CXXFLAGS"

   AC_CHECK_HEADERS([gdcmVersion.h], ,
     [GDCM_CXXFLAGS=""; GDCM_LIBS=""],
   )

   CPPFLAGS=$save_CPPFLAGS
fi

# newer cmake seems to have issues so if found nothing, and dont have anything set, then try hard way
if test "$GDCM_CXXFLAGS$GDCM_LIBS" == "" ; then
  AC_MSG_NOTICE([Trying fallback GDCM detection])
  #AC_LANG(C++)

  save_LDFLAGS=$LDFLAGS
  save_CPPFLAGS=$CPPFLAGS

  # find headers first
  AC_MSG_CHECKING([for gdcm headers path])
  found_gdcm_header=
  for p in [ /usr/include /usr/local/include /opt/include /mingw32/include /mingw64/include ]; do
    for v in [  gdcm gdcm-2.0 gdcm-2.2 gdcm-2.4 gdcm-2.6 gdcm-2.8 gdcm-3.0 gdcm-3.2]; do

    CPPFLAGS="-I$p/$v $save_CPPFLAGS"
    AC_COMPILE_IFELSE(
      [AC_LANG_PROGRAM([
        #include <gdcmVersion.h>
        ],
        [
          int i=0;
        ])],
      [found_gdcm_header="$p/$v"],
      [])
    done
  done

  if test "$found_gdcm_header" == "" ; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Unable to find GDCM headers]),
  else
    AC_MSG_RESULT([$found_gdcm_header])
    GDCM_CXXFLAGS="-I$found_gdcm_header"
  fi

  # check can use the libraries - later on we add the libs we are going to use to 
  # GDCM_LIBS anyway, so dont need set GDCM_LIBS here
  AC_MSG_CHECKING([for gdcm libraries])
  CPPFLAGS="$GDCM_CXXFLAGS $save_CPPFLAGS"
  LDFLAGS="$LDFLAGS -lgdcmCommon"
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([#include <gdcmVersion.h>], 
      [(void)gdcm::Version::GetBuildVersion()])],
    [AC_MSG_RESULT([yes])], 
    [AC_MSG_RESULT([no])])

  LDFLAGS=$save_LDFLAGS
  CPPFLAGS=$save_CPPFLAGS

fi

## Remove -rdynamics which mkoctfile does not handle.
GDCM_LIBS=$(echo "$GDCM_LIBS" | $SED "s,-rdynamic,,")

GDCM_LIBS="$GDCM_LIBS -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF"

## TODO not needed anymore as cmake settings for gdcm should handle it ?
## or only not needed for clang ?
#case $host_os in
#  darwin*)
#    # Mac needs core framework
#    GDCM_LIBS="$GDCM_LIBS -framework CoreFoundation"
#    ;;
#esac

## set temporarily to search for the headers only
OLD_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $GDCM_CXXFLAGS"

## All the GDCM headers we use (we should probably have something less ugly)
GDCM_HEADERS=$($SED -n 's,^#include.*"\(gdcm[^"]*\)\".*$,\1,p' *.cpp *.h | $SORT | $UNIQ)
AC_CHECK_HEADERS($GDCM_HEADERS, ,
  AC_MSG_ERROR([Unable to find GDCM headers (do you have CMake installed?)]),
)

## restore
CPPFLAGS=$OLD_CPPFLAGS

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
