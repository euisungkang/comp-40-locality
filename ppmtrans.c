#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
        printf("fun\n");                                        \
} while (0);

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

/* Function Prototypes */
Pnm_ppm initialize_array(A2Methods_T *, FILE *);
//F *read_file(int, char **);
void rotate_0(Pnm_ppm);
void rotate_90(Pnm_ppm);
void apply_rotation_90(int, int, A2Methods_UArray2, void *, void *);
void print_array(int, int, A2Methods_UArray2, void *, void *);

int main(int argc, char *argv[]) 
{
        if (argc == 1) {
            fprintf(stderr, "Usage: ./ppmtrans [-rotate 90] [-time] [filename]");
            exit(EXIT_FAILURE);
        } 

        FILE *stream = NULL;

        int   rotation       = 0;
        int   i;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        Pnm_ppm ppm_image;
        //initialize_array(methods, stdin);

        for (i = 1; i < argc; i++) {

                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
				    "row-major");

                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
				    "column-major");

                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");

                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
					"Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }

                        if (rotation == 0) {
                            rotate_0(ppm_image);
                        } else if (rotation == 90) {
                            rotate_90(ppm_image);
                        }


                // } else if (strcmp(argv[i], "-time") == 0) {
                //         char *time_file_name = NULL;
                //         time_file_name = argv[++i];
                }else if(*argv[i] != '-'){
                    stream = fopen(argv[i], "r");
                    //initialize the array
                    ppm_image = initialize_array(&methods, stream);

                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
				argv[i]);

                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);

                } else {
                        break;
                }
        }

        rotate_0(ppm_image);
        printf("\n");
        rotate_90(ppm_image);
        //assert(0);              // the rest of this function is not yet implemented
}

Pnm_ppm initialize_array(A2Methods_T *methods, FILE *img)
{
    return Pnm_ppmread(img, *methods);
}


void rotate_0(Pnm_ppm image)
{
    image -> methods -> map_col_major(image -> pixels, print_array, image);

    return;
}

void rotate_90(Pnm_ppm image)
{
    A2Methods_UArray2 old_array;

    old_array = image -> pixels;

    int width = image->width;
    int height = image->height;

    //Initialize new array for rotated image

    printf("width is %d\n", width);
    printf("height is %d\n", height);

    A2Methods_UArray2 new_array = image -> methods -> new(height, width, sizeof(int));

    image -> pixels = new_array;

    image -> width = height;
    image -> height = width;

    printf("new width is %d, new height is %d\n", image -> width, image -> height);

    image -> methods -> map_col_major(old_array, apply_rotation_90, image);

    image -> methods -> map_col_major(image -> pixels, print_array, image);

   //A2Methods_T aux_array = methods->new(width, height, size);
}

void apply_rotation_90(int i, int j, A2Methods_UArray2 array2b, void *value, void *cl) 
{
    (void) array2b; /* we don't use this array here  because
    we only need the current value and it passed in as a void pointer*/

    
    Pnm_ppm ppm = (Pnm_ppm) cl;

    int h = ppm -> width;
    

    //finding new location for the current value in the
    //rotated image
    int new_i = h - j - 1;
    int new_j = i;


    int *curr_value = (int *) value;

    //finding location in the rotated array
    printf("(%d, %d) -> ", i, j);
    printf("(%d, %d)\n", new_i, new_j);
    int *new_location = ppm -> methods -> at(ppm -> pixels, new_i, new_j);

    //*new_location  = *curr_value;

    //printf("Value: %d -> %d\n", *new_location, *curr_value);


    //setting the value
    *new_location  = *curr_value;
}

void print_array(int i, int j, A2Methods_UArray2 array2b, void *value, void *cl)
{
    (void) i;
    (void) j;
    (void) array2b;
    (void)value;
    printf("%d ", (*((int *) value)));
    printf("i is %d, Width - 1: %d\n",i, (((Pnm_ppm) cl) -> width) - 1);
    if ((unsigned) i == (((Pnm_ppm) cl) -> width) - 1){
        printf("\n");
    }
}


#undef SET_METHODS