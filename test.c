/*
 *	TEST.C
 *	------
 *	Comprehensive test suite for sorting algorithms
 *	Tests all 12 sorting algorithms with various scenarios
 *	Coded by Trinh D.D. Nguyen
 *	MIT licensed
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

/* Define required types and constants */
#define	SAMPLE_SIZE		18
#define	BOARD_WIDTH		(SAMPLE_SIZE << 2)

/* Define SAMPLE_SPEED for algs.h (set to 0 for fast testing) */
int SAMPLE_SPEED = 0;

typedef enum {false, true} bool;

typedef struct samples {
	int data[SAMPLE_SIZE];
	int max;
} SAMPLES;

#ifndef _WIN32
	#include <locale.h>
	#include <wchar.h>
#endif

/* Stub out VT macros needed by sample_generate() */
#define VT_COLOR(x) ""
#define VT_RESET ""
#define VT_DEFAULTATTR ""
#define VT_ATTR(x) ""

/* Stub out visual constants needed by sample_show() */
#define VBAR '-'
#define VSHADE '.'
#define VBLOCK '#'
#define VCURRENT '^'
#define VRUNNING '_'
#define VEXTRA '*'

/* Dummy variables needed by algs.h */
static char sortTitle[256] = "";
typedef char SHADES[SAMPLE_SIZE][16];
static SHADES colors = {0};

/* Disable visualization during tests */
extern bool ENABLE_VISUALIZATION;

/* Stub out helper functions that algs.h needs but doesn't define */
void mssleep(long ms) { (void)ms; }
void title(const char * name) { (void)name; }
void clear(void) { }
void reset_colors(void) { }

/* getch() implementation for raw input */
int getch(void) {
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

/* Now include the actual algorithm implementations from algs.h */
#include "algs.h"


/* Test statistics */
typedef struct {
	int total_tests;
	int passed_tests;
	int failed_tests;
} TestStats;

TestStats stats = {0, 0, 0};

/* Color output for test results */
#define TEST_PASS "\x1B[32m✓ PASS\x1B[0m"
#define TEST_FAIL "\x1B[31m✗ FAIL\x1B[0m"
#define TEST_INFO "\x1B[36m[INFO]\x1B[0m"
#define TEST_ERROR "\x1B[31m[ERROR]\x1B[0m"

/*---- TEST UTILITIES --------------------------*/
void print_array(SAMPLES * s, const char* label) {
	printf("%s: [", label);
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		printf("%d", s->data[i]);
		if (i < SAMPLE_SIZE - 1) printf(", ");
	}
	printf("]\n");
}

bool arrays_equal(SAMPLES * a, SAMPLES * b) {
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		if (a->data[i] != b->data[i]) return false;
	}
	return true;
}

void copy_samples(SAMPLES * dest, SAMPLES * src) {
	memcpy(dest->data, src->data, sizeof(src->data));
	dest->max = src->max;
}

void create_expected_sorted(SAMPLES * s, SAMPLES * expected) {
	copy_samples(expected, s);
	/* Simple bubble sort to create expected result */
	for (int i = 0; i < SAMPLE_SIZE - 1; i++) {
		for (int j = 0; j < SAMPLE_SIZE - i - 1; j++) {
			if (expected->data[j] > expected->data[j + 1]) {
				int temp = expected->data[j];
				expected->data[j] = expected->data[j + 1];
				expected->data[j + 1] = temp;
			}
		}
	}
}

void run_test(const char* test_name, void (*sort_func)(SAMPLES*), SAMPLES * input, int extra_param) {
	SAMPLES test_data, expected;
	copy_samples(&test_data, input);
	create_expected_sorted(input, &expected);
	
	/* Run the sorting algorithm */
	if (extra_param >= 0) {
		/* For merge and quick sort with parameters */
		if (strstr(test_name, "Merge")) {
			sample_sort_merge(&test_data, 0, SAMPLE_SIZE - 1);
		} else if (strstr(test_name, "Quick")) {
			sample_sort_quick(&test_data, 0, SAMPLE_SIZE - 1);
		}
	} else {
		sort_func(&test_data);
	}
	
	stats.total_tests++;
	
	/* Verify the result */
	bool is_sorted = sample_is_sorted(&test_data);
	bool matches_expected = arrays_equal(&test_data, &expected);
	
	if (is_sorted && matches_expected) {
		printf("  %s: %s\n", test_name, TEST_PASS);
		stats.passed_tests++;
	} else {
		printf("  %s: %s\n", test_name, TEST_FAIL);
		if (!is_sorted) {
			printf("    %s Array is not sorted\n", TEST_ERROR);
		}
		if (!matches_expected) {
			printf("    %s Result doesn't match expected output\n", TEST_ERROR);
			print_array(&test_data, "    Got");
			print_array(&expected, "    Expected");
		}
		stats.failed_tests++;
	}
}

/*---- TEST SCENARIOS --------------------------*/
void test_random_data() {
	printf("\n%s Testing with Random Data\n", TEST_INFO);
	SAMPLES random;
	sample_generate_random(&random);
	
	printf("Input: Random shuffled array\n");
	
	run_test("Interchange Sort", sample_sort_interchange, &random, -1);
	run_test("Bubble Sort", sample_sort_bubble, &random, -1);
	run_test("Cocktail Sort", sample_sort_cocktail, &random, -1);
	run_test("Selection Sort", sample_sort_selection, &random, -1);
	run_test("Insertion Sort", sample_sort_insertion, &random, -1);
	run_test("Shell Sort", sample_sort_shell, &random, -1);
	run_test("Comb Sort", sample_sort_comb, &random, -1);
	run_test("Merge Sort", NULL, &random, 1);
	run_test("Heap Sort", sample_sort_heap, &random, -1);
	run_test("Counting Sort", sample_sort_count, &random, -1);
	run_test("Quick Sort", NULL, &random, 1);
	run_test("Radix Sort", sample_sort_radix, &random, -1);
}

void test_already_sorted() {
	printf("\n%s Testing with Already Sorted Data (Best Case)\n", TEST_INFO);
	SAMPLES sorted;
	sample_generate_ascending(&sorted);
	
	printf("Input: [1, 2, 3, ..., %d]\n", SAMPLE_SIZE);
	
	run_test("Interchange Sort", sample_sort_interchange, &sorted, -1);
	run_test("Bubble Sort", sample_sort_bubble, &sorted, -1);
	run_test("Cocktail Sort", sample_sort_cocktail, &sorted, -1);
	run_test("Selection Sort", sample_sort_selection, &sorted, -1);
	run_test("Insertion Sort", sample_sort_insertion, &sorted, -1);
	run_test("Shell Sort", sample_sort_shell, &sorted, -1);
	run_test("Comb Sort", sample_sort_comb, &sorted, -1);
	run_test("Merge Sort", NULL, &sorted, 1);
	run_test("Heap Sort", sample_sort_heap, &sorted, -1);
	run_test("Counting Sort", sample_sort_count, &sorted, -1);
	run_test("Quick Sort", NULL, &sorted, 1);
	run_test("Radix Sort", sample_sort_radix, &sorted, -1);
}

void test_reverse_sorted() {
	printf("\n%s Testing with Reverse Sorted Data (Worst Case)\n", TEST_INFO);
	SAMPLES reversed;
	sample_generate_descending(&reversed);
	
	printf("Input: [%d, %d, %d, ..., 1]\n", SAMPLE_SIZE, SAMPLE_SIZE-1, SAMPLE_SIZE-2);
	
	run_test("Interchange Sort", sample_sort_interchange, &reversed, -1);
	run_test("Bubble Sort", sample_sort_bubble, &reversed, -1);
	run_test("Cocktail Sort", sample_sort_cocktail, &reversed, -1);
	run_test("Selection Sort", sample_sort_selection, &reversed, -1);
	run_test("Insertion Sort", sample_sort_insertion, &reversed, -1);
	run_test("Shell Sort", sample_sort_shell, &reversed, -1);
	run_test("Comb Sort", sample_sort_comb, &reversed, -1);
	run_test("Merge Sort", NULL, &reversed, 1);
	run_test("Heap Sort", sample_sort_heap, &reversed, -1);
	run_test("Counting Sort", sample_sort_count, &reversed, -1);
	run_test("Quick Sort", NULL, &reversed, 1);
	run_test("Radix Sort", sample_sort_radix, &reversed, -1);
}

void test_duplicate_values() {
	printf("\n%s Testing with Duplicate Values\n", TEST_INFO);
	SAMPLES duplicates;
	
	/* Create array with many duplicates */
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		duplicates.data[i] = (i % 5) + 1; /* Values: 1,2,3,4,5,1,2,3,4,5,... */
	}
	duplicates.max = SAMPLE_SIZE;
	
	printf("Input: Array with duplicate values [");
	for (int i = 0; i < 8 && i < SAMPLE_SIZE; i++) {
		printf("%d", duplicates.data[i]);
		if (i < 7) printf(", ");
	}
	printf(", ...]\n");
	
	run_test("Interchange Sort", sample_sort_interchange, &duplicates, -1);
	run_test("Bubble Sort", sample_sort_bubble, &duplicates, -1);
	run_test("Cocktail Sort", sample_sort_cocktail, &duplicates, -1);
	run_test("Selection Sort", sample_sort_selection, &duplicates, -1);
	run_test("Insertion Sort", sample_sort_insertion, &duplicates, -1);
	run_test("Shell Sort", sample_sort_shell, &duplicates, -1);
	run_test("Comb Sort", sample_sort_comb, &duplicates, -1);
	run_test("Merge Sort", NULL, &duplicates, 1);
	run_test("Heap Sort", sample_sort_heap, &duplicates, -1);
	run_test("Counting Sort", sample_sort_count, &duplicates, -1);
	run_test("Quick Sort", NULL, &duplicates, 1);
	run_test("Radix Sort", sample_sort_radix, &duplicates, -1);
}

void test_nearly_sorted() {
	printf("\n%s Testing with Nearly Sorted Data\n", TEST_INFO);
	SAMPLES nearly;
	sample_generate_ascending(&nearly);
	
	/* Swap a few elements to make it nearly sorted */
	sample_swap(&nearly, 2, 5);
	sample_swap(&nearly, 10, 12);
	sample_swap(&nearly, 15, 16);
	
	printf("Input: Nearly sorted array with few swaps\n");
	
	run_test("Interchange Sort", sample_sort_interchange, &nearly, -1);
	run_test("Bubble Sort", sample_sort_bubble, &nearly, -1);
	run_test("Cocktail Sort", sample_sort_cocktail, &nearly, -1);
	run_test("Selection Sort", sample_sort_selection, &nearly, -1);
	run_test("Insertion Sort", sample_sort_insertion, &nearly, -1);
	run_test("Shell Sort", sample_sort_shell, &nearly, -1);
	run_test("Comb Sort", sample_sort_comb, &nearly, -1);
	run_test("Merge Sort", NULL, &nearly, 1);
	run_test("Heap Sort", sample_sort_heap, &nearly, -1);
	run_test("Counting Sort", sample_sort_count, &nearly, -1);
	run_test("Quick Sort", NULL, &nearly, 1);
	run_test("Radix Sort", sample_sort_radix, &nearly, -1);
}

void test_all_same_values() {
	printf("\n%s Testing with All Same Values\n", TEST_INFO);
	SAMPLES same;
	
	/* All elements have the same value */
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		same.data[i] = 7;
	}
	same.max = SAMPLE_SIZE;
	
	printf("Input: All elements are 7\n");
	
	run_test("Interchange Sort", sample_sort_interchange, &same, -1);
	run_test("Bubble Sort", sample_sort_bubble, &same, -1);
	run_test("Cocktail Sort", sample_sort_cocktail, &same, -1);
	run_test("Selection Sort", sample_sort_selection, &same, -1);
	run_test("Insertion Sort", sample_sort_insertion, &same, -1);
	run_test("Shell Sort", sample_sort_shell, &same, -1);
	run_test("Comb Sort", sample_sort_comb, &same, -1);
	run_test("Merge Sort", NULL, &same, 1);
	run_test("Heap Sort", sample_sort_heap, &same, -1);
	run_test("Counting Sort", sample_sort_count, &same, -1);
	run_test("Quick Sort", NULL, &same, 1);
	run_test("Radix Sort", sample_sort_radix, &same, -1);
}

/*---- MAIN TEST RUNNER ------------------------*/
int main(int argc, char **argv) {
	printf("\n");
	printf("+----------------------------------------------------+\n");
	printf("|  SORTVIS - Sorting Algorithms Test Suite           |\n");
	printf("|  Testing all 12 sorting algorithms                 |\n");
	printf("+----------------------------------------------------+\n");
	
	srand(time(NULL));
	
	/* Disable visualization during tests */
	ENABLE_VISUALIZATION = false;
	
	/* Run all test scenarios */
	test_random_data();
	test_already_sorted();
	test_reverse_sorted();
	test_duplicate_values();
	test_nearly_sorted();
	test_all_same_values();
	
	/* Print summary */
	printf("\n");
	printf("+----------------------------------------------------+\n");
	printf("|  TEST SUMMARY                                      |\n");
	printf("+----------------------------------------------------+\n");
	printf("|  Total Tests:  %-4d                                |\n", stats.total_tests);
	
	if (stats.failed_tests == 0) {
		printf("|  Passed:       \x1B[32m%-4d ✓\x1B[0m                              |\n", stats.passed_tests);
		printf("|  Failed:       \x1B[32m%-4d\x1B[0m                                |\n", stats.failed_tests);
	} else {
		printf("|  Passed:       %-4d                                |}\n", stats.passed_tests);
		printf("|  Failed:       \x1B[31m%-4d ✗\x1B[0m                              |\n", stats.failed_tests);
	}
	
	printf("+----------------------------------------------------+\n");
	
	if (stats.failed_tests == 0) {
		printf("\n\x1B[32m🎉 All tests passed successfully!\x1B[0m\n\n");
		return 0;
	} else {
		printf("\n\x1B[31m⚠️  Some tests failed. Please review the errors above.\x1B[0m\n\n");
		return 1;
	}
}
