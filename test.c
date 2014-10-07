#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "rmq.h"


void test(RQTable* rqt, int start, int end);


void test(RQTable* rqt, int start, int end){
	int out = 0;
	clock_t begin, done;
	
	begin = clock();
	out = rminq(rqt, start, end);
	done = clock();
	
	if (out > -1) {
		printf("Found the minimum A[%d] = %d in range(%d, %d) in %f seconds\n", out, rqt->orig_list[out], start, end, (double)(done - begin) / CLOCKS_PER_SEC);
	} else {
		printf("Invalid query on range(%d, %d)\n", start, end);
	}
}


int main(int argc, char** argv){
	
	int arr_len = 0;
	int* arr = NULL;
	RQTable* rqt = NULL;
	FILE* fp = NULL;
	int c = 0, i = 0;
	char buffer[16];
	
	clock_t begin, done;
	
	int q1 = 0;
	int q2 = 0;
	
	/* Exit if no file to process or too many arguments */
	if(argc > 2) {
		printf("Too many arguments\n");
		return 1;
	} else if(argc < 2) {	
		printf("Not enough arguments\n");
		return 1;
	}
	
	/* Open file*/
	fp = fopen(argv[1], "r");
	
	/* Read array
	 * Expect each entry to occupy its own line
	 */
	c = fgetc(fp);
	while(c != EOF) {
		if(c == (int)'\n'){
			arr_len += 1;
		}
		
		c = fgetc(fp);
	}
	
	/* Allocate space for the array */
	arr = (int*)calloc( arr_len, sizeof(int) );
	
	/* Bounce back to front of file
	 * End program if unable to do so */
	if(fseek(fp, 0, SEEK_SET) < 0){
		fclose(fp);
		free(arr);
		return 1;
	}
	
	/* Populate array */
	arr_len = 0;
	c = fgetc(fp);
	while(c != EOF) {
		switch(c){
			case (int)'\n': /* \n */
				buffer[i] = '\0';
				arr[arr_len] = atoi(buffer);
				
				memset(buffer, 0, 16);
				i = 0;
				arr_len += 1;
				break;
			case 32: /* Space */
				break;
			default:
				buffer[i] = (char)c;
				i++;
		}
		
		c = fgetc(fp);
	}
	
	printf("[");
	for(i = 0; i < arr_len; i++){
		if(i < arr_len - 1) {
			printf("%d, ", arr[i]);
		} else {
			printf("%d]\n", arr[i]);
		}
	}
	
	/* Build RQTable and display it */
	begin = clock();
	rqt = rq_table(arr, arr_len, T_MIN);
	done = clock();
	printf("Generated table in %f seconds\n", (double)(done - begin) / CLOCKS_PER_SEC);
	print_table(rqt);
	
	/* Make some queries and time them */
	q1 = 2;
	q2 = 9;
	test(rqt, q1, q2);
	
	q1 = 2;
	q2 = 4;
	test(rqt, q1, q2);

	q1 = 0;
	q2 = 25;
	test(rqt, q1, q2);
	
	q1 = 0;
	q2 = 24;
	test(rqt, q1, q2);

	q1 = 7;
	q2 = 16;
	test(rqt, q1, q2);
	
	q1 = -1;
	q2 = 12;
	test(rqt, q1, q2);
	
	q1 = 17;
	q2 = 20;
	test(rqt, q1, q2);
	
	q1 = 15;
	q2 = 16;
	test(rqt, q1, q2);
	
	q1 = 9;
	q2 = 23;
	test(rqt, q1, q2);
	
	
	/* Clean up */
	fclose(fp);
	free(arr);
	destr_table(rqt);
	
	
	return 0;

}
