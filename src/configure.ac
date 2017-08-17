AC_PREREQ([2.67])
AC_INIT([Octave-Forge dicom package], [0.2.0])
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
