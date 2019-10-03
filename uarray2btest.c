#include <stdio.h>
#include "uarray2b.h"

int main()
{
	UArray2b_T array = UArray2b_new(4, 4, sizeof(int), 2);

	
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int *temp = UArray2b_at(array, i, j);
			*temp = count++;


			printf("temp is %d ", *temp);
			printf("UArray2b_at is %d\n", *((int *)UArray2b_at(array, i, j)));
		}
	}	
	

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			printf("%d  ", *((int *)UArray2b_at(array, i, j)));	
		}
		printf("\n");
	}
	
	printf("Width: %d\n", UArray2b_width(array));
	printf("Height: %d\n", UArray2b_height(array));
	printf("Size: %d\n", UArray2b_size(array));
	printf("Blocksize: %d\n", UArray2b_blocksize(array));


	UArray2b_free(&array);
	return 0;
}