/*
 * Copyright 2016 Garrett D'Amore <garrett@damore.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/*
 * This file is intended to test the framework.  It also demonstrates
 * some of the capabilities.
 */

#include "convey.h"

#include <stdlib.h>
#include <string.h>


Main({

	/*
	 * The ordering test demonstrates the execution order.
	 * At the end of each inner Convey, we roll back up the stack
	 * to the root, and then start over, bypassing Convey blocks
	 * that have already completed.  Note that if a Convey block
	 * is the last thing in an enclosing Convey, we will make
	 * one more pass all the way through until we bypass that last
	 * item and can close the outer Convey.
	 */
	Test("Ordering", {
		/*
		 * The buffer has to be static because don't want to clear
		 * it with each new pass -- that would defeat our tests!
		 * Note that it starts zeroed (C standard).
		 */
		static char buffer[32];
		static int bufidx;

		Convey("A runs first", { buffer[bufidx++] = 'A'; });
		buffer[bufidx++] = '1';

		Convey("B runs after A", {

			So(strlen(buffer) > 0);
			So(buffer[bufidx-1] == '1');
			buffer[bufidx++] = 'B';

			Convey("C runs inside B", {
				So(buffer[bufidx-1] == 'B');
				buffer[bufidx++] = 'C';
			});
		});

		Convey("D runs afer A, B, C.", {
			So(buffer[bufidx-1] == '1');
			buffer[bufidx++] = 'D';
		});

		buffer[bufidx++] = '2';

		Convey("E is last", {
			So(buffer[bufidx-1] == '2');
			buffer[bufidx++] = 'E';
		});

		So(strcmp(buffer, "A1BC1B1D12E12") == 0);
	});

	Test("Reset", {
		static int x;

		Convey("Initialize X to a non-zero value", {
			So(x == 0);
			x = 1;
			So(x == 1);
		});

		Reset({ x = 20; });

		Convey("Verify that reset did not get called", {
			So(x == 1);
			x = 5;
			So(x == 5);
		});

		Convey("But now it did", {
			So(x == 20);
		});
	});
})