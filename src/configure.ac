AC_PREREQ([2.67])
AC_INIT([Octave-Forge dicom package], [0.2.1])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET
target_arch="${target_cpu}-${target_os}"

AC_PROG_SED

AC_PATH_PROG([MKOCTFILE], [mkoctfile])
if test -z "$MKOCTFILE"; then
  AC_MSG_ERROR([*** 'mkoctfile' not found.])
fi

AC_PROG_CXX
AC_LANG(C++)

## octave API tests
save_CXX="$CXX"
save_CXXFLAGS="$CXXFLAGS"
save_LIBS="$LIBS"
save_LDFLAGS="$LDFLAGS"

CXX=`${MKOCTFILE} -p CXX`
OCTINCLUDEDIR=${OCTINCLUDEDIR:-`$MKOCTFILE -p INCFLAGS`}
OCTLIBDIR=${OCTLIBDIR:-`$MKOCTFILE -p OCTLIBDIR`}
CXXFLAGS="$OCTINCLUDEDIR $CXXFLAGS"
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
  [file_stat],
  [octave::sys::file_stat],
  [[octave::sys::file_stat();]],
  [OCTAVE__FILE_STAT],
  [#include <octave/file-stat.h>],
  [#include <octave/file-stat.h>]
]

],[oct-alt-includes.h])

CC=$save_CXX
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

# newer cmake seems to have issues so if found nothing, and dont have anything set, then try hard way
if test "$GDCM_CXXFLAGS$GDCM_LIBS" == "" ; then
  AC_MSG_WARN([No GDCM detected using cmake - trying fallback detection])
  #AC_LANG(C++)

  save_LDFLAGS=$LDFLAGS
  save_CPPFLAGS=$CPPFLAGS

  # find headers first
  AC_MSG_CHECKING([for gdcm headers path])
  found_gdcm_header=
  for p in [ /usr/include /usr/local/include /opt/include ]; do
    for v in [  gdcm gdcm-2.0 gdcm-2.2 gdcm-2.4 gdcm-2.6 gdcm-2.8]; do

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

  LDFLAGS=$saved_LDFLAGS
  CPPFLAGS=$save_CPPFLAGS

fi

## Remove -rdynamics which mkoctfile does not handle.
GDCM_LIBS=$(echo "$GDCM_LIBS" | $SED "s,-rdynamic,,")

GDCM_LIBS="$GDCM_LIBS -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF"

# Mac needs core framework
case $host_os in
  darwin*)
    GDCM_LIBS="$GDCMLIBS -Wl,-framework -Wli,CoreFoundation"
    ;;
esac

## set temporarily to search for the headers only
OLD_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $GDCM_CXXFLAGS"

## All the GDCM headers we use (we should probably have something less ugly)
GDCM_HEADERS=$($SED -n 's,^#include.*"\(gdcm[^"]*\)\".*$,\1,p' *.cpp *.h)
AC_CHECK_HEADERS($GDCM_HEADERS, ,
  AC_MSG_ERROR([Unable to find GDCM headers (do you have CMake installed?)]),
)

## restore
CPPFLAGS=$OLD_CPPFLAGS

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
