#ifndef __SORTVIS_VT__
#define __SORTVIS_VT__

#include <string.h>
#ifdef _WIN32
    #include <windows.h>
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
	#	define	ENABLE_VIRTUAL_TERMINAL_PROCESSING	0x0004
	#endif
#endif

#define	VT_COLOR(v)		"\x1B[38;5;"#v"m"
#define	VT_BKGD(v)		"\x1B[48;5;"#v"m"
#define	VT_ATTR(v)		"\x1B["#v"m"
#define	VT_CLEAR		"\x1B[H\x1B[2J"
#define	VT_CURSORSHOW	"\x1B[?25h"
#define	VT_CURSORHIDE	"\x1B[?25l"
#define	VT_DEFAULTATTR	"\x1B[0m"
#define	VT_CURSORHOME	"\x1B[H"
#define VT_RESET		VT_DEFAULTATTR

#ifdef _WIN32
	static	DWORD	vtOldMode = 0;
	static	HANDLE	hConsole = 0;
#endif

bool vt_start() {
#ifndef _WIN32	
	setlocale(LC_ALL, "");			/* enable Unicode printf() */
	return true;
#else
	/* enable Virtual Terminal in Windows CMD */
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole) {
		GetConsoleMode(hConsole, &vtOldMode);
		SetConsoleMode(hConsole, vtOldMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		return true;
	}
	else 
		return false;
#endif
}

void vt_done() {
#ifdef _WIN32
	if (hConsole)							/* restore previous Windows CMD mode */
		SetConsoleMode(hConsole, vtOldMode);
#endif
}

void clear()        	{ fputs(VT_CLEAR, stdout); }
void set_bkgd(int c)	{ fputs(VT_BKGD(c), stdout); }
void cursor_show()  	{ fputs(VT_CURSORSHOW, stdout); }
void cursor_hide()  	{ fputs(VT_CURSORHIDE, stdout); }
void reset_colors() 	{ fputs(VT_DEFAULTATTR, stdout); }

#endif
