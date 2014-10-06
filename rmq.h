#ifndef RMQ_H
#define RMQ_H


/* Range Query table mode constants */
#define T_MIN 0
#define T_MAX 1


/* Stores a Range Query table and its dimensions */
struct RQTable_ {
	int mode;
	int height;
	int* width;
	int* orig_list;
	int** table;
};
typedef struct RQTable_ RQTable;


/* Build a Range Query table (effective RQTable constructor)
 * 0 == min, 1 == max
 */
RQTable* rq_table(int* list, int list_len, int mode);


/* Helper function to populate a RQTable
 * Uses r1 to build r2 */
void populate(RQTable* rqt, int r1, int r2);


/* Displays information about the table */
void print_table(RQTable* rqt);


/* Get k in range(start, end) such that A[k] is minimized
 * Constant time using 2 table entries */
int rminq(RQTable* rqt, int start, int end);


/* Destroys RQTable */
void destr_table(RQTable* rqt);


#endif
