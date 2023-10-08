/*
 *	SORTVIS.H
 *	---------
 *	Definitions for sort algorithms visualization
 *	Version 0.4.5
 *	Coded by Trinh D.D. Nguyen
 *
 */
#ifndef __SORTVIS_H__
#define	__SORTVIS_H__

#define	APP_VERSION	0x0400		/* app version and build number */
#define	APP_BUILD	0x0005

#ifndef _WIN32					/* printf() Unicode support on non-Windows platforms */
	#include <locale.h>
	#include <wchar.h>
	#ifdef __linux__
	#	define	APP_PLATFORM		"Linux"
	#else
	#	ifdef __APPLE__
	#		define	APP_PLATFORM	"macOS"
	#	else
	#		define	APP_PLATFORM	"Unknown"
	#	endif
	#endif
#else
	#include <windows.h>
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
	#	define	ENABLE_VIRTUAL_TERMINAL_PROCESSING	0x0004
	#endif
	#define	APP_PLATFORM			"Windows"
#endif

/*---- SORT SAMPLES DATA--------------------*/
#define SAMPLE_SPEED	60		/* animation speed (in milliseconds) */
#define	SAMPLE_SIZE		18
#define	BOARD_WIDTH		(SAMPLE_SIZE << 2)

typedef enum {false, true} bool;

typedef struct samples {
	int data[SAMPLE_SIZE];
	int max;
} SAMPLES;

typedef	char SHADES[SAMPLE_SIZE][16];

/* characters for rendering samples on screen */
#ifdef _WIN32
#	define	VBLOCK		0x00DB
#	define	VSHADE		0x00B0
#	define	VBAR		0x00C4
#	define	VCURRENT	0x00DF
#	define	VRUNNING	0x00DC
#	define	VEXTRA		'#'
#else
#	define	VBLOCK		0x2588
#	define	VSHADE		0x2591
#	define	VBAR		0x002D
#	define	VCURRENT	0x2580
#	define	VRUNNING	0x2584
#	define	VEXTRA		'#'
#endif

#define	VT_COLOR(v)		("\x1B[38;5;"#v"m")
#define	VT_BKGD(v)		("\x1B[48;5;"#v"m")
#define	VT_ATTR(v)		("\x1B["#v"m")
#define	VT_CLEAR		("\x1B[H\x1B[2J")
#define	VT_CURSORSHOW	("\x1B[?25h")
#define	VT_CURSORHIDE	("\x1B[?25l")
#define	VT_DEFAULTATTR	("\x1B[0m")
#define	VT_CURSORHOME	("\x1B[H")

const SHADES SHADE_RAINBOW = 				/* rainbow colors for data bars */
	{
		VT_COLOR(200),		/* color 0 */
		VT_COLOR(197),		/* color 1 */
		VT_COLOR(160),		/* color 2 */
		VT_COLOR(196),		/* color 3 */
		VT_COLOR(208),		/* color 4 */
		VT_COLOR(220),		/* color 5 */
		VT_COLOR(226),		/* color 6 */
		VT_COLOR(190),		/* color 7 */
		VT_COLOR(154),		/* color 8 */
		VT_COLOR(76) ,		/* color 9 */
		VT_COLOR(48) ,		/* color 10 */
		VT_COLOR(121),		/* color 11 */
		VT_COLOR(87) ,		/* color 12 */
		VT_COLOR(45) ,		/* color 13 */
		VT_COLOR(33) ,		/* color 14 */
		VT_COLOR(21) ,		/* color 15 */
		VT_COLOR(129),		/* color 16 */
		VT_COLOR(90) ,		/* color 17 */
	};

const SHADES SHADE_VAMPYRIC =
	{
		VT_COLOR(22), 		/* color 0 */
		VT_COLOR(28),		/* color 1 */
		VT_COLOR(34),		/* color 2 */
		VT_COLOR(40),		/* color 3 */
		VT_COLOR(76),		/* color 4 */
		VT_COLOR(82),		/* color 5 */
		VT_COLOR(112),		/* color 6 */
		VT_COLOR(118),		/* color 7 */
		VT_COLOR(154),		/* color 8 */
		VT_COLOR(148),		/* color 9 */
		VT_COLOR(184),		/* color 10 */
		VT_COLOR(220),		/* color 11 */
		VT_COLOR(178),		/* color 12 */
		VT_COLOR(226),		/* color 13 */
		VT_COLOR(227),		/* color 14 */
		VT_COLOR(228),		/* color 15 */
		VT_COLOR(229),		/* color 16 */
		VT_COLOR(231),		/* color 17 */
	};

const SHADES SHADE_GRAYSCALE =
	{
		VT_COLOR(235), 		/* color 0 */
		VT_COLOR(236),		/* color 1 */
		VT_COLOR(237),		/* color 2 */
		VT_COLOR(238),		/* color 3 */
		VT_COLOR(239),		/* color 4 */
		VT_COLOR(240),		/* color 5 */
		VT_COLOR(241),		/* color 6 */
		VT_COLOR(242),		/* color 7 */
		VT_COLOR(243),		/* color 8 */
		VT_COLOR(244),		/* color 9 */
		VT_COLOR(245),		/* color 10 */
		VT_COLOR(246),		/* color 11 */
		VT_COLOR(247),		/* color 12 */
		VT_COLOR(248),		/* color 13 */
		VT_COLOR(249),		/* color 14 */
		VT_COLOR(250),		/* color 15 */
		VT_COLOR(251),		/* color 16 */
		VT_COLOR(252),		/* color 17 */
	};


#endif

