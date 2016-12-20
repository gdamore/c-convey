@echo off
rem Copyright 2016 Garrett D'Amore <garrett@damore.org>
rem
rem This software is supplied under the terms of the MIT License, a
rem copy of which should be located in the distribution where this
rem file was obtained (LICENSE.txt).  A copy of the license may also be
rem found online at https://opensource.org/licenses/MIT.

rem This build script is for Win32 systems.  You can also use the
rem Makefile.win32.  You need to set up your environment to include the
rem PATH for cl.exe, as well as anything else it needs.  Just run
rem vsvars32.bat from Visual Studio first.

set PROG=convey_test.exe
set SRCS=convey_test.c convey.c
set OBJS=convey_test.obj convey.obj
set HDRS=convey.h

set CC=cl
set RM=del
set CFLAGS=/I .
set LDLIBS=

if "%1" == "build" (
	echo %CC% %CFLAGS% /Fe%PROG% %SRCS% %LDLIBS%
	%CC% %CFLAGS% /Fe%PROG% %SRCS% %LDLIBS%
) else if "%1" == "test" (
	echo %PROG% %2 %3 %4 %5 %6 %7 %8 %9
	%PROG% %2 %3 %4 %5 %6 %7 %8 %9
) else if "%1" == "clean" (
	echo %RM% %OBJS% %PROG%
	%RM% %OBJS% %PROG%
) else (
	echo "Usage: %0 <build|test|clean> [args...]"
)
