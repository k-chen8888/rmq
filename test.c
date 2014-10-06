#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "rmq.h"


int main(int argc, char** argv){
	
	int arr_len = 0;
	int* arr = NULL;
	int out = 0;
	RQTable* rqt = NULL;
	FILE* fp = NULL;
	int c = 0, i = 0;
	char buffer[16];
	
	clock_t begin, end;
	
	
	/* Exit if no file to process or too many arguments */
	if(argc > 3) {
		printf("Too many arguments\n");
		return 1;
	} else if(argc < 3) {	
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
	end = clock();
	printf("Generated table in %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);
	print_table(rqt);
	
	/* Make some queries and time them */
	begin = clock();
	out = max_range(arr, arr_len);
	end = clock();
	printf("Maximum subvector in range (%d, %d) found in %f seconds\n", out[0], out[1], (double)(end - begin) / CLOCKS_PER_SEC);

	/* Clean up */
	fclose(fp);
	free(arr);
	free(out);
	
	
	return 0;

}
