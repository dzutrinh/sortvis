/*
 *	TEST.C
 *	------
 *	Test suite for the sort algorithms used by SortVis
 *	MIT licensed
 *
 *	Compile & run:
 *		make test
 *	or:
 *		gcc -O2 -Wall -std=c99 test.c -o sortvis_test && ./sortvis_test [seed]
 *
 *	Every algorithm is run with visualization disabled and checked for:
 *	- sorted output that is a permutation of its input
 *	- values staying within the range the renderer can draw
 *	- statistics counters matching what the algorithm must do
 *	The renderer itself is exercised once at the end with output discarded.
 */

#include "sortvis.h"

#ifdef _WIN32
#	include <io.h>
#	define	NULL_DEVICE	"NUL"
#else
#	include <unistd.h>
#	define	NULL_DEVICE	"/dev/null"
#endif

#define	RANDOM_RUNS		2000		/* random inputs tried per algorithm */
#define	PAIRS			(SAMPLE_SIZE * (SAMPLE_SIZE - 1) / 2)

/*---- ALGORITHMS UNDER TEST ---------------*/
static void sort_merge(SAMPLES * s) { sample_sort_merge(s, 0, SAMPLE_SIZE-1); }
static void sort_quick(SAMPLES * s) { sample_sort_quick(s, 0, SAMPLE_SIZE-1); }

typedef struct algorithm {
	const char * name;
	void (*sort)(SAMPLES * s);
	bool exchange;		/* only swaps out-of-order elements: no swaps on sorted input */
	bool adjacent;		/* only fixes adjacent inversions: swaps == inversions */
} ALGORITHM;

static const ALGORITHM ALGORITHMS[] = {
	{ "Interchange Sort",	sample_sort_interchange,	true,	false },
	{ "Bubble Sort",		sample_sort_bubble,			true,	true  },
	{ "Cocktail Sort",		sample_sort_cocktail,		true,	true  },
	{ "Selection Sort",		sample_sort_selection,		true,	false },
	{ "Insertion Sort",		sample_sort_insertion,		true,	true  },
	{ "Shell Sort",			sample_sort_shell,			true,	false },
	{ "Comb Sort",			sample_sort_comb,			true,	false },
	{ "Merge Sort",			sort_merge,					false,	false },
	{ "Heap Sort",			sample_sort_heap,			false,	false },
	{ "Counting Sort",		sample_sort_count,			false,	false },
	{ "Quick Sort",			sort_quick,					true,	false },
	{ "Radix Sort",			sample_sort_radix,			false,	false },
	{ "Circle Sort",		sample_sort_circle,			true,	false },
};
#define	ALGORITHM_COUNT	(int)(sizeof(ALGORITHMS) / sizeof(ALGORITHMS[0]))

static const ALGORITHM * find(const char * name) {
	for (int k = 0; k < ALGORITHM_COUNT; k++)
		if (strcmp(ALGORITHMS[k].name, name) == 0) return &ALGORITHMS[k];
	fprintf(stderr, "unknown algorithm '%s'\n", name);
	exit(2);
}

/*---- MINIMAL TEST FRAMEWORK --------------*/
static int checks = 0, failures = 0;
static int case_failures = 0;

static void dump(const char * label, const SAMPLES * s) {
	if (s == NULL) return;
	printf("        %-8s", label);
	for (int i = 0; i < SAMPLE_SIZE; i++) printf(" %d", s->data[i]);
	printf("\n");
}

static void failed(const SAMPLES * input, const SAMPLES * output) {
	printf("\n");
	dump("input:",  input);
	dump("output:", output);
	failures++;
	case_failures++;
}

/* records a failed check, printing the samples involved (either may be NULL) */
#define	CHECK(cond, input, output, ...)	do {				\
		checks++;											\
		if (!(cond)) {										\
			if (case_failures == 0) printf("\n");			\
			printf("    FAIL  " __VA_ARGS__);				\
			failed(input, output);							\
		}													\
	} while (0)

static void begin(const char * name) {
	case_failures = 0;
	printf("  %-42s", name);
	fflush(stdout);
}

static void end() {
	if (case_failures == 0) printf("ok\n");
}

/*---- SAMPLE HELPERS ----------------------*/
static void reset_stats(SAMPLES * s) {
	s->comparisons = 0;
	s->swaps = 0;
	s->sorted_until = -1;
	s->sorted_from = SAMPLE_SIZE;
}

static void generate_constant(SAMPLES * s, int value) {
	sample_generate_ascending(s);
	for (int i = 0; i < SAMPLE_SIZE; i++) s->data[i] = value;
}

static void generate_duplicates(SAMPLES * s) {
	sample_generate_ascending(s);
	for (int i = 0; i < SAMPLE_SIZE; i++) s->data[i] = 1 + rand() % 4;
}

static void generate_with_values(SAMPLES * s) {
	sample_generate_ascending(s);
	for (int i = 0; i < SAMPLE_SIZE; i++) s->data[i] = 1 + rand() % SAMPLE_SIZE;
}

static long inversions(const SAMPLES * s) {
	long n = 0;
	for (int i = 0; i < SAMPLE_SIZE; i++)
		for (int j = i + 1; j < SAMPLE_SIZE; j++)
			if (s->data[i] > s->data[j]) n++;
	return n;
}

static bool same_values(const SAMPLES * a, const SAMPLES * b) {
	int count[SAMPLE_SIZE + 2] = {0};
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		if (a->data[i] < 1 || a->data[i] > SAMPLE_SIZE) return false;
		if (b->data[i] < 1 || b->data[i] > SAMPLE_SIZE) return false;
		count[a->data[i]]++;
		count[b->data[i]]--;
	}
	for (int v = 0; v <= SAMPLE_SIZE; v++)
		if (count[v] != 0) return false;
	return true;
}

/* runs one algorithm on a copy of the input and checks the universal properties */
static SAMPLES run(const ALGORITHM * alg, const SAMPLES * input) {
	SAMPLES s = *input;
	reset_stats(&s);
	alg->sort(&s);
	CHECK(sample_is_sorted(&s), input, &s, "%s: output is not sorted", alg->name);
	CHECK(same_values(input, &s), input, &s, "%s: output is not a permutation of the input", alg->name);
	CHECK(s.max == input->max, NULL, NULL, "%s: max changed from %d to %d", alg->name, input->max, s.max);
	CHECK(s.swaps >= 0, input, NULL, "%s: negative swap count %ld", alg->name, s.swaps);
	/* even on sorted input every element has to be looked at once */
	CHECK(s.comparisons >= SAMPLE_SIZE - 1, input, NULL,
		  "%s: only %ld comparisons counted, at least %d expected", alg->name, s.comparisons, SAMPLE_SIZE - 1);
	return s;
}

/*---- TEST CASES ---------------------------*/
static void test_generators() {
	SAMPLES s;

	begin("generate ascending");
	sample_generate_ascending(&s);
	for (int i = 0; i < SAMPLE_SIZE; i++)
		CHECK(s.data[i] == i + 1, NULL, &s, "data[%d] is %d, expected %d", i, s.data[i], i + 1);
	CHECK(s.max == SAMPLE_SIZE, NULL, NULL, "max is %d, expected %d", s.max, SAMPLE_SIZE);
	CHECK(s.comparisons == 0 && s.swaps == 0, NULL, NULL, "statistics not reset");
	CHECK(s.sorted_until == -1 && s.sorted_from == SAMPLE_SIZE, NULL, NULL, "sorted region not reset");
	end();

	begin("generate descending");
	sample_generate_descending(&s);
	for (int i = 0; i < SAMPLE_SIZE; i++)
		CHECK(s.data[i] == SAMPLE_SIZE - i, NULL, &s, "data[%d] is %d, expected %d", i, s.data[i], SAMPLE_SIZE - i);
	CHECK(s.max == SAMPLE_SIZE, NULL, NULL, "max is %d, expected %d", s.max, SAMPLE_SIZE);
	CHECK(s.comparisons == 0 && s.swaps == 0, NULL, NULL, "statistics not reset");
	CHECK(s.sorted_until == -1 && s.sorted_from == SAMPLE_SIZE, NULL, NULL, "sorted region not reset");
	end();

	begin("generate random");
	SAMPLES ascending;
	sample_generate_ascending(&ascending);
	int position[SAMPLE_SIZE][SAMPLE_SIZE] = {{0}};	/* how often value v lands at index i */
	for (int run = 0; run < RANDOM_RUNS; run++) {
		sample_generate_random(&s);
		CHECK(same_values(&ascending, &s), NULL, &s, "random samples are not a permutation of 1..%d", SAMPLE_SIZE);
		CHECK(s.comparisons == 0 && s.swaps == 0, NULL, NULL, "statistics not reset after shuffle");
		CHECK(s.sorted_until == -1 && s.sorted_from == SAMPLE_SIZE, NULL, NULL, "sorted region not reset");
		for (int i = 0; i < SAMPLE_SIZE; i++) position[s.data[i]-1][i]++;
	}
	/* an unbiased shuffle puts each value at each index with probability 1/n:
	   allow 6 standard deviations around the expected count */
	double expected = (double)RANDOM_RUNS / SAMPLE_SIZE;
	double variance = expected * (1.0 - 1.0 / SAMPLE_SIZE);
	for (int v = 0; v < SAMPLE_SIZE; v++)
		for (int i = 0; i < SAMPLE_SIZE; i++) {
			double deviation = position[v][i] - expected;
			CHECK(deviation * deviation <= 36.0 * variance, NULL, NULL,
				  "value %d landed at index %d %d times, expected about %.0f (biased shuffle)",
				  v + 1, i, position[v][i], expected);
		}
	end();

	begin("is_sorted");
	sample_generate_ascending(&s);
	CHECK(sample_is_sorted(&s), NULL, &s, "ascending samples reported unsorted");
	generate_constant(&s, 5);
	CHECK(sample_is_sorted(&s), NULL, &s, "constant samples reported unsorted");
	sample_generate_descending(&s);
	CHECK(!sample_is_sorted(&s), NULL, &s, "descending samples reported sorted");
	sample_generate_ascending(&s);
	sample_swap(&s, SAMPLE_SIZE - 2, SAMPLE_SIZE - 1);
	CHECK(!sample_is_sorted(&s), NULL, &s, "last pair out of order reported sorted");
	end();
}

static void test_algorithm(const ALGORITHM * alg) {
	SAMPLES input, s;
	char name[64];

	printf("%s\n", alg->name);

	begin("already sorted");
	sample_generate_ascending(&input);
	s = run(alg, &input);
	if (alg->exchange)
		CHECK(s.swaps == 0, NULL, NULL, "%s: %ld swaps on sorted input, expected 0", alg->name, s.swaps);
	end();

	begin("reverse sorted");
	sample_generate_descending(&input);
	s = run(alg, &input);
	if (alg->adjacent)
		CHECK(s.swaps == PAIRS, NULL, NULL, "%s: %ld swaps on reversed input, expected %d", alg->name, s.swaps, PAIRS);
	end();

	begin("all equal");
	generate_constant(&input, SAMPLE_SIZE / 2);
	s = run(alg, &input);
	if (alg->exchange && alg != find("Quick Sort"))	/* Quick Sort swaps equal keys around the pivot */
		CHECK(s.swaps == 0, NULL, NULL, "%s: %ld swaps on equal input, expected 0", alg->name, s.swaps);
	end();

	begin("single element out of place");
	sample_generate_ascending(&input);
	for (int i = SAMPLE_SIZE - 1; i > 0; i--) sample_swap(&input, i, i - 1);	/* max value first */
	run(alg, &input);
	sample_generate_ascending(&input);
	for (int i = 0; i < SAMPLE_SIZE - 1; i++) sample_swap(&input, i, i + 1);	/* min value last */
	run(alg, &input);
	end();

	sprintf(name, "%d random permutations", RANDOM_RUNS);
	begin(name);
	for (int r = 0; r < RANDOM_RUNS && case_failures == 0; r++) {
		sample_generate_random(&input);
		s = run(alg, &input);
		if (alg->adjacent)
			CHECK(s.swaps == inversions(&input), &input, NULL,
				  "%s: %ld swaps, expected %ld (number of inversions)", alg->name, s.swaps, inversions(&input));
	}
	end();

	sprintf(name, "%d random inputs with duplicates", RANDOM_RUNS);
	begin(name);
	for (int r = 0; r < RANDOM_RUNS && case_failures == 0; r++) {
		if (r & 1) generate_duplicates(&input); else generate_with_values(&input);
		s = run(alg, &input);
		if (alg->adjacent)
			CHECK(s.swaps == inversions(&input), &input, NULL,
				  "%s: %ld swaps, expected %ld (number of inversions)", alg->name, s.swaps, inversions(&input));
	}
	end();
}

/* number of elements merge sort writes back: every merge of [l, r] writes r-l+1 */
static long merge_writes(int l, int r) {
	if (l >= r) return 0;
	int m = (l + r) >> 1;
	return merge_writes(l, m) + merge_writes(m + 1, r) + (r - l + 1);
}

/* algorithm specific bookkeeping that is known exactly */
static void test_statistics() {
	SAMPLES input, s;
	const ALGORITHM * alg;

	printf("Statistics\n");

	begin("interchange/selection compare all pairs");
	for (int r = 0; r < 100; r++) {
		sample_generate_random(&input);
		alg = find("Interchange Sort");
		s = run(alg, &input);
		CHECK(s.comparisons == PAIRS, &input, NULL, "%s: %ld comparisons, expected %d", alg->name, s.comparisons, PAIRS);
		alg = find("Selection Sort");
		s = run(alg, &input);
		CHECK(s.comparisons == PAIRS, &input, NULL, "%s: %ld comparisons, expected %d", alg->name, s.comparisons, PAIRS);
		CHECK(s.swaps <= SAMPLE_SIZE - 1, &input, NULL, "%s: %ld swaps, at most %d expected", alg->name, s.swaps, SAMPLE_SIZE - 1);
	}
	end();

	begin("single pass on sorted input");
	const char * single_pass[] = { "Bubble Sort", "Cocktail Sort", "Insertion Sort" };
	sample_generate_ascending(&input);
	for (int k = 0; k < 3; k++) {
		alg = find(single_pass[k]);
		s = run(alg, &input);
		CHECK(s.comparisons == SAMPLE_SIZE - 1, NULL, NULL,
			  "%s: %ld comparisons on sorted input, expected %d", alg->name, s.comparisons, SAMPLE_SIZE - 1);
	}
	end();

	begin("counting/radix pass sizes");
	sample_generate_random(&input);
	alg = find("Counting Sort");
	s = run(alg, &input);
	CHECK(s.comparisons == SAMPLE_SIZE && s.swaps == 2 * SAMPLE_SIZE, NULL, NULL,
		  "%s: %ld counted / %ld placed, expected %d / %d", alg->name, s.comparisons, s.swaps, SAMPLE_SIZE, 2 * SAMPLE_SIZE);
	int digits = 0;			/* one counting pass per decimal digit of the largest value */
	for (int m = input.max; m > 0; m /= 10) digits++;
	alg = find("Radix Sort");
	s = run(alg, &input);
	CHECK(s.comparisons == digits * SAMPLE_SIZE && s.swaps == 2 * digits * SAMPLE_SIZE, NULL, NULL,
		  "%s: %ld counted / %ld placed, expected %d / %d",
		  alg->name, s.comparisons, s.swaps, digits * SAMPLE_SIZE, 2 * digits * SAMPLE_SIZE);
	end();

	begin("heap sort extraction swaps");
	alg = find("Heap Sort");
	generate_constant(&input, 3);		/* equal keys: heapify never swaps */
	s = run(alg, &input);
	CHECK(s.swaps == SAMPLE_SIZE - 1, NULL, NULL,
		  "%s: %ld swaps on equal input, expected %d (one per extracted root)", alg->name, s.swaps, SAMPLE_SIZE - 1);
	end();

	begin("merge sort writes");
	alg = find("Merge Sort");
	long writes = merge_writes(0, SAMPLE_SIZE - 1);
	for (int r = 0; r < 100; r++) {
		sample_generate_random(&input);
		s = run(alg, &input);
		CHECK(s.swaps == writes, &input, NULL, "%s: %ld writes, expected %ld", alg->name, s.swaps, writes);
	}
	end();
}

/* runs every algorithm with visualization on, discarding the output, so the
   renderer's buffers are exercised (build with -fsanitize=address to check) */
static void test_rendering() {
	SAMPLES input;
	bool sorted = true;

	printf("Rendering\n");
	begin("all algorithms with visualization on");
	fflush(stdout);

	int saved = dup(fileno(stdout));
	if (saved < 0 || freopen(NULL_DEVICE, "w", stdout) == NULL) {
		printf("skipped (cannot redirect output)\n");
		return;
	}

	ENABLE_VISUALIZATION = true;
	sample_generate_random(&input);
	for (int k = 0; k < ALGORITHM_COUNT; k++) {
		SAMPLES s = input;
		ALGORITHMS[k].sort(&s);
		sorted = sorted && sample_is_sorted(&s);
	}
	sample_show(&input, 0, SAMPLE_SIZE - 1, SAMPLE_SIZE / 2);
	ENABLE_VISUALIZATION = false;

	fflush(stdout);
	dup2(saved, fileno(stdout));
	close(saved);

	CHECK(sorted, &input, NULL, "an algorithm failed to sort with visualization on");
	end();
}

/*---- MAIN ---------------------------------*/
int main(int argc, char ** argv) {
	unsigned seed = (argc > 1) ? (unsigned)strtoul(argv[1], NULL, 10) : (unsigned)time(NULL);

#ifndef _WIN32
	setlocale(LC_ALL, "");			/* renderer prints Unicode blocks */
#endif
	srand(seed);
	set_shades(SHADE_RAINBOW);
	SAMPLE_SPEED = 0;
	ENABLE_VISUALIZATION = false;

	printf("SortVis test suite (seed %u, sample size %d)\n\n", seed, SAMPLE_SIZE);

	printf("Samples\n");
	test_generators();
	for (int k = 0; k < ALGORITHM_COUNT; k++)
		test_algorithm(&ALGORITHMS[k]);
	test_statistics();
	test_rendering();

	printf("\n%d checks, %d failed%s\n", checks, failures,
		   failures ? "" : " - all tests passed");
	if (failures) printf("Re-run with the same seed to reproduce: %s %u\n", argv[0], seed);
	return failures ? 1 : 0;
}
