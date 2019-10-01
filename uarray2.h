
/*
    Authors: Mico Theogene Micomyiza
             Yves Chen

    COMP40 FALL 2019
*/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED


#include "uarray.h"
#include <stdio.h>

typedef UArray_T UArray2_T;


/*
Description: creates an unboxed array of size height*width.

Parameters: integer value for height, width, and size of element that will be
stored in the
array in order to correctly allocate enough memory.

Returns: a UArray2_T to client
*/
UArray2_T UArray2_new( int height, int width, int size);


/* 
Description: applies the given function to a group of elements in the given
unboxed array

Parameters: UArray2_T, function pointer, and row index. A cl void pointer
as a check

Returns: nothing since it’s a void function
*/

void UArray2_map_row_major(UArray2_T uarray, void apply
    (int row, int col, UArray2_T array, void *cl2,void *cl), void *cl);



/*
Description: applies the given function to every element in the column
of the unboxed array
Parameters: UArray2_T, function pointer, and column index. A cl void
pointer as a check

Returns: nothing since it’s a void function
*/
 void UArray2_map_col_major(UArray2_T uarray, void apply
    (int row, int col, UArray2_T array, void *cl2,void *cl), void *cl);



/*
Description: free all memory that was allocated to uarray

Parameters: a UArray2_T pointer to the unboxed array to be freed
Return: none
*/
void UArray2_free(UArray2_T *uarray);


/*
Description: function takes a uarray and returns a void pointer to
 the element at that
particular element, specified by the indices.

Parameters: UArray2_T, row index and col index

Return: a void pointer to the element in [row, col]
*/
void *UArray2_at(UArray2_T uarray, int row, int col);


/*
Description: takes a uarray2 and returns the width of that array. 

Parameters: a UArray2_T

Returns: an int representing the width of the uarray2
*/
int UArray2_width(UArray2_T uarray);




/*
Description: takes a uarray2 and returns the height of that array. 

Parameters: a UArray2_T

Returns: an int representing the height of the uarray2
*/
int UArray2_height(UArray2_T uarray);


/*Description: takes a uarray2 and returns the size of elements stored in
 the unboxed array

Parameters: a UArray2_T

Returns: an int representing the size of the uarray2 element
*/
int UArray2_size(UArray2_T uarray);


#undef UArray2_T
#endif