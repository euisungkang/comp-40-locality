#include <stdio.h>
#include<stdlib.h>

#include "uarray2.h"

void print(int col, int row, UArray2_T array, void *cl2,void *cl){

	(void)	array;
	(void) cl;
	(void) col;
	printf("(%d, %d)  -> ", col, row);
	printf("%d  ",*((int *) cl2));
	// if (col == UArray2_width(array) - 1) 
	// 	printf("\n");
	if (row == UArray2_height(array) - 1)
		printf("\n");
}


int main()
{
	int width = 4;
	int height = 3;

	UArray2_T new_arry = UArray2_new (width, height, sizeof(int));
	int count = 0;

	for (int j = 0; j < UArray2_height(new_arry); ++j)
	{
		for (int i = 0; i < UArray2_width(new_arry); ++i)
		{
			int *temp = UArray2_at(new_arry, i, j);

			*temp = count ++;
		}
	}

	//printf("here\n");
	//UArray2_map_row_major(new_arry, print, NULL);

	printf("col major\n");
	UArray2_map_col_major(new_arry, print, NULL);
	UArray2_free(&new_arry);
	
	return 0;
}