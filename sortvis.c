/*
 *	SORTVIS.C
 *	---------
 *	Sort algorithms visualization
 *	Version 0.5.0
 *	Coded by Trinh D.D. Nguyen
 *	MIT licensed
 *
 *	Compile: 
 *		gcc -O2 sortvis.c -o bin/sortvis -s
 *	or:
 *		make
 *	or:
 *		mingw32-make
 *	
 *	Tested on the following platforms:
 *	- macOS Tahoe, LLVM Clang 17
 *
 *	Notes:
 *	- Windows 7 or ealier are not supported.
 *	- Haven't test under Windows 8.1
 *
 *	Updates:
 *	- Added Cocktail Sort, Comb Sort, Counting Sort, Shell Sort, Radix Sort
 *	- Fixed several compile time warnings
 * 	- Flickering fixed
 *	- Enabled VT Terminal on Windows (requires Windows 10 or later)
 *	- Now supports colorul texts on Windows CMD
 *	- Code clean-up
 *	- Animation speed fixed to make it easier to catch up
 *	- Some minor updates
 *	- Animation speed is now customizable via command line parameter.
 *	- New samples generated can now be randomized, ascending or descending.
 *	- Minor optimizations to some sort algorithms added.
 *	- Code clean-up
 *  - Menu navigation via arrow keys added.
 *  - Test suite added as separate program "test.c"
 *  - Other minor fixes and improvements.
 *  - Gradient color schemes added.
 */

#include "sortvis.h"

/*---- MAIN APP ----------------------------*/
int main(int argc, char ** argv) {
	
	app_params(argc, argv);		/* handling command line parameters */
	app_init();					/* initializes environment */
	app_exec();					/* program main loop */	
	app_close();				/* shutting down */

	return 0;	
}