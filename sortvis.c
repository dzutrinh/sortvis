/*
 *	SORTVIS.C
 *	---------
 *	Sort algorithms visualization
 *	Version 0.4.7
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
 *	- Windows 10 Pro, LLVM MinGW64 GCC 18
 *	- Windows 10 Pro, Dev-C++ with TDM-GCC 4.9.2
 *	- Windows 10 Pro, Dev-C++ with MinGW64 13
 *	- macOS Monterey, LLVM Clang 14
 *	- Ubuntu 23.04  , GNU C/C++ 13
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