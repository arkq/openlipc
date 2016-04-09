# [open]lipc - configure.ac
# Copyright (c) 2016 Arkadiusz Bokowy

AC_INIT([openlipc], [0.1.0], [arkadiusz.bokowy@gmail.com])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AM_PROG_CC_C_O


AC_ARG_VAR([KINDLE_ROOTDIR], [directory containing Kindle root tree])
AC_ARG_ENABLE([kindle-env],
	[AS_HELP_STRING([--enable-kindle-env[[=DIR]]], [use Kindle environment])], [
		if test "x$enableval" != "xno"; then enable_kindle_env=yes; fi
		if test "x$enableval" != "xno" -a "x$enableval" != "xyes"; then
			AC_SUBST([KINDLE_ROOTDIR], [$enableval])
		fi
	])
AM_CONDITIONAL([ENABLE_KINDLE_ENV], [test "x$enable_kindle_env" = "xyes"])
AM_COND_IF([ENABLE_KINDLE_ENV], [
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#ifndef __arm__
		error: Not an ARM compiler!
		#endif]])], [],
		[AC_MSG_FAILURE([ARM-compatible compiler is required])])
	if test "x$KINDLE_ROOTDIR" = "x"; then
		AC_MSG_WARN([Kindle environment enabled, but Kindle root tree location not given!])
		AC_MSG_WARN([Either set the KINDLE_ROOTDIR environmental variable or set the root])
		AC_MSG_WARN([tree location with the --enable-kindle-env option.])
	fi
])


AC_ARG_WITH([lipc-xxx-prop],
	[AS_HELP_STRING([--without-lipc-prop], [omit lipc-get/set-prop replacements])],
	[], [with_lipc_xxx_prop=yes])
AM_CONDITIONAL([WITH_LIPC_XXX_PROP], [test "x$with_lipc_xxx_prop" = "xyes"])

AC_ARG_WITH([lipc-probe],
	[AS_HELP_STRING([--without-lipc-probe], [omit lipc-probe replacement])],
	[], [with_lipc_probe=yes])
AM_CONDITIONAL([WITH_LIPC_PROBE], [test "x$with_lipc_probe" = "xyes"])
AM_COND_IF([WITH_LIPC_PROBE], [
	PKG_CHECK_MODULES([GLIB20], [glib-2.0])
	PKG_CHECK_MODULES([GIO20], [gio-2.0])
])


AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
