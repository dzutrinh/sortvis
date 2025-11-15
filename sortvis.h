/*
 *	SORTVIS.H
 *	---------
 *	Definitions for sort algorithms visualization
 *	Version 0.4.7
 *	Coded by Trinh D.D. Nguyen
 *
 */
#ifndef __SORTVIS_H__
#define	__SORTVIS_H__

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define	APP_VERSION	0x0500		/* app version and build number */
#define	APP_BUILD	0x0000

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
	#define	APP_PLATFORM			"Windows"
#endif

/*---- SORT SAMPLES DATA--------------------*/
#define	SAMPLE_SIZE		18
#define	BOARD_WIDTH		(SAMPLE_SIZE << 2)
int SAMPLE_SPEED = 60;		/* animation speed (in milliseconds) */

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
#	define	VBLOCK		0x2589
#	define	VSHADE		0x2591
#	define	VBAR		'='
#	define	VCURRENT	0x25B2
#	define	VRUNNING	0x25BC
#	define	VEXTRA		'#'
#endif

#include "vt.h"

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

#include "helpers.h"

/* global variables */
static char		sortTitle[256] = {0};		/* for displaying sort algorithm title */
static char		menuText[1600] = {0};		/* for setting up main menu */
static SHADES	colors = {0};				/* for selecting shades for sample rendering */
static char		menuTitle[1024];
static char 	menuCommands[1600];
static char 	menuFooter[64];

void set_shades(const SHADES shade) { memcpy(colors, shade, sizeof(SHADES)); }

void title(const char * name) {
	sprintf(sortTitle,	"%s%*s%s<%s<%s<%s< %s%s %s>%s>%s>%s>", 
						VT_CURSORHOME,
						(int)(BOARD_WIDTH-strlen(name)-10)>>1,"",
						VT_COLOR(241), VT_COLOR(244), VT_COLOR(247), VT_COLOR(250),
						VT_COLOR(231), name,
						VT_COLOR(250), VT_COLOR(247), VT_COLOR(244), VT_COLOR(241));
}

#include "algs.h"

void app_init() {

	if (!vt_start())
		die(-1, "Cannot enable virtual terminal.\n");

	cursor_hide();
	srand(time(NULL));
	set_shades(SHADE_RAINBOW);
	
	sprintf(menuTitle, 
			"%so----------------------o\n"
			"%s|  %sS%sO%sR%sT %sV%sI%sS%sU%sA%sL%sI%sZ%sA%sT%sI%sO%sN  %s|\n"
			"%so----------------------o\n", 
			VT_COLOR(8), 
			VT_COLOR(8),
			colors[0], colors[1], colors[2], colors[3], 
			colors[4], colors[5], colors[6], colors[7], 
			colors[8], colors[9], colors[10], colors[11],
			colors[12], colors[13], colors[14], colors[15],
			colors[16], 
			VT_COLOR(8),			
			VT_COLOR(8));

	sprintf(menuCommands, 
			"%s| %sA%s. Interchange Sort  %s|\n"
			"%s| %sB%s. Bubble Sort       %s|\n"
			"%s| %sC%s. Cocktail Sort     %s|\n"
			"%s| %sD%s. Selection Sort    %s|\n"
			"%s| %sE%s. Insertion Sort    %s|\n"
			"%s| %sF%s. Shell Sort        %s|\n"
			"%s| %sG%s. Comb Sort         %s|\n"
			"%s| %sH%s. Merge Sort        %s|\n"
			"%s| %sI%s. Heap Sort         %s|\n"
			"%s| %sJ%s. Counting Sort     %s|\n"
			"%s| %sK%s. Quick Sort        %s|\n"
			"%s| %sL%s. Radix Sort        %s|\n"
			"%s+----------------------+\n"
			"%s| %sM%s. View data         %s|\n"
			"%s| %sN%s. Generate new data %s|\n"	
			"%s+----------------------+\n"
			"%s| %sO%s. Exit              %s|\n"
			"%so----------------------o\n",
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(153), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(147), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(147), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(228),  VT_DEFAULTATTR, VT_COLOR(8), 
			VT_COLOR(8));
	
	sprintf(menuFooter, 
			"%sChoice (%sA%s-%sO%s): ",
			VT_DEFAULTATTR, VT_ATTR(33), VT_DEFAULTATTR, VT_ATTR(33), VT_DEFAULTATTR);

	strcpy(menuText, menuTitle);
	strcat(menuText, menuCommands);
	strcat(menuText, menuFooter);
}

void app_menu() {
	fputs(menuText, stdout);	
}

void app_menu_arrow(int selected) {
	const char* items[] = {
		"A. Interchange Sort",
		"B. Bubble Sort",
		"C. Cocktail Sort",
		"D. Selection Sort",
		"E. Insertion Sort",
		"F. Shell Sort",
		"G. Comb Sort",
		"H. Merge Sort",
		"I. Heap Sort",
		"J. Counting Sort",
		"K. Quick Sort",
		"L. Radix Sort",
		"",
		"M. View Samples",
		"N. Generate New",
		"",
		"O. Exit"
	};
	
	printf("%s", menuTitle);
	for (int i = 0; i < 17; i++) {
		if (items[i][0] == '\0') {
			printf("%s|                      %s|\n", VT_COLOR(8), VT_COLOR(8));
		} else {
			if (i == selected) {
				printf("%s| %s%-20s %s|\n", VT_COLOR(8), VT_ATTR(7), items[i], VT_DEFAULTATTR);
			} else {
				printf("%s| %s%-20s %s|\n", VT_COLOR(8), VT_COLOR(153), items[i], VT_DEFAULTATTR);
			}
		}
	}
	printf("%so----------------------o\n", VT_COLOR(8));
	printf("%sUse %s↑↓%s or %sA-O%s, press %sENTER%s to select\n",
		   VT_DEFAULTATTR, VT_ATTR(33), VT_DEFAULTATTR,
		   VT_ATTR(33), VT_DEFAULTATTR,
		   VT_ATTR(33), VT_DEFAULTATTR);
}

void app_version() {
	char buffer[256];
	sprintf(buffer, 
			"--------------------------\n"
		    "SortVis %d.%d.%d (%s)\n%s\n"
			"--------------------------\n",
		    (APP_VERSION & 0xFF), (APP_VERSION >> 8) & 0xFF, 
		    APP_BUILD, APP_PLATFORM,
			"Coded by Trinh D.D. Nguyen");
	die(0, buffer);
}

void app_help() {
	die(0, 	"usage: sortvis [--version|-v] [--help|-h] [--speed|-s n]\n\n"
			"whereas:\n"
			"\t--version|-v\tdisplay program version information\n"
			"\t--help|-h\tdisplay this message\n"
			"\t--speed|-s\tspecify animation speed\n\n");
}

void app_exec() {
	SAMPLES	origin, sort;
	int done = 0;
	char choice;
	int selected = 0;
	int useArrows = 1;  /* Use arrow key navigation by default */
	
	sample_generate_random(&origin); 
	
	while (!done) {
				
		clear();
		
		if (useArrows) {
			app_menu_arrow(selected);
			
			int ch = getch();
			
			/* Handle arrow keys (escape sequences) */
			if (ch == 27 || ch == 224) {  /* ESC or extended key on Windows */
				getch();  /* Skip '[' on Unix or get actual key on Windows */
				ch = getch();
				if (ch == 'A' || ch == 72) {  /* Up arrow */
					do {
						selected = (selected - 1 + 17) % 17;
					} while (selected == 12 || selected == 15);  /* Skip empty lines */
					continue;
				} else if (ch == 'B' || ch == 80) {  /* Down arrow */
					do {
						selected = (selected + 1) % 17;
					} while (selected == 12 || selected == 15);  /* Skip empty lines */
					continue;
				}
			} else if (ch == '\n' || ch == '\r') {  /* Enter key */
				/* Map selected index to choice letter */
				const char mapping[] = "ABCDEFGHIJKL MN O";
				choice = mapping[selected];
			} else if (ch >= 'a' && ch <= 'z') {
				choice = toupper(ch);
			} else if (ch >= 'A' && ch <= 'Z') {
				choice = ch;
			} else {
				continue;
			}
		} else {
			app_menu();
			fflush(stdin); scanf("%c", &choice);	
			choice = toupper(choice & 0xFF);
		}
		
		if (choice < 'A' && choice > 'M') continue;
				
		clear();

		switch(choice) {
		case 'A' : 	sort = origin; sample_sort_interchange(&sort);                    break;
		case 'B' : 	sort = origin; sample_sort_bubble     (&sort);                    break;
		case 'C' : 	sort = origin; sample_sort_cocktail   (&sort);                    break;
		case 'D' : 	sort = origin; sample_sort_selection  (&sort);                    break;
		case 'E' : 	sort = origin; sample_sort_insertion  (&sort);                    break;
		case 'F' : 	sort = origin; sample_sort_shell      (&sort);                    break;
		case 'G' : 	sort = origin; sample_sort_comb       (&sort);                    break;
		case 'H' : 	sort = origin; sample_sort_merge      (&sort, 0, SAMPLE_SIZE-1);  break;
		case 'I' : 	sort = origin; sample_sort_heap       (&sort);                    break;
		case 'J' : 	sort = origin; sample_sort_count      (&sort);                    break; 
		case 'K' : 	sort = origin; sample_sort_quick      (&sort, 0, SAMPLE_SIZE-1);  break;
		case 'L' : 	sort = origin; sample_sort_radix      (&sort);					  break;

		case 'M' : 	title("CURRENT SORT SAMPLES"); sample_show(&origin, -1, -1, -1);  break;
		case 'N' : 	if (sample_generate(&origin)) {
						title("NEW SAMPLES GENERATED"); 
			       		sample_show(&origin, -1, -1, -1);
					}
					else {
						title("CURRENT SAMPLES"); 
			       		sample_show(&origin, -1, -1, -1);
					}		
				   	break;
		
		case 'X' :
		case 'O' : 	done = 1; break;
		}
		if (choice >= 'A' && choice <= 'N') waitkey();
	}
}

void app_close() {
	cursor_show();
	vt_done();	
}

void app_params(int argc, char ** argv) {
	/* command line parsing */
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if(strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0)
				app_version();
			if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
				app_help();
			if(strcmp(argv[i], "--speed") == 0 || strcmp(argv[i], "-s") == 0) {
				if (argv[i+1])
					sscanf(argv[i+1], "%d", &SAMPLE_SPEED);
				else
					die(0, "No value given.\n");
			}
		}
	}
}

#endif