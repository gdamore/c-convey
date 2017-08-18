#!/bin/sh
#
# Copyright 2017 Garrett D'Amore <garrett@damore.org>
#
# This software is supplied under the terms of the MIT License, a
# copy of which should be located in the distribution where this
# file was obtained (LICENSE.txt).  A copy of the license may also be
# found online at https://opensource.org/licenses/MIT.
#

#
# This build script is for building and running the tests.  It knows
# about Linux, MacOS X, and Solaris/illumos. Mostly this is for my
# own testing, and for integrating with CI -- because writing portable
# Makefiles is *hard*, and can't be bothered with CMake for this tiny
# project.
#
# If cc is on your $PATH and a working compiler, the rest should just work.
#

PROG="convey_test"
SRCS="convey_test.c convey.c"
HDRS="convey.h"
OBJS="convey_test.o convey.o"

CC=${CC:-cc}
RM="${RM:-rm -f}"
CFLAGS="${CFLAGS:- -I .} ${EXTRA_CFLAGS}"

case ${UNAME:-`uname`} in
Linux)
	LDLIBS="${LDLIBS:- -lpthread -lrt }"
	;;
SunOS)
	LDLIBS="${LDLIBS:- -lpthread -lrt }"
	;;
Darwin)
	LDLIBS="${LDLIBS:- -lpthread }"
	;;
*)
	echo "Unknown OS, using safe, unthreaded mode."
	# LDLIBS=${LDLIBS:-}
	CFLAGS="${CFLAGS} -DCONVEY_NO_THREAD -DCONVEY_USE_GETTIMEOFDAY"
	;;
esac

build() {
	${CC} ${CFLAGS} -o ${PROG} ${SRCS} ${LDLIBS}
}

test() {
	if [ ! -f ${PROG} ]
	then
		build
	fi
	./${PROG} $*
}

coverage() {
	${CC} -g -O0 -fprofile-arcs -ftest-coverage ${CFLAGS} -o ${PROG} ${SRCS} ${LDLIBS}
	./${PROG} $* || exit 1
	./${PROG} -v || exit 1
	./${PROG} -d || exit 1
	./${PROG} -v -d || exit 1
	./${PROG} -v  || exit 1
	./${PROG} -v >/dev/null || exit 1
	./${PROG} -p NAME=VAR
	env TERM=dumb ./${PROG} -v
}

clean() {
	${RM} ${OBJS} ${PROG}
}

case "$1" in
build|test|coverage|clean)
	fn="$1"
	shift
	${fn} "$*"
	;;

*)
	echo "Usage: $0 <build|test|clean> [args...]\n"
	exit 2
	;;
esac
