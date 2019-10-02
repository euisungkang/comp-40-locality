#include <stdio.h>
#include <stdlib.h>
#include "uarray2b.h"

#define T UArray2b_T

const int DEFAULT_BLOCK_SIZE = 256;

T UArray2b_new (int width, int height, int size, int blocksize)
{
    int curr_aux_width = 0, curr_aux_height = 0;

    if (width % blocksize != 0) 
        curr_aux_width = (width / blocksize) + 1;
    else
        curr_aux_width = width / blocksize;

    if (height % blocksize != 0)
        curr_aux_height = (height / blocksize) + 1;
    else
        curr_aux_height = height / blocksize;


    //printf("aux_width: %d\n", curr_aux_width);
    //printf("aux_height: %d\n", curr_aux_height);
    T bArray = (T) malloc(sizeof(*bArray));
    bArray -> blockArray = UArray2_new(1, curr_aux_width * curr_aux_height, sizeof(UArray_T));
    bArray -> aux_width = curr_aux_width * curr_aux_height;
    bArray -> blocksize = blocksize;
    bArray -> width = width;
    bArray -> height = height;
    bArray -> size = size;

    for (int i = 0; i < bArray->aux_width; i++) {
        //UArray2_at(row, col)
        UArray_T *temp = UArray2_at(bArray -> blockArray, 0, i);
        *temp = (UArray_T) UArray_new(blocksize * blocksize, size);
    }

    return bArray;
}

T UArray2b_new_64K_block(int width, int height, int size)
{
    //blocksize is 256 = sqrt(65,536)
    return UArray2b_new(width, height, size, DEFAULT_BLOCK_SIZE);
}

void UArray2b_free (T *array2b)
{
    //Free every block
    for (int i = 0; i < (*array2b) -> aux_width; i++) {

        //UArray2_at(row, col)
        //if((*array2b) -> blockArray == NULL) printf("buff\n");
        UArray_free(UArray2_at((*array2b) -> blockArray, 0, i));
        //printf("aux_width is %d\n", (*array2b) -> aux_width);
    }

    //Free the array of blocks
    UArray2_free(&(*array2b) -> blockArray);

    //Free the array of arrays
    free(*array2b);
}

int UArray2b_width (T array2b)
{
    return array2b -> width;
}

int UArray2b_height (T array2b)
{
    return array2b-> height;
}

int UArray2b_size (T array2b)
{
    return array2b-> size;
}

int UArray2b_blocksize(T array2b)
{
    return array2b->blocksize;
}

void *UArray2b_at(T array2b, int column, int row)
{
    int blockCol = column / array2b -> blocksize;
    int blockRow = row / array2b -> blocksize;

    //Returns the block
    UArray_T temp = UArray2_at(array2b -> blockArray, 0, blockRow);

    //Return the actual element in the block
    return UArray_at(temp, (array2b -> blocksize * blockCol) + blockRow);
}

/*
void UArray2b_map(T array2b, void apply(int col, int row, T array2b, void *elem, void *cl), void *cl)
{

}
*/
#undef T