#include <string.h>

#include <a2plain.h>
#include "uarray2.h"

/************************************************/
/* Define a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/

static A2Methods_UArray2 new(int width, int height, int size)
{
  //TODO: Implement this function and remove the dummy return statement.
  return NULL;
}

static A2Methods_UArray2 new_with_blocksize(int width, int height, int size, int blocksize)
{
  //TODO: Implement this function and remove the dummy return statement.
  (void) blocksize;
  return NULL;
}


/* TODO: ...many more private (static) definitions follow */

static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
  UArray2_map_row_major(uarray2, (UArray2_applyfun*)apply, cl);
}

static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
  UArray2_map_col_major(uarray2, (UArray2_applyfun*)apply, cl);
}

struct small_closure {
  A2Methods_smallapplyfun *apply; 
  void                    *cl;
};

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
  struct small_closure *cl = vcl;
  (void)i;
  (void)j;
  (void)uarray2;
  cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_row_major(a2, apply_small, &mycl);
}

static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_col_major(a2, apply_small, &mycl);
}


static struct A2Methods_T uarray2_methods_blocked_struct = {
  new,
  new_with_blocksize,
  /* ... other functions follow in order,
   *     with NULL for those not implemented ...
   */
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_blocked = &uarray2_methods_blocked_struct;
