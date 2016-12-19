#
# Simple Makefile
#
# The way the tests work is that you have to compile both your own .c
# (containing your test code) and the test.c file that contains the
# convey framework; you'll also need to have the header files in
# the same directory, or located on the include search path.
#
all:	run_tests

CC	=	cc
PROGS	=	convey_test

run_tests:	$(PROGS)
	for f in $(PROGS); do ./$$f -v; done
	
clean:
	rm $(PROGS)

$(PROGS):	$(@:%=%.c) test.c
	$(CC) -o $@ $(@:%=%.c) test.c

