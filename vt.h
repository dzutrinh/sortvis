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
	if (setlocale(LC_ALL, "") == NULL) {
		fprintf(stderr, "Warning: Could not set locale for Unicode support\n");
	}
	return true;
#else
	/* enable Virtual Terminal in Windows CMD */
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE || hConsole == NULL) {
		fprintf(stderr, "Error: Cannot get console handle\n");
		fprintf(stderr, "Make sure you're running in a console window\n");
		return false;
	}
	
	if (!GetConsoleMode(hConsole, &vtOldMode)) {
		fprintf(stderr, "Error: Cannot get console mode\n");
		fprintf(stderr, "This may not be a valid console window\n");
		return false;
	}
	
	DWORD newMode = vtOldMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hConsole, newMode)) {
		fprintf(stderr, "Error: Cannot enable Virtual Terminal processing\n");
		fprintf(stderr, "Windows 10 (Build 10586) or later is required\n");
		fprintf(stderr, "Your Windows version may not support ANSI escape sequences\n");
		return false;
	}
	
	return true;
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