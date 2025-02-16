#ifndef __SORTVIS_ALGS__
#define __SORTVIS_ALGS__

/*---- SORT SAMPLES HANDLERS ---------------------*/
void sample_swap(SAMPLES * s, int a, int b) {
	int t = s->data[a];
	s->data[a] = s->data[b];
	s->data[b] = t;
}

int sample_height(SAMPLES * s, int i) {
	return 	(s->max-s->data[i]);
}

void sample_generate_ascending(SAMPLES * s) {
	int i;
	for (i = 0; i < SAMPLE_SIZE; i++) s->data[i] = i+1;
	s->max = i;
}

void sample_generate_descending(SAMPLES * s) {
	int i;
	for (i = 0; i < SAMPLE_SIZE; i++) s->data[i] = SAMPLE_SIZE-i;
	s->max = i;
}

void sample_generate_random(SAMPLES * s) {
	int i;
	sample_generate_ascending(s);
	for (i = 0; i < SAMPLE_SIZE; i++)	/* shuffling */
		sample_swap(s, rand() % SAMPLE_SIZE, rand() % SAMPLE_SIZE);
}

bool sample_generate(SAMPLES * s) {
	char choice;
	
	clear();
	printf(VT_COLOR(220)"GENERATE NEW SAMPLES\n");
	printf(     VT_RESET"--------------------\n");
	printf(VT_COLOR(150)"A"VT_RESET". Randomized\n");
	printf(VT_COLOR(150)"B"VT_RESET". Acending\n");
	printf(VT_COLOR(150)"C"VT_RESET". Descending\n");
	printf(     VT_RESET"--------------------\n");
	printf(VT_COLOR(170)"D"VT_RESET". Back\n");
	printf(VT_RESET"Choice: ");
	fflush(stdin);
	scanf("%c", &choice);
	choice = toupper(choice);
	switch (choice) {
	case 'A': sample_generate_random(s); break;
	case 'B': sample_generate_ascending(s); break;
	case 'C': sample_generate_descending(s); break;
	case 'D': return false;
	}
	return true;
}

void sample_show(SAMPLES * s, int u, int v, int t){
	int i, j, c;
	char temp[1024], buffer[64], sep[BOARD_WIDTH+1];
	
	/* setup horizontal line */
	for (i = 0; i < BOARD_WIDTH; i++) 
		sep[i] = VBAR;
	sep[BOARD_WIDTH] = 0;

	/* draw sort algorithm title */	
	puts(sortTitle);
	
	/* draw top horz, line */
	strcpy(temp, VT_COLOR(8));
	strcat(temp, sep);
	puts(temp);

	/* draw sort samples */
	for (j = 0; j < s->max; j++) {
		temp[0] = 0;
		for (i = 0; i < SAMPLE_SIZE; i++) {
			c = (j < sample_height(s, i)) ? VSHADE : VBLOCK;		
			sprintf(buffer, " %s%lc%lc ", colors[s->data[i]-1], (wint_t) c, (wint_t) c);
			strcat(temp, buffer);
		}
		puts(temp);
	}
	
	/* draw bottom horz. line */
	strcpy(temp, VT_COLOR(8));
	strcat(temp, sep);
	puts(temp);
	
	/* draw sort values */
	temp[0] = 0;
	for (i = 0; i < SAMPLE_SIZE; i++) {
		if (i == u || i == v)
			sprintf(buffer, "%s%3d ", VT_COLOR(231), s->data[i]);
		else
			sprintf(buffer, "%s%3d ", VT_COLOR(244), s->data[i]);			
		strcat(temp, buffer);
	}
	puts(temp);

	/* draw current indices */
	strcpy(temp, VT_COLOR(231));
	for (i = 0; i < SAMPLE_SIZE; i++) {
		if (i == u) c = VCURRENT; else 
		if (i == v) c = VRUNNING; else 
		if (i == t) c = VEXTRA; else 
		c = 32;
		sprintf(buffer, " %lc%lc ", (wint_t)c, (wint_t)c);
		strcat(temp, buffer);
	}
	puts(temp);
	reset_colors();
}
/*---- INTERCHANGE SORT --------------------*/
void sample_sort_interchange(SAMPLES * s) {
	int i, j;
    title("INTERCHANGE SORT");
	for (i = 0; i < SAMPLE_SIZE-1; i++) {
		for (j = i + 1; j < SAMPLE_SIZE; j++) {
			if (s->data[i] > s->data[j]) {
				sample_swap(s, i, j);
			}
			sample_show(s, i, j, -1);
			mssleep(SAMPLE_SPEED);
		}
	}
	sample_show(s, -1, -1, -1);
}
/*---- SELECTION SORT ----------------------*/
void sample_sort_selection(SAMPLES * s) {
	int i, j, minidx;
    title("SELECTION SORT");
	for (i = 0; i < SAMPLE_SIZE-1; i++) {
		minidx = i;
		for (j = i + 1; j < SAMPLE_SIZE; j++) {
			if (s->data[j] < s->data[minidx])
				minidx = j;
			sample_show(s, i, j, minidx);
			mssleep(SAMPLE_SPEED);
		}
		if (minidx != i)
			sample_swap(s, i, minidx);
		sample_show(s, i, j, minidx);
	}
	sample_show(s, -1, -1, -1);
}
/*---- BUBBLE SORT -------------------------*/
void sample_sort_bubble(SAMPLES * s) {
	int i, j;
	bool swapped;
    title("BUBBLE SORT");
	for (i = 0; i < SAMPLE_SIZE-1; i++) {
		swapped = false;
		for (j = 0; j < SAMPLE_SIZE-i-1; j++) {
			if (s->data[j] > s->data[j+1]) {
				sample_swap(s, j, j+1);
				swapped = true;
			}
			sample_show(s, j, j+1, -1);
			mssleep(SAMPLE_SPEED);
		}
		if (!swapped) break;
	}
	sample_show(s, -1, -1, -1);
}
/*---- INSERTION SORT ----------------------*/
void sample_sort_insertion(SAMPLES * s) {
	int i, j, key;
    title("INSERTION SORT");
	for (i = 1; i < SAMPLE_SIZE; i++) {
		key = s->data[i];
		j = i-1;
		while (j >= 0 && s->data[j] > key) {
			s->data[j+1] = s->data[j];
			j--;
			sample_show(s, j, j+1, -1);
			mssleep(SAMPLE_SPEED);
		}
		s->data[j+1] = key;
		sample_show(s, i, j+1, -1);
		mssleep(SAMPLE_SPEED);
	}
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
                s->data[j] = s->data[j - gap];
				sample_show(s, j, j-gap, -1);
				mssleep(SAMPLE_SPEED);
			}
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

    if (left < n && s->data[left] > s->data[largest])   largest = left;
    if (right < n && s->data[right] > s->data[largest]) largest = right;

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
        while (pivot >= s->data[i]) i++;
        while (pivot < s->data[j]) j--;
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
  
void sample_sort_quick(SAMPLES * s, int low, int high) {
	title("QUICK SORT");
    if (low < high) {
        int pivot = partition(s, low, high);
        sample_sort_quick(s, low, pivot - 1);
        sample_sort_quick(s, pivot + 1, high);
	    sample_show(s, low, high, pivot);
    }
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
        if (L[i] <= R[j]) {
            s->data[k] = L[i];
            i++;
        }
        else {
            s->data[k] = R[j];
            j++;
        }
        k++;
	    sample_show(s, l, r, m);
        mssleep(SAMPLE_SPEED);
    }
    while (i < n1) {  
        s->data[k] = L[i];
        i++;
        k++;
	    sample_show(s, l, r, m);
        mssleep(SAMPLE_SPEED);
    }
    while (j < n2) {  
        s->data[k] = R[j];
        j++;
        k++;
	    sample_show(s, l, r, m);
        mssleep(SAMPLE_SPEED);
    }
}

void sample_sort_merge(SAMPLES * s, int l, int r) {
    title("MERGE SORT");
    if (l < r) {
        int m = (l + r) >> 1; 
        sample_sort_merge(s, l, m);
        sample_sort_merge(s, m + 1, r);
        merge(s, l, m, r);
    }
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
		sample_show(s, i, -1, -1);
    	mssleep(SAMPLE_SPEED);
	}
    for (i = 1; i <= SAMPLE_SIZE; ++i)
        count[i] += count[i - 1];
    for (i = 0; i < SAMPLE_SIZE; ++i) {
        output[count[s->data[i]] - 1] = s->data[i];
        --count[s->data[i]];
		sample_show(s, -1, i, -1);
    	mssleep(SAMPLE_SPEED);

    }
    for (i = 0; i < SAMPLE_SIZE; ++i) {    	
		s->data[i] = output[i];
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
	}

	for (int i = 1; i < 10; i++)
		count[i] += count[i - 1];

	for (int i = SAMPLE_SIZE - 1; i >= 0; i--) {
		output.data[count[(s->data[i] / exp) % 10] - 1] = s->data[i];
		count[(s->data[i] / exp) % 10]--;
		sample_show(s, i, -1, -1);
    	mssleep(SAMPLE_SPEED);
	}

	for (int i = 0; i < SAMPLE_SIZE; i++) {
		s->data[i] = output.data[i];
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