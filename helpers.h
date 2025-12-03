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

/* Read a single character without waiting for Enter */
int getch() {
#ifdef _WIN32
	return _getch();
#else
	struct termios oldt, newt;
	int ch;
	if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
		return -1;  /* Error: not a terminal */
	}
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
		return -1;  /* Error: cannot set terminal mode */
	}
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
#endif
}

/* Read arrow keys and special keys in a cross-platform way */
/* Returns: 'U' for up, 'D' for down, 'L' for left, 'R' for right, or the actual character */
int getch_arrow() {
#ifdef _WIN32
	int ch = _getch();
	if (ch == 0 || ch == 224) {  /* Extended key prefix on Windows */
		ch = _getch();
		switch (ch) {
			case 72: return 'U';  /* Up arrow */
			case 80: return 'D';  /* Down arrow */
			case 75: return 'L';  /* Left arrow */
			case 77: return 'R';  /* Right arrow */
			default: return ch;
		}
	}
	return ch;
#else
	int ch = getch();
	if (ch == 27) {  /* ESC sequence */
		int next = getch();
		if (next == '[') {  /* ANSI escape sequence */
			int arrow = getch();
			switch (arrow) {
				case 'A': return 'U';  /* Up arrow */
				case 'B': return 'D';  /* Down arrow */
				case 'C': return 'R';  /* Right arrow */
				case 'D': return 'L';  /* Left arrow */
				default: return arrow;
			}
		}
		return next;
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
	system(PAUSE);
}

void mssleep(long ms) {
#ifndef _WIN32
	struct timespec rem;
	struct timespec req = { (int)(ms / 1000U), (ms % 1000U) * 1000000UL };
	nanosleep(&req , &rem);
#else
	Sleep(ms);
#endif
}

#endif
