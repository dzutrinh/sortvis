#ifndef __SORTVIS_ALGS__
#define __SORTVIS_ALGS__

/*---- GLOBAL FLAGS --------------------------------*/
bool ENABLE_VISUALIZATION = true;	/* set to false to disable visualization output */

/*---- FORWARD DECLARATIONS ------------------------*/
void sample_show(SAMPLES * s, int u, int v, int t);

/*---- SORT SAMPLES HANDLERS ---------------------*/
void sample_swap(SAMPLES * s, int a, int b) {
	int t = s->data[a];
	s->data[a] = s->data[b];
	s->data[b] = t;
	s->swaps++;	/* track swap count */
}

void sample_swap_animated(SAMPLES * s, int a, int b, int u, int v, int t) {
	/* Animated swap with smooth transition */
	if (!SMOOTH_TRANSITIONS || !ENABLE_VISUALIZATION) {
		sample_swap(s, a, b);
		return;
	}
	
	/* Show highlighting before swap */
	for (int frame = 0; frame < TRANSITION_FRAMES; frame++) {
		sample_show(s, a, b, t);
		mssleep(SAMPLE_SPEED / (TRANSITION_FRAMES + 1));
	}
	
	/* Perform actual swap */
	sample_swap(s, a, b);
	
	/* Show result with flash effect */
	sample_show(s, a, b, t);
	mssleep(SAMPLE_SPEED / (TRANSITION_FRAMES + 1));
}

int sample_height(SAMPLES * s, int i) {
	return 	(s->max-s->data[i]);
}

void sample_generate_ascending(SAMPLES * s) {
	int i;
	for (i = 0; i < SAMPLE_SIZE; i++) s->data[i] = i+1;
	s->max = i;
	s->comparisons = 0;
	s->swaps = 0;
	s->sorted_until = -1;
}

void sample_generate_descending(SAMPLES * s) {
	int i;
	for (i = 0; i < SAMPLE_SIZE; i++) s->data[i] = SAMPLE_SIZE-i;
	s->max = i;
	s->comparisons = 0;
	s->swaps = 0;
	s->sorted_until = -1;
}

void sample_generate_random(SAMPLES * s) {
	int i;
	sample_generate_ascending(s);
	for (i = 0; i < SAMPLE_SIZE; i++)	/* shuffling */
		sample_swap(s, rand() % SAMPLE_SIZE, rand() % SAMPLE_SIZE);
	/* Reset stats after shuffle */
	s->comparisons = 0;
	s->swaps = 0;
	s->sorted_until = -1;
}

void sample_validate(SAMPLES * s) {
	/* Ensure samples are within valid range */
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		if (s->data[i] < 1 || s->data[i] > SAMPLE_SIZE) {
			fprintf(stderr, "Warning: Invalid sample value %d at index %d\n", s->data[i], i);
			s->data[i] = (s->data[i] < 1) ? 1 : SAMPLE_SIZE;
		}
	}
}

bool sample_generate(SAMPLES * s) {
	char choice;
	int selected = 0;
	const char* items[] = {
		"A. Randomized",
		"B. Ascending",
		"C. Descending",
		"",
		"D. Back"
	};
	
	while (1) {
		clear();
		printf(VT_COLOR(220)"GENERATE NEW SAMPLES\n");
		printf(VT_RESET"--------------------\n");
		
		for (int i = 0; i < 5; i++) {
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
		
		printf(VT_RESET"\nUse "VT_ATTR(33)"UP/DOWN"VT_DEFAULTATTR" arrows or "VT_ATTR(33)"A-D"VT_DEFAULTATTR", press "VT_ATTR(33)"ENTER"VT_DEFAULTATTR" to select\n");
		fflush(stdout);
		
		int ch = getch_arrow();
		
		/* Handle arrow keys */
		if (ch == 'U') {  /* Up arrow */
			do {
				selected = (selected - 1 + 5) % 5;
			} while (selected == 3);  /* Skip separator */
			continue;
		} else if (ch == 'D') {  /* Down arrow */
			do {
				selected = (selected + 1) % 5;
			} while (selected == 3);  /* Skip separator */
			continue;
		} else if (ch == '\n' || ch == '\r') {  /* Enter */
			const char mapping[] = "ABCD";
			choice = (selected == 4) ? 'D' : mapping[selected];
		} else {
			choice = toupper(ch & 0xFF);
		}
		
		switch (choice) {
		case 'A': sample_generate_random(s); return true;
		case 'B': sample_generate_ascending(s); return true;
		case 'C': sample_generate_descending(s); return true;
		case 'D': return false;
		default: continue;
		}
	}
}

bool sample_is_sorted(SAMPLES * s) {
	for (int i = 0; i < SAMPLE_SIZE - 1; i++) {
		if (s->data[i] > s->data[i + 1])
			return false;
	}
	return true;
}

void sample_show(SAMPLES * s, int u, int v, int t){
	int i, j, c;
	char temp[1024], buffer[128], sep[BOARD_WIDTH+1];
	static char screen[16384];  /* Large buffer for entire screen */
	static int last_u = -1, last_v = -1;  /* Track last swap for highlighting */
	
	/* skip visualization if disabled */
	if (!ENABLE_VISUALIZATION) return;
	
	/* Clear screen buffer */
	screen[0] = 0;
	
	/* setup horizontal line */
	for (i = 0; i < BOARD_WIDTH; i++) 
		sep[i] = VBAR;
	sep[BOARD_WIDTH] = 0;

	/* Add sort algorithm title to buffer */
	strcat(screen, sortTitle);
	strcat(screen, "\n");
	
	/* Add top horz. line to buffer */
	sprintf(temp, "%s%s\n", VT_COLOR(8), sep);
	strcat(screen, temp);

	/* Add sort samples to buffer */
	for (j = 0; j < s->max; j++) {
		temp[0] = 0;
		for (i = 0; i < SAMPLE_SIZE; i++) {
			c = (j < sample_height(s, i)) ? VSHADE : VBLOCK;
			sprintf(buffer, " %s%lc%lc ", colors[s->data[i]-1], (wint_t) c, (wint_t) c);
			strcat(temp, buffer);
		}
		strcat(temp, "\n");
		strcat(screen, temp);
	}
	
	/* Add bottom horz. line to buffer */
	sprintf(temp, "%s%s\n", VT_COLOR(8), sep);
	strcat(screen, temp);
	
	/* Add sort values to buffer with enhanced highlighting */
	temp[0] = 0;
	for (i = 0; i < SAMPLE_SIZE; i++) {
		if (i == u || i == v) {
			/* Current comparison - bright yellow */
			sprintf(buffer, "%s%s%3d%s ", VT_ATTR(1), VT_COLOR(226), s->data[i], VT_DEFAULTATTR);
		} else if (SHOW_SORTED_REGION && s->sorted_until >= 0 && i <= s->sorted_until) {
			/* Sorted region - green */
			sprintf(buffer, "%s%3d%s ", VT_COLOR(46), s->data[i], VT_DEFAULTATTR);
		} else if (HIGHLIGHT_SWAPS && (i == last_u || i == last_v)) {
			/* Just swapped - cyan */
			sprintf(buffer, "%s%3d%s ", VT_COLOR(51), s->data[i], VT_DEFAULTATTR);
		} else {
			/* Normal - gray */
			sprintf(buffer, "%s%3d ", VT_COLOR(244), s->data[i]);
		}
		strcat(temp, buffer);
	}
	strcat(temp, "\n");
	strcat(screen, temp);
	
	/* Update last swap positions */
	if (u != last_u || v != last_v) {
		last_u = u;
		last_v = v;
	}

	/* Add current indices to buffer */
	strcpy(temp, VT_COLOR(231));
	for (i = 0; i < SAMPLE_SIZE; i++) {
		if (i == u) c = VCURRENT; else 
		if (i == v) c = VRUNNING; else 
		if (i == t) c = VEXTRA; else 
		c = 32;
		sprintf(buffer, " %lc%lc ", (wint_t)c, (wint_t)c);
		strcat(temp, buffer);
	}
	strcat(temp, "\n");
	strcat(screen, temp);
	
	/* Add statistics bar if enabled */
	if (SHOW_STATISTICS) {
		sprintf(temp, "%s%s", VT_COLOR(8), sep);
		strcat(screen, "\n");
		strcat(screen, temp);
		sprintf(temp, "\n%sComparisons: %s%-8ld%s  Swaps: %s%-8ld%s  Speed: %dms\n",
				VT_COLOR(244), VT_COLOR(39), s->comparisons, 
				VT_COLOR(244), VT_COLOR(196), s->swaps,
				VT_COLOR(244), SAMPLE_SPEED);
		strcat(screen, temp);
	}
	
	/* Write entire screen at once (atomic operation) */
	fputs(screen, stdout);
	fflush(stdout);
	
	reset_colors();
}
/*---- INTERCHANGE SORT --------------------*/
void sample_sort_interchange(SAMPLES * s) {
	int i, j;
    title("INTERCHANGE SORT");
	for (i = 0; i < SAMPLE_SIZE-1; i++) {
		s->sorted_until = i - 1;  /* Track sorted region */
		for (j = i + 1; j < SAMPLE_SIZE; j++) {
			s->comparisons++;
			if (s->data[i] > s->data[j]) {
				if (SMOOTH_TRANSITIONS) {
					sample_swap_animated(s, i, j, i, j, -1);
				} else {
					sample_swap(s, i, j);
					sample_show(s, i, j, -1);
					mssleep(SAMPLE_SPEED);
				}
			} else {
				sample_show(s, i, j, -1);
				mssleep(SAMPLE_SPEED);
			}
		}
	}
	s->sorted_until = SAMPLE_SIZE - 1;
	sample_show(s, -1, -1, -1);
}
/*---- SELECTION SORT ----------------------*/
void sample_sort_selection(SAMPLES * s) {
	int i, j, minidx;
    title("SELECTION SORT");
	for (i = 0; i < SAMPLE_SIZE-1; i++) {
		s->sorted_until = i - 1;  /* Elements before i are sorted */
		minidx = i;
		for (j = i + 1; j < SAMPLE_SIZE; j++) {
			s->comparisons++;
			if (s->data[j] < s->data[minidx])
				minidx = j;
			sample_show(s, i, j, minidx);
			mssleep(SAMPLE_SPEED);
		}
		if (minidx != i) {
			if (SMOOTH_TRANSITIONS) {
				sample_swap_animated(s, i, minidx, i, j, minidx);
			} else {
				sample_swap(s, i, minidx);
			}
		}
		sample_show(s, i, j, minidx);
	}
	s->sorted_until = SAMPLE_SIZE - 1;
	sample_show(s, -1, -1, -1);
}
/*---- BUBBLE SORT -------------------------*/
void sample_sort_bubble(SAMPLES * s) {
	int i, j;
	bool swapped;
    title("BUBBLE SORT");
	for (i = 0; i < SAMPLE_SIZE-1; i++) {
		swapped = false;
		s->sorted_until = SAMPLE_SIZE - i - 1;  /* Elements after this are sorted */
		for (j = 0; j < SAMPLE_SIZE-i-1; j++) {
			s->comparisons++;
			if (s->data[j] > s->data[j+1]) {
				if (SMOOTH_TRANSITIONS) {
					sample_swap_animated(s, j, j+1, j, j+1, -1);
				} else {
					sample_swap(s, j, j+1);
					sample_show(s, j, j+1, -1);
					mssleep(SAMPLE_SPEED);
				}
				swapped = true;
			} else {
				sample_show(s, j, j+1, -1);
				mssleep(SAMPLE_SPEED);
			}
		}
		if (!swapped) break;
	}
	s->sorted_until = SAMPLE_SIZE - 1;
	sample_show(s, -1, -1, -1);
}
/*---- INSERTION SORT ----------------------*/
void sample_sort_insertion(SAMPLES * s) {
	int i, j, key;
    title("INSERTION SORT");
	for (i = 1; i < SAMPLE_SIZE; i++) {
		s->sorted_until = i - 1;  /* Elements 0 to i-1 are sorted */
		key = s->data[i];
		j = i-1;
		while (j >= 0 && s->data[j] > key) {
			s->comparisons++;
			s->swaps++;  /* Shifting is essentially a swap */
			s->data[j+1] = s->data[j];
			j--;
			sample_show(s, j, j+1, -1);
			mssleep(SAMPLE_SPEED);
		}
		if (j >= 0) s->comparisons++;  /* Count final comparison */
		s->data[j+1] = key;
		sample_show(s, i, j+1, -1);
		mssleep(SAMPLE_SPEED);
	}
	s->sorted_until = SAMPLE_SIZE - 1;
	sample_show(s, -1, -1, -1);
}
/*---- SHELL SORT --------------------------*/
void sample_sort_shell(SAMPLES * s) {
	int gap, i, temp;
    title("SHELL SORT");
    for (gap = SAMPLE_SIZE/2; gap > 0; gap /= 2) {
        for (i = gap; i < SAMPLE_SIZE; i++)
        {
            temp = s->data[i];
            int j;
            for (j = i; j >= gap && s->data[j - gap] > temp; j -= gap) {
                s->comparisons++;
                s->swaps++;
                s->data[j] = s->data[j - gap];
				sample_show(s, j, j-gap, -1);
				mssleep(SAMPLE_SPEED);
			}
            if (j >= gap) s->comparisons++;  /* Count final comparison */
            s->data[j] = temp;
			sample_show(s, i, j, -1);
			mssleep(SAMPLE_SPEED);
        }
    }
	sample_show(s, -1, -1, -1);
}
/*---- HEAP SORT ---------------------------*/
void heapify(SAMPLES * s, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        s->comparisons++;
        if (s->data[left] > s->data[largest]) largest = left;
    }
    if (right < n) {
        s->comparisons++;
        if (s->data[right] > s->data[largest]) largest = right;
    }

    if (largest != i) {
        sample_swap(s, i, largest);
		sample_show(s, i, n, largest);
		mssleep(SAMPLE_SPEED);
        heapify(s, n, largest);
    }
}

void sample_sort_heap(SAMPLES * s) {
	int i;
    title("HEAP SORT");
    for (i = SAMPLE_SIZE / 2 - 1; i >= 0; i--)
		heapify(s, SAMPLE_SIZE, i);
    for (i = SAMPLE_SIZE - 1; i >= 0; i--) {
        sample_swap(s, 0, i);
        sample_show(s, 0, i, -1);
        mssleep(SAMPLE_SPEED);
        heapify(s, i, 0);
    }
    sample_show(s, -1, -1, -1);
} 
/*---- QUICK SORT --------------------------*/
int partition(SAMPLES * s, int low, int high)
{
    int i = low, j = high, pivot = s->data[low];
    while (i < j) {
        while (i < high && pivot >= s->data[i]) {
            s->comparisons++;
            i++;
        }
        while (j > low && pivot < s->data[j]) {
            s->comparisons++;
            j--;
        }
        if (i < j) {
			sample_swap(s, i, j);
    	}
        sample_show(s, i, j, pivot);
   	    mssleep(SAMPLE_SPEED);
    }
    sample_swap(s, low, j);
    sample_show(s, low, j, pivot);
   	mssleep(SAMPLE_SPEED);
    return j;
}
  
void sample_sort_quick_recursive(SAMPLES * s, int low, int high) {
    if (low < high) {
        int pivot = partition(s, low, high);
        sample_sort_quick_recursive(s, low, pivot - 1);
        sample_sort_quick_recursive(s, pivot + 1, high);
	    sample_show(s, low, high, pivot);
    }
}

void sample_sort_quick(SAMPLES * s, int low, int high) {
	title("QUICK SORT");
	sample_sort_quick_recursive(s, low, high);
    sample_show(s, -1, -1, -1);
} 
/*---- MERGE SORT --------------------------*/
void merge(SAMPLES * s, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2]; 
  
    for (i = 0; i < n1; i++) L[i] = s->data[l + i];
    for (j = 0; j < n2; j++) R[j] = s->data[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        s->comparisons++;
        if (L[i] <= R[j]) {
            s->data[k] = L[i];
            i++;
        }
        else {
            s->data[k] = R[j];
            j++;
        }
        s->swaps++;
        k++;
	    sample_show(s, l, r, m);
        mssleep(SAMPLE_SPEED);
    }
    while (i < n1) {  
        s->data[k] = L[i];
        s->swaps++;
        i++;
        k++;
	    sample_show(s, l, r, m);
        mssleep(SAMPLE_SPEED);
    }
    while (j < n2) {  
        s->data[k] = R[j];
        s->swaps++;
        j++;
        k++;
	    sample_show(s, l, r, m);
        mssleep(SAMPLE_SPEED);
    }
}

void sample_sort_merge_recursive(SAMPLES * s, int l, int r) {
    if (l < r) {
        int m = (l + r) >> 1; 
        sample_sort_merge_recursive(s, l, m);
        sample_sort_merge_recursive(s, m + 1, r);
        merge(s, l, m, r);
    }
}

void sample_sort_merge(SAMPLES * s, int l, int r) {
    title("MERGE SORT");
    sample_sort_merge_recursive(s, l, r);
    sample_show(s, -1, -1, -1);
} 
/*---- COMB SORT ----------------------------*/
void sample_sort_comb(SAMPLES * s)  { 
    int gap = SAMPLE_SIZE, i;
	bool swapped = true; 
    title("COMB SORT");
    while (gap != 1 || swapped)  { 
	    gap = (gap*10)/13; 
    	if (gap < 1) gap = 1; 
        swapped = false; 
        for (i = 0; i < SAMPLE_SIZE - gap; i++)  { 
            s->comparisons++;
            if (s->data[i] > s->data[i+gap]) { 
                sample_swap(s, i, i+gap); 
                swapped = true;
            } 
	        sample_show(s, i, i+gap, -1);
    	    mssleep(SAMPLE_SPEED);
        } 
    } 
	sample_show(s, -1, -1, -1);
} 
/*---- COUNTING SORT -----------------------*/
void sample_sort_count(SAMPLES * s) {
	int output[SAMPLE_SIZE]; 
    int count[SAMPLE_SIZE + 1], i;
    title("COUNTING SORT");
	
	for (i = 0; i <= SAMPLE_SIZE; i++) count[i] = 0;
    for (i = 0; i < SAMPLE_SIZE; ++i) {
		++count[s->data[i]];
		s->comparisons++;  /* Counting operation */
		sample_show(s, i, -1, -1);
    	mssleep(SAMPLE_SPEED);
	}
    for (i = 1; i <= SAMPLE_SIZE; ++i)
        count[i] += count[i - 1];
    for (i = 0; i < SAMPLE_SIZE; ++i) {
        output[count[s->data[i]] - 1] = s->data[i];
        --count[s->data[i]];
        s->swaps++;  /* Placement operation */
		sample_show(s, -1, i, -1);
    	mssleep(SAMPLE_SPEED);

    }
    for (i = 0; i < SAMPLE_SIZE; ++i) {    	
		s->data[i] = output[i];
		s->swaps++;
		sample_show(s, i, -1, -1);
    	mssleep(SAMPLE_SPEED);
	}
    sample_show(s, -1, -1, -1);
}
/*---- COCKTAIL SORT -----------------------*/
void sample_sort_cocktail(SAMPLES * s) {	
    int swapped = 1, start = 0, end = SAMPLE_SIZE - 1, i;
    title("COCKTAIL SORT");
    while (swapped) {
        swapped = 0;
        for (i = start; i < end; i++) {
            s->comparisons++;
            if (s->data[i] > s->data[i + 1]) {
                sample_swap(s, i, i + 1);
                swapped = 1;
            }
			sample_show(s, i, i+1, -1);
    		mssleep(SAMPLE_SPEED);
        }
 
        if (!swapped) break;
        swapped = 0;
        --end;
 
        for (i = end - 1; i >= start; i--) {
            s->comparisons++;
            if (s->data[i] > s->data[i + 1]) {
                sample_swap(s, i, i + 1);
                swapped = 1;
            }
			sample_show(s, i, i+1, -1);
    		mssleep(SAMPLE_SPEED);
        }
        ++start;
    }
    sample_show(s, -1, -1, -1);
}
/*---- RADIX SORT -----------------------*/
void count_sort_radix(SAMPLES * s, int exp) {
	SAMPLES output;
	int count[10] = { 0 };

	output.max = s->max;
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		output.data[i] = 1;
		count[(s->data[i] / exp) % 10]++;
		s->comparisons++;  /* Classification operation */
	}

	for (int i = 1; i < 10; i++)
		count[i] += count[i - 1];

	for (int i = SAMPLE_SIZE - 1; i >= 0; i--) {
		output.data[count[(s->data[i] / exp) % 10] - 1] = s->data[i];
		count[(s->data[i] / exp) % 10]--;
		s->swaps++;  /* Placement operation */
		sample_show(s, i, -1, -1);
    	mssleep(SAMPLE_SPEED);
	}

	for (int i = 0; i < SAMPLE_SIZE; i++) {
		s->data[i] = output.data[i];
		s->swaps++;
		sample_show(s, i, -1, -1);
    	mssleep(SAMPLE_SPEED);
	}
}

void sample_sort_radix(SAMPLES * s) {	
    title("RADIX SORT");
	int m = s->max;
	for (int exp = 1; m / exp > 0; exp *= 10) {
		count_sort_radix(s, exp);
	}
    sample_show(s, -1, -1, -1);
}

#endif