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
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
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
