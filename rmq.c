#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* Build a Range Query table (effective RQTable constructor)
 * 0 == min, 1 == max
 */
RQTable* rq_table(int* list, int list_len, int mode) {
	RQTable* out = NULL;
	int i = 0;
	
	/* Create table */
	out = (RQTable*)calloc(1, sizeof(RQTable*));
	out->mode = mode;
	out->orig_list = list;
	
	/* Height of table is log width of list (and one extra for original) */
	out->height = (int)ceil(log((double)list_len)/log(2.0)) + 1;
	/* Width is given for first row, calculated for all others */
	out->width = (int*)calloc(out->height, sizeof(int));
	out->width[0] = list_len;
	for(i = 1; i < out->height; i++) {
		out->width[i] = (int)ceil(out->width[i - 1] / 2.0);
	}
	
	/* Generate table in O(n \log n) time */
	out->table = (int**)calloc(tbl_height, sizeof(int*));
	/* Allocate space for every row */
	for(i = 0; i < out->height; i++) {
		out->table[i] = (int*)calloc(out->width[i], sizeof(int));
	}
	/* Populate table
	 * First row stores its index */
	i = 0;
	for(i = 0; i < out->width[0]; i++){
		out->table[0][i] = i;
	}
	while(i + 1 < out->height){
		populate(out, i, i+1);
		
		i += 2;
	}
	
	return out;
}


/* Helper function to populate a RQTable
 * Uses r1 to build r2 */
void populate(RQTable* rqt, int r1, int r2) {
	int i = 0;
	int j = 0;

	int index1 = 0;
	int index2 = 0;

	for(i = 0; i < rqt->width[r1]; i += 2){
		index1 = rqt->table[r1][i];
		index2 = rqt->table[r1][i + 1];
		
		rqt->table[r2][j] = (rqt->orig_list[index1] < rqt->orig_list[index2] ? index1 : index2);
		j++;
	}
}


/* Get k in range(start, end) such that A[k] is minimized
 * Constant time using 2 table entries */
int rminq(RQTable* rqt, int start, int end) {
	int row = 0;
	
	int s = 0;
	int e = 0;
	
	int index1 = 0;
	int index2 = 0;
	
	if(rqt->mode != T_MIN){
		/* Check if correct table was given */
		return -1;
	} else if (end < start || end < 0 || start < 0 || end >= rqt->width[0] || start >= rqt->width[0]) {
		/* Check if valid range was given */
		return -1;
	} else if (end == start){
		/* Handle simplest case */
		return rqt->table[0][start];
	} else {}
	
	/* Find out which row to operate on */
	row = (int)(log(end - start)/log(2.0)) - 1;
	
	/* Handle simple cases (first row) */
	if(row == 0){
		if(end - start == 1){
			return (rqt->orig_list[start] <= rqt->orig_list[end] ? start : end);
		} else {
			return (rqt->orig_list[start] <= rqt->orig_list[end] ? (rqt->orig_list[start] <= rqt->orig_list[start+1] ? start : start+1) : (rqt->orig_list[end-1] <= rqt->orig_list[end] ? end-1 : end) );
		}
	} else {
		/* Floor the log of the indices to find which elements to check */
		s = (int)floor(log((double)start)/log(2.0));
		e = (int)floor(log((double)end)/log(2.0));
		
		index1 = rqt->table[row][s];
		index2 = rqt->table[row][e];
		
		return (rqt->orig_list[index1] <= rqt->orig_list[index2] ? index1 : index2);
	}
	
	/* Somehow failed in some other way */
	return -1;
}


/* Destroys RQTable */
void destr_table(RQTable* rqt){
	int i = 0;
	
	for(i = 0; i < rqt->height) {
		free(rqt->table[i]);
	}
	
	free(rqt->table);
	free(rqt->width);
	free(rqt);
}
