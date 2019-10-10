/*
    Authors: Mico Theogene Micomyiza
             Yves Chen

    COMP40 FALL 2019
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "assert.h"

#include "uarray2.h"


/************************************************/
/* implementation of uarray2                    */
/************************************************/

UArray2_T UArray2_new(int width, int height, int size)
{

        /*all inputs shall be greater than 0*/
        assert(height > 0 && width > 0 && size > 0);

        UArray2_T array2 = calloc(height * width, sizeof(UArray2_T));
        assert (array2 != NULL);

        /*initializing the struct*/
        array2 -> width = width;
        array2 -> height = height;
        //printf("Actual Size: %d\n", size);
        array2 -> size = size;
        //printf("Debug Size: %d\n", array2 -> size);
        array2 -> array = (UArray_T) UArray_new(height * width, size);

        return array2;
}


/*frees the memory*/
void UArray2_free(UArray2_T *uarray)
{
        UArray_free(&((*uarray) -> array));
        free(*uarray);
}


/*private function to identify which cell in 1D array
that corresponds to the [row, col] in 2D array*/
int Convert_to_one_dim_index(UArray2_T uarray, int col,int row)
{

        /*indices must be non-negative and not out of bounds*/
        assert(row >= 0 && col >= 0);


        assert(col <  uarray -> width && row < uarray -> height);

        /*this formular correctly coverts a 2D indices to a correspondig
        1D index*/
        return (uarray -> width * row) + col;
}

void *UArray2_at(UArray2_T uarray, int col, int row)
{
        /*transform into a single index*/
        int index = Convert_to_one_dim_index(uarray, col, row);

        return UArray_at(uarray -> array, index);
}




int UArray2_width(UArray2_T uarray)
{
        return uarray -> width;
}



int UArray2_height(UArray2_T uarray)
{
        return uarray -> height;
}


int UArray2_size(UArray2_T uarray)
{
        return uarray -> size;
}


void UArray2_map_row_major(UArray2_T uarray, void apply
    (int col, int row, UArray2_T uarray, void *cl2,void *cl), void *cl)
{
    
        int row, col;
        for (int i = 0; i < (uarray -> height) * (uarray -> width); ++i){
            /*Note: (WIDTH * row) + col to identity 1D array index,
            then row is quotient of the result, and col is remainder.
            So here we transform from 1D index to 2D indeces*/
            row = (i / uarray -> width);
            col = (i % uarray -> width);
            apply(col, row, uarray, UArray_at(uarray -> array, 
                            Convert_to_one_dim_index(uarray, col, row)), cl);

        }

}


void UArray2_map_col_major(UArray2_T uarray, void apply
    (int col, int row, UArray2_T uarray, void *cl2,void *cl), void *cl)
{
    
        int row, col;
        for (int i = 0; i < uarray -> width; ++i){
            /*j increments by Width because elements of same column are Width 
            distance apart from each other*/
            for (int j = i; j < (uarray -> height) * (uarray -> width);
                                                 j += uarray -> width){
                /*Note: (WIDTH * row) + col to identity 1D array index,
                then row is quotient of the result, and col is remainder.
                So here we transform from 1D index to 2D indeces*/
                row = (j / uarray -> width);
                col = (j % uarray -> width);
                apply(col, row, uarray, UArray_at(uarray -> array, 
                            Convert_to_one_dim_index(uarray, col, row)), cl);
            }

        }

}