#ifndef __SORTVIS_HELPERS__
#define __SORTVIS_HELPERS__

#include <stdlib.h>
#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

/*---- HELPERS -----------------------------*/
void die(int code, const char * prompt) {
	printf("%s", prompt);
	exit(code);
}

/* Special key codes returned by getch_arrow(), kept outside the
   character range so they never collide with letter keys */
#define	KEY_NONE	0x000		/* unrecognized key, should be ignored */
#define	KEY_UP		0x100
#define	KEY_DOWN	0x101
#define	KEY_LEFT	0x102
#define	KEY_RIGHT	0x103

/* Read a single character without waiting for Enter, returns EOF on end of input */
int getch() {
#ifdef _WIN32
	return _getch();
#else
	struct termios oldt, newt;
	unsigned char c;
	if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
		/* not a terminal: fall back to plain (buffered) input */
		return getchar();
	}
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	newt.c_cc[VMIN] = 1;
	newt.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
		return EOF;  /* Error: cannot set terminal mode */
	}
	/* use read() rather than getchar() so no bytes are left behind in the
	   stdio buffer where the shell's "read" in waitkey() cannot see them */
	int ch = (read(STDIN_FILENO, &c, 1) == 1) ? c : EOF;
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
#endif
}

/* Read arrow keys and special keys in a cross-platform way */
/* Returns: KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_NONE for other special
   keys, EOF on end of input, or the actual character */
int getch_arrow() {
#ifdef _WIN32
	int ch = _getch();
	if (ch == 0 || ch == 224) {  /* Extended key prefix on Windows */
		ch = _getch();
		switch (ch) {
			case 72: return KEY_UP;
			case 80: return KEY_DOWN;
			case 75: return KEY_LEFT;
			case 77: return KEY_RIGHT;
			default: return KEY_NONE;  /* Home, End, PgUp, PgDn, F-keys, ... */
		}
	}
	return ch;
#else
	int ch = getch();
	if (ch == 27) {  /* ESC sequence */
		int next = getch();
		if (next == '[' || next == 'O') {  /* CSI or SS3 sequence */
			int code = getch();
			switch (code) {
				case 'A': return KEY_UP;
				case 'B': return KEY_DOWN;
				case 'C': return KEY_RIGHT;
				case 'D': return KEY_LEFT;
			}
			/* skip the rest of longer sequences such as ESC [ 3 ~ */
			while (code != EOF && (code < 0x40 || code > 0x7E))
				code = getch();
			return KEY_NONE;
		}
		return (next == EOF) ? EOF : KEY_NONE;
	}
	return ch;
#endif
}

void waitkey() {
#ifndef _WIN32
	#define	PAUSE 	"read -p 'Press ENTER to continue. . .' var"	
#else
	#define	PAUSE 	"pause"	
#endif
	if (system(PAUSE) == -1) getch();
}

void mssleep(long ms) {
	if (ms <= 0) return;
#ifndef _WIN32
	struct timespec rem;
	struct timespec req = { (int)(ms / 1000U), (ms % 1000U) * 1000000UL };
	nanosleep(&req , &rem);
#else
	Sleep(ms);
#endif
}

#endif
