/*
 *	SORTVIS.H
 *	---------
 *	Definitions for sort algorithms visualization
 *	Version 0.5.0
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
		VT_COLOR(196),		/* color 0:  Bright Red */
		VT_COLOR(202),		/* color 1:  Red-Orange */
		VT_COLOR(208),		/* color 2:  Orange */
		VT_COLOR(214),		/* color 3:  Orange-Yellow */
		VT_COLOR(220),		/* color 4:  Gold */
		VT_COLOR(226),		/* color 5:  Yellow */
		VT_COLOR(190),		/* color 6:  Yellow-Green */
		VT_COLOR(154),		/* color 7:  Lime */
		VT_COLOR(118),		/* color 8:  Green */
		VT_COLOR(82) ,		/* color 9:  Spring Green */
		VT_COLOR(48) ,		/* color 10: Mint */
		VT_COLOR(51) ,		/* color 11: Cyan */
		VT_COLOR(45) ,		/* color 12: Sky Blue */
		VT_COLOR(39) ,		/* color 13: Blue */
		VT_COLOR(33) ,		/* color 14: Deep Blue */
		VT_COLOR(27) ,		/* color 15: Indigo */
		VT_COLOR(57) ,		/* color 16: Violet */
		VT_COLOR(93) ,		/* color 17: Purple */
	};

const SHADES SHADE_PASTEL =
	{
		VT_COLOR(51),  		/* color 0:  Cyan */
		VT_COLOR(50),		/* color 1:  Bright Cyan */
		VT_COLOR(49),		/* color 2:  Light Cyan */
		VT_COLOR(48),		/* color 3:  Cyan-Green */
		VT_COLOR(84),		/* color 4:  Aqua */
		VT_COLOR(120),		/* color 5:  Light Aqua */
		VT_COLOR(156),		/* color 6:  Cyan-Yellow */
		VT_COLOR(192),		/* color 7:  Light Green-Yellow */
		VT_COLOR(228),		/* color 8:  Pale Yellow */
		VT_COLOR(227),		/* color 9:  Yellow */
		VT_COLOR(226),		/* color 10: Bright Yellow */
		VT_COLOR(220),		/* color 11: Gold */
		VT_COLOR(214),		/* color 12: Peach */
		VT_COLOR(211),		/* color 13: Light Pink */
		VT_COLOR(218),		/* color 14: Pink */
		VT_COLOR(212),		/* color 15: Rose Pink */
		VT_COLOR(206),		/* color 16: Hot Pink */
		VT_COLOR(200),		/* color 17: Magenta-Pink */
	};

const SHADES SHADE_PLASMA =
	{
		VT_COLOR(54),  		/* color 0:  Deep Purple */
		VT_COLOR(55),		/* color 1:  Purple */
		VT_COLOR(93),		/* color 2:  Violet */
		VT_COLOR(92),		/* color 3:  Light Purple */
		VT_COLOR(129),		/* color 4:  Magenta-Purple */
		VT_COLOR(128),		/* color 5:  Magenta */
		VT_COLOR(164),		/* color 6:  Pink-Magenta */
		VT_COLOR(200),		/* color 7:  Hot Pink */
		VT_COLOR(206),		/* color 8:  Bright Pink */
		VT_COLOR(213),		/* color 9:  Rose */
		VT_COLOR(219),		/* color 10: Light Pink */
		VT_COLOR(224),		/* color 11: Peach */
		VT_COLOR(223),		/* color 12: Light Peach */
		VT_COLOR(216),		/* color 13: Salmon */
		VT_COLOR(209),		/* color 14: Light Orange */
		VT_COLOR(215),		/* color 15: Orange */
		VT_COLOR(221),		/* color 16: Gold-Orange */
		VT_COLOR(227),		/* color 17: Light Yellow */
	};

const SHADES SHADE_INFERNO =
	{
		VT_COLOR(16),  		/* color 0:  Black */
		VT_COLOR(52),		/* color 1:  Dark Purple */
		VT_COLOR(53),		/* color 2:  Purple */
		VT_COLOR(54),		/* color 3:  Deep Purple */
		VT_COLOR(90),		/* color 4:  Purple-Magenta */
		VT_COLOR(91),		/* color 5:  Dark Magenta */
		VT_COLOR(127),		/* color 6:  Magenta */
		VT_COLOR(126),		/* color 7:  Magenta-Red */
		VT_COLOR(160),		/* color 8:  Dark Red */
		VT_COLOR(161),		/* color 9:  Red */
		VT_COLOR(196),		/* color 10: Bright Red */
		VT_COLOR(202),		/* color 11: Red-Orange */
		VT_COLOR(208),		/* color 12: Orange */
		VT_COLOR(214),		/* color 13: Orange-Gold */
		VT_COLOR(220),		/* color 14: Gold */
		VT_COLOR(226),		/* color 15: Yellow */
		VT_COLOR(227),		/* color 16: Light Yellow */
		VT_COLOR(228),		/* color 17: Pale Yellow */
	};

const SHADES SHADE_VIRIDIS =
	{
		VT_COLOR(53),  		/* color 0:  Deep Purple */
		VT_COLOR(54),		/* color 1:  Dark Purple */
		VT_COLOR(55),		/* color 2:  Purple */
		VT_COLOR(56),		/* color 3:  Blue-Purple */
		VT_COLOR(62),		/* color 4:  Dark Blue */
		VT_COLOR(63),		/* color 5:  Blue */
		VT_COLOR(26),		/* color 6:  Blue */
		VT_COLOR(25),		/* color 7:  Deep Blue */
		VT_COLOR(24),		/* color 8:  Dark Cyan */
		VT_COLOR(30),		/* color 9:  Teal */
		VT_COLOR(36),		/* color 10: Dark Green-Cyan */
		VT_COLOR(35),		/* color 11: Green-Cyan */
		VT_COLOR(34),		/* color 12: Green */
		VT_COLOR(40),		/* color 13: Bright Green */
		VT_COLOR(76),		/* color 14: Lime Green */
		VT_COLOR(112),		/* color 15: Light Green */
		VT_COLOR(148),		/* color 16: Yellow-Green */
		VT_COLOR(184),		/* color 17: Light Yellow */
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

	if (!vt_start()) {
		fprintf(stderr, "\nFailed to initialize terminal support.\n");
		fprintf(stderr, "SortVis requires:\n");
		fprintf(stderr, "  - Windows 10 (Build 10586) or later, OR\n");
		fprintf(stderr, "  - macOS/Linux with ANSI terminal support\n\n");
		exit(-1);
	}

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
			"%s| %sP%s. Select gradient   %s|\n"	
			"%s+----------------------+\n"
			"%s| %sQ%s. Exit              %s|\n"
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
			VT_COLOR(8), VT_COLOR(147), VT_DEFAULTATTR, VT_COLOR(8),
			VT_COLOR(8),
			VT_COLOR(8), VT_COLOR(228),  VT_DEFAULTATTR, VT_COLOR(8), 
			VT_COLOR(8));
	
	sprintf(menuFooter, 
			"%sChoice (%sA%s-%sQ%s): ",
			VT_DEFAULTATTR, VT_ATTR(33), VT_DEFAULTATTR, VT_ATTR(33), VT_DEFAULTATTR);

	strcpy(menuText, menuTitle);
	strcat(menuText, menuCommands);
	strcat(menuText, menuFooter);
}

bool select_gradient() {
	char choice;
	int selected = 0;
	const char* items[] = {
		"A. Rainbow",
		"B. Pastel",
		"C. Plasma",
		"D. Inferno",
		"E. Viridis",
		"",
		"F. Back"
	};
	
	while (1) {
		clear();
		printf(VT_COLOR(220)"SELECT COLOR GRADIENT\n");
		printf(VT_RESET"--------------------\n");
		
		for (int i = 0; i < 7; i++) {
			if (items[i][0] == '\0') {
				printf(VT_RESET"--------------------\n");
			} else {
				if (i == selected) {
					printf(VT_ATTR(7)"%-20s"VT_DEFAULTATTR"\n", items[i]);
				} else {
					printf(VT_COLOR(150)"%c"VT_RESET"%s\n", items[i][0], items[i] + 1);
				}
			}
		}
		
		printf(VT_RESET"\nUse "VT_ATTR(33)"UP/DOWN"VT_DEFAULTATTR" arrows or "VT_ATTR(33)"A-F"VT_DEFAULTATTR", press "VT_ATTR(33)"ENTER"VT_DEFAULTATTR" to select\n");
		fflush(stdout);
		
		int ch = getch_arrow();
		
		/* Handle arrow keys */
		if (ch == 'U') {  /* Up arrow */
			do {
				selected = (selected - 1 + 7) % 7;
			} while (selected == 5);  /* Skip separator */
			continue;
		} else if (ch == 'D') {  /* Down arrow */
			do {
				selected = (selected + 1) % 7;
			} while (selected == 5);  /* Skip separator */
			continue;
		} else if (ch == '\n' || ch == '\r') {  /* Enter */
			const char mapping[] = "ABCDEF";
			choice = (selected == 6) ? 'F' : mapping[selected];
		} else {
			choice = toupper(ch & 0xFF);
		}
		
		switch (choice) {
		case 'A': set_shades(SHADE_RAINBOW); return true;
		case 'B': set_shades(SHADE_PASTEL); return true;
		case 'C': set_shades(SHADE_PLASMA); return true;
		case 'D': set_shades(SHADE_INFERNO); return true;
		case 'E': set_shades(SHADE_VIRIDIS); return true;
		case 'F': return false;
		default: continue;
		}
	}
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
		"P. Select Gradient",
		"",
		"Q. Exit"
	};
	
	printf("%s", menuTitle);
	for (int i = 0; i < 18; i++) {
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
	printf("%sUse %sUP/DOWN%s arrows or %sA-Q%s, press %sENTER%s to select\n",
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
		    (APP_VERSION >> 8) & 0xFF, (APP_VERSION & 0xFF), 
		    APP_BUILD, APP_PLATFORM,
			"Coded by Trinh D.D. Nguyen");
	die(0, buffer);
}

void app_help() {
	printf("SortVis - Sort Algorithm Visualizations\n");
	printf("========================================\n\n");
	printf("USAGE:\n");
	printf("  sortvis [OPTIONS]\n\n");
	printf("OPTIONS:\n");
	printf("  -v, --version        Display program version information\n");
	printf("  -h, --help           Display this help message\n");
	printf("  -s, --speed <value>  Set animation speed in milliseconds (default: 60)\n");
	printf("                       Lower values = faster animation\n");
	printf("                       Recommended range: 10-200\n\n");
	printf("EXAMPLES:\n");
	printf("  sortvis              Run with default settings\n");
	printf("  sortvis -s 100       Run with slower animation (100ms delay)\n");
	printf("  sortvis --speed 30   Run with faster animation (30ms delay)\n\n");
	printf("SUPPORTED ALGORITHMS:\n");
	printf("  Interchange, Bubble, Cocktail, Selection, Insertion, Shell,\n");
	printf("  Comb, Merge, Heap, Counting, Quick, Radix\n\n");
	printf("NAVIGATION:\n");
	printf("  Use UP/DOWN arrow keys or letter keys (A-Q) to navigate menus\n");
	printf("  Press ENTER to select an option\n\n");
	printf("REQUIREMENTS:\n");
	printf("  Windows 10 or later (for color support)\n");
	printf("  macOS or Linux with ANSI terminal support\n\n");
	exit(0);
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
			
			int ch = getch_arrow();
			
			/* Handle arrow keys */
			if (ch == 'U') {  /* Up arrow */
				do {
					selected = (selected - 1 + 18) % 18;
				} while (selected == 12 || selected == 16);  /* Skip empty lines */
				continue;
			} else if (ch == 'D') {  /* Down arrow */
				do {
					selected = (selected + 1) % 18;
				} while (selected == 12 || selected == 16);  /* Skip empty lines */
				continue;
			} else if (ch == '\n' || ch == '\r') {  /* Enter key */
				/* Map selected index to choice letter */
				const char mapping[] = "ABCDEFGHIJKL MNP Q";
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
		
		if (choice < 'A' || choice > 'Q') continue;
				
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
		
		case 'P' : 	if (select_gradient()) {
						title("GRADIENT CHANGED"); 
			       		sample_show(&origin, -1, -1, -1);
					}
					else {
						title("CURRENT SAMPLES"); 
			       		sample_show(&origin, -1, -1, -1);
					}
				   	break;
		
		case 'X' :
		case 'Q' : 	done = 1; break;
		}
		if (choice >= 'A' && choice <= 'P') waitkey();
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
			if(strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
				app_version();
			}
			else if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
				app_help();
			}
			else if(strcmp(argv[i], "--speed") == 0 || strcmp(argv[i], "-s") == 0) {
				if (i + 1 < argc) {
					int speed;
					if (sscanf(argv[i+1], "%d", &speed) == 1) {
						if (speed < 0) {
							fprintf(stderr, "Error: Speed value must be non-negative (got %d)\n", speed);
							fprintf(stderr, "Use --help for usage information\n");
							exit(1);
						}
						if (speed > 10000) {
							fprintf(stderr, "Warning: Speed value %d is very high (>10 seconds)\n", speed);
							fprintf(stderr, "Continuing anyway...\n");
						}
						SAMPLE_SPEED = speed;
						i++;  /* Skip next argument since we consumed it */
					}
					else {
						fprintf(stderr, "Error: Invalid speed value '%s'\n", argv[i+1]);
						fprintf(stderr, "Speed must be a non-negative integer\n");
						fprintf(stderr, "Use --help for usage information\n");
						exit(1);
					}
				}
				else {
					fprintf(stderr, "Error: --speed/-s requires a value\n");
					fprintf(stderr, "Example: sortvis --speed 100\n");
					fprintf(stderr, "Use --help for usage information\n");
					exit(1);
				}
			}
			else {
				fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
				fprintf(stderr, "Use --help for usage information\n");
				exit(1);
			}
		}
	}
}

#endif