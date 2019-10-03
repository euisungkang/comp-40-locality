#include <stdio.h>
#include "uarray2b.h"
//#include "uarray2.h"

const int col = 5;
const int row = 5;

void apply(int col, int row, UArray2b_T array2b, void *elem, void *cl);

int main()
{
	UArray2b_T array = UArray2b_new(col, row, sizeof(int), 2);

	
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			int *temp = UArray2b_at(array, i, j);
			//printf("%p\n", (void*)temp);
			*temp = count++;
		}
	}	
	printf("Width: %d\n", UArray2b_width(array));
	printf("Height: %d\n", UArray2b_height(array));
	printf("Size: %d\n", UArray2b_size(array));
	printf("Blocksize: %d\n", UArray2b_blocksize(array));


	UArray2b_map(array, apply, NULL);

	UArray2b_free(&array);




	// //test UArray2
	// UArray2_T  testing = UArray2_new(4,4, sizeof(int));

	// int count = 0;
	// for (int i = 0; i < 4; i++) {
	// 	for (int j = 0; j < 4; j++) {
	// 		int *temp = UArray2_at(testing, i, j);
	// 		*temp = count++;
	// 	}
	// }

	// //print elements
	// for (int i = 0; i < 4; ++i){
	// 	for (int j = 0; j < 4; ++j){
	// 		printf("%d  ", *((int *)UArray2_at(testing, i, j)));	
	// 	}
	// 	printf("\n");
	// }

	return 0;
}

void apply(int col, int row, UArray2b_T array2b, void *elem, void *cl)
{
	(void) col;
	(void) row;
	(void) cl;
	(void) array2b;
	printf("%d\n", *((int*)elem));
}




