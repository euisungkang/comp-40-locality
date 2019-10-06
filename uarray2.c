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


/*initialized default values*/
int HEIGHT = -1, WIDTH = -1, SIZE = -1;


UArray2_T UArray2_new(int width, int height, int size){

        /*all inputs shall be greater than 0*/
        assert(height > 0 && width > 0 && size > 0);

        HEIGHT = height;
        WIDTH = width;
        SIZE = size;

        return (UArray2_T)(UArray_new(height * width, size));
}


/*frees the memory*/
void UArray2_free(UArray2_T *uarray){
        UArray_free(uarray);
}


/*private function to identify which cell in 1D array
that corresponds to the [row, col] in 2D array*/
int Convert_to_one_dim_index(int col,int row){

        /*indices must be non-negative and not out of bounds*/
        assert(row >= 0 && col >= 0);
        //printf("HEIGHT: %d, ROW: %d\n", HEIGHT, row);
        //printf("WIDTH: %d, COL: %d\n", WIDTH, col);
        assert(row < HEIGHT && col <  WIDTH);

        /*this formular correctly coverts a 2D indices to a correspondig
        1D index*/
        return (WIDTH * row) + col;
}

void *UArray2_at(UArray2_T uarray, int row, int col){
        //transform into a single index
        int index = Convert_to_one_dim_index(row, col);

        return UArray_at(uarray, index);
}




int UArray2_width(UArray2_T uarray){
        assert(WIDTH != -1);

        int width = UArray_length((UArray_T)uarray) / WIDTH;
        return width;
}



int UArray2_height(UArray2_T uarray){
        assert(HEIGHT != -1);
        
        int height = UArray_length((UArray_T)uarray) / HEIGHT;

        return height;
}


int UArray2_size(UArray2_T uarray){
        assert(SIZE != -1);
        
        int size =  UArray_size(uarray);


        return size;
}


void UArray2_map_col_major(UArray2_T uarray, void apply
    (int row, int col, UArray2_T uarray, void *cl2,void *cl), void *cl){
    
        int row, col, length;

        length = UArray_length((UArray_T)uarray);

        for (int i = 0; i < length; ++i)
        {
            /*Note: (WIDTH * row) + col to identity 1D array index,
            then row is quotient of the result, and col is remainder.
            So here we transform from 1D index to 2D indeces*/
            row = (i / WIDTH);
            col = (i % WIDTH);
            apply(row, col, uarray, UArray_at(uarray, 
                            Convert_to_one_dim_index(row, col)), cl);

        }

}


void UArray2_map_row_major(UArray2_T uarray, void apply
    (int row, int col, UArray2_T uarray, void *cl2,void *cl), void *cl){
    
        int row, col;

        for (int i = 0; i < WIDTH; ++i)
        {
            /*j increments by Width because elements of same column are Width 
            distance apart from each other*/
            for (int j = i; j < HEIGHT * WIDTH; j += WIDTH)
            {
                /*Note: (WIDTH * row) + col to identity 1D array index,
                then row is quotient of the result, and col is remainder.
                So here we transform from 1D index to 2D indeces*/
                row = (j / WIDTH);
                col = (j % WIDTH);
                apply(row, col, uarray, UArray_at(uarray, 
                            Convert_to_one_dim_index(row, col)), cl);
            }

        }

}