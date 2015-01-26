#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_CONFIG_MACRO_DIR([m4])
AC_INIT([mopo], [0.1], [littleioaudio@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_SILENT_RULES(yes)
AC_CONFIG_SRCDIR([src/delay.cpp])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_CXXCPP
AM_PROG_AR
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stddef.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([dup2 memset modf pow rmdir strcasecmp strchr strdup strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
