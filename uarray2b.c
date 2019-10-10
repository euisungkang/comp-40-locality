#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "uarray2b.h"

#define T UArray2b_T

const int DEFAULT_BLOCK_SIZE = 256;

T UArray2b_new (int width, int height, int size, int blocksize)
{
    int curr_aux_width = 0, curr_aux_height = 0;

    /* If width is not divisible by blocksize, there has to be +1 block */
    if (width % blocksize != 0) 
        curr_aux_width = (width / blocksize) + 1;
    else
        curr_aux_width = width / blocksize;

    /* If height is not divisible by blocksize, there has to be +1 block */
    if (height % blocksize != 0)
        curr_aux_height = (height / blocksize) + 1;
    else
        curr_aux_height = height / blocksize;

    T bArray = (T) malloc(sizeof(*bArray));
    bArray -> blockArray = UArray2_new(1, curr_aux_width *
                         curr_aux_height, sizeof(UArray_T));

    bArray -> aux_width = curr_aux_width * curr_aux_height;
    bArray -> blocksize = blocksize;
    bArray -> width = width;
    bArray -> height = height;
    bArray -> size = size;

    for (int i = 0; i < bArray->aux_width; i++) {
        UArray_T *temp  = UArray2_at(bArray -> blockArray, 0, i);
        *temp  = (UArray_T) UArray_new(blocksize * blocksize, size);
    }

    return bArray;
}

T UArray2b_new_64K_block(int width, int height, int size)
{
    /* blocksize is 256 = sqrt(65,536) */
    return UArray2b_new(width, height, size, DEFAULT_BLOCK_SIZE);
}

void UArray2b_free (T *array2b)
{
    /* Free every block */
    for (int i = 0; i < (*array2b) -> aux_width; i++) {
        UArray_free(UArray2_at((*array2b) -> blockArray, 0, i));
    }

    /* Free the array of blocks */
    UArray2_free(&(*array2b) -> blockArray);
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
    int blockRow = row / array2b -> blocksize;
    int blockCol = column / array2b -> blocksize;

    /* Returns the block */

    UArray_T *temp = UArray2_at(array2b -> blockArray, 0,
             (((array2b -> aux_width) * blockRow) + blockCol)) ;

    /* Return the actual element in the block */
    int aux = (array2b -> width * (row % array2b -> blocksize)) +
                                         (column % array2b -> blocksize);

    return UArray_at(*temp, aux);
}

void UArray2b_map(T array2b, void apply(int col, int row, T array2b, 
                                    void *elem, void *cl), void *cl)
{
    UArray2_T block = array2b -> blockArray;

    if (array2b -> width % array2b -> blocksize != 0) {
        width_exceed = true;
    }
    if (array2b -> height % array2b -> blocksize != 0) {
        height_exceed = true;
    }

    for (int i = 0; i < array2b -> aux_width; i++) {
        UArray_T *curr_block = UArray2_at(block, 0, i);
        for (int x = 0; x < array2b -> width; x++) {
            for (int y = 0; y < array2b -> height; y++) {
                void *aux = UArray_at(*curr_block, array2b -> width * x + y);
                apply(x, y, array2b, aux, cl);
            }
        }
    }
}


#undef T