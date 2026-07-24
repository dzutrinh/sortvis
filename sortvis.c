/*
 *	SORTVIS.C
 *	---------
 *	Sort algorithms visualization
 *	Version 0.6.0
 *	Coded by Trinh D.D. Nguyen
 *	MIT licensed
 *
 *	Compile: 
 *		gcc -O2 sortvis.c -o sortvis
 *	or:
 *		make
 *	or:
 *		mingw32-make
 *	
 *	Tested on the following platforms:
 *	- macOS Tahoe, LLVM Clang 17, LLVM Clang 21
 *	- Windows 10/11 with MinGW
 *
 *	Notes:
 *	- Windows 10 Build 10586 or later required for VT support
 *	- Windows 7/8/8.1 are not supported
 *
 *	Updates:
 *	- Added Cocktail Sort, Comb Sort, Counting Sort, Shell Sort, Radix Sort
 *	- Fixed several compile time warnings
 * 	- Flickering fixed
 *	- Enabled VT Terminal on Windows (requires Windows 10 or later)
 *	- Now supports colorful texts on Windows CMD
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
 *  - Enhanced --help parameter with detailed information
 *  - Improved error handling for command-line arguments
 *  - Fixed Win32 arrow key detection compatibility
 *  - Added robust terminal initialization with error messages
 *  - Added Circle Sort visualization
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