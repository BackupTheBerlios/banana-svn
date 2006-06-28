#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.56)
AC_INIT([banana],[0.1])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
AC_ARG_WITH([opengl], [  --without-opengl        compile without OpenGL support])

AC_DEFUN([AC_CHECK_SDL_HEADER],
	OLD_CFLAGS="${CFLAGS}"
	CFLAGS="${CFLAGS} ${SDL_CFLAGS}"
	AC_MSG_CHECKING([for $1])
	AC_COMPILE_IFELSE(
		[AC_LANG_SOURCE([[#include "$1"]])],
		[AC_MSG_RESULT([yes]) ; $2],
		[AC_MSG_RESULT([no]) ; $3]
		)
	CFLAGS="${OLD_CFLAGS}"
	)

AC_DEFUN([AC_NEEDS],
	OLD_LIBS="${LIBS}"
	LIBS="${LIBS} -l$1 $6"
	[AC_CACHE_CHECK([whether lib$1 needs -l$2], ac_$1_needs_$2,
		AC_TRY_LINK_FUNC($3,
			[ac_$1_needs_$2="no" ; $5],
			[ac_$1_needs_$2="yes" ; $4]
			)
		)]
	LIBS="${OLD_LIBS}"
	)

have_gl="yes"

AM_PATH_SDL([1.2.0], [], [AC_MSG_ERROR([SDL not found or outdated])])

AC_CHECK_SDL_HEADER([SDL_image.h], [], [AC_MSG_ERROR([SDL_image header file not found])]
	)

AC_CHECK_SDL_HEADER([SDL_net.h], [], [AC_MSG_ERROR([SDL_net header file not found])]
	)

AC_CHECK_SDL_HEADER([SDL_opengl.h], [], [have_gl="no"]
	)

AC_CHECK_LIB(m, sqrt, [have_m="yes" M_LIBS="-lm"], [have_m="no"])

AC_CHECK_LIB(z, compress, [have_z="yes" Z_LIBS="-lz"],
	[have_z="no"]
	)

AC_CHECK_LIB(png, png_create_read_struct, [have_png="yes"
	PNG_LIBS="-lpng"], [have_png="no"], [${Z_LIBS} ${M_LIBS}]
	)

if test x"$have_png" = xyes -a x"$have_m" = xyes; then
	AC_NEEDS(png, m, png_create_read_struct,
		[PNG_LIBS="${PNG_LIBS} -lm"], [], [${Z_LIBS}]
		)
fi

if test x"$have_png" = xyes -a x"$have_z" = xyes; then
	AC_NEEDS(png, z, png_create_read_struct,
		[PNG_LIBS="${PNG_LIBS} -lz"], [],
		[${M_LIBS}]
		)
fi

AC_CHECK_LIB(jpeg, jpeg_CreateCompress,
	[have_jpeg="yes" JPEG_LIBS="-ljpeg"], [have_jpeg="no"], []
	)

AC_CHECK_LIB([SDL_image], [IMG_LoadPNG_RW],
	[SDL_IMAGE_LIBS="-lSDL_image"],
	AC_MSG_ERROR([SDL_image not found]),
	[${PNG_LIBS} ${JPEG_LIBS} ${SDL_LIBS}]
	)

if test x"$have_png" = xyes; then
	AC_NEEDS(SDL_image, png, IMG_LoadPNG_RW,
		[SDL_IMAGE_LIBS="${SDL_IMAGE_LIBS} ${PNG_LIBS}"],
		[], [${JPEG_LIBS} ${SDL_LIBS}]
		)
fi

if test x"$have_jpeg" = xyes; then
	AC_NEEDS(SDL_image, jpeg, IMG_LoadPNG_RW,
		[SDL_IMAGE_LIBS="${SDL_IMAGE_LIBS} ${JPEG_LIBS}"],
		[], [${PNG_LIBS} ${SDL_LIBS}]
		)
fi

AC_CHECK_LIB([SDL_net], [SDLNet_Init], 
        [SDL_NET_LIBS="-lSDL_net"], 
        [AC_MSG_ERROR([SDL_net not found])])

if test "$host_os" = "mingw32"; then
	AC_CHECK_LIB(opengl32, main,
		AC_CHECK_LIB(glu32, main,
			[OPENGL_LIBS="-lopengl32 -lglu32"],
			[have_gl="no"]
			),
		[have_gl="no"]
		)
elif test x"$CC" = xkos-cc; then
	AC_CHECK_LIB(gl, glBegin,
		[OPENGL_LIBS="-lgl"],
		[have_gl="no"]
		)
else
	AC_CHECK_LIB(GL, glBegin,
		AC_CHECK_LIB(GLU, gluUnProject,
			[OPENGL_LIBS="-lGL -lGLU"],
			AC_CHECK_LIB(GL, gluUnProject,
				[OPENGL_LIBS="-lGL"],
				[have_gl="no"]
				)
			),
		[have_gl="no"]
		)
fi

LIBS="${SDL_IMAGE_LIBS} ${SDL_NET_LIBS} ${SDL_LIBS}"

if test x"$have_gl" = xyes -a x"$with_opengl" != xno; then
	AC_DEFINE([WITH_OPENGL], [1], [Define to 1 if you want to enable OpenGL support.])
	LIBS="${LIBS} ${OPENGL_LIBS}"
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_VOLATILE

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		src/Makefile
        src/gamegui/Makefile
        src/gamegui/include/Makefile
        src/gamegui/include/gamegui/Makefile])
AC_OUTPUT
