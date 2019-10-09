/*
    Authors: Mico Theogene Micomyiza
             Easton Kang


    Date: 10/9/2019

    COMP40 FALL 2019
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                                                              \
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
double rotate_0(Pnm_ppm, CPUTime_T);
double rotate_90(Pnm_ppm, CPUTime_T);
double rotate_180(Pnm_ppm, CPUTime_T);
double rotate_270(Pnm_ppm, CPUTime_T);
void apply_rotation_180(int, int, A2Methods_UArray2, void *, void *);
void apply_rotation_90(int, int, A2Methods_UArray2, void *, void *);
double flip_horizontal(Pnm_ppm, CPUTime_T);
double flip_vertical(Pnm_ppm, CPUTime_T);
double transpose(Pnm_ppm, CPUTime_T);
void apply_horizontal_flip(int, int, A2Methods_UArray2, void *, void *);
void apply_vertical_flip(int, int, A2Methods_UArray2, void *, void *);
void print_array_helper(Pnm_ppm);
void print_array(int, int, A2Methods_UArray2, void *, void *);

int main(int argc, char *argv[]) 
{
        FILE *stream = NULL;

        int   rotation       = 0;
        char *flip           = "";
        int   i;

        /* Horizontal = 1, Vertical = 2, Transpose = 3, rotate_0 = 0
           rotate_90 = 90, rotate_180 = 180, rotate_270 = 270  */
        int which_command;
        bool measure_time;  /* Should we measure time? TRUE = measure */
        (void) measure_time;
        bool file_name_given = false; /* Determines if we read from
                                         stdin or file */


        CPUTime_T time = CPUTime_New();
        char *time_file_name = NULL;
        (void) time_file_name; /* Only set in an if condition */

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        Pnm_ppm ppm_image;

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
                            which_command = 0;
                        } else if (rotation == 90) {
                            which_command = 90;
                        } else if (rotation == 180) {
                            which_command = 180;
                        } else {
                            which_command = 270;
                        }

                } else if (strcmp(argv[i], "-flip") == 0) {
                    printf("Compare: %d\n", strcmp(argv[i], "-flip"));
                    fprintf(stderr, "%s\n", argv[i]);
                    if (!(i + 1 < argc)) {
                        usage(argv[0]);
                    }
                    flip = argv[++i];
                    if (strcmp(flip, "horizontal") == 0 || 
                                        strcmp(flip, "vertical") == 0) {
                        fprintf(stderr,
                            "Flip must be horizontal or vertical\n");
                        usage(argv[0]);
                    }

                    if (strcmp(flip, "horizontal") == 0) {
                        which_command = 1;
                    } else {
                        which_command = 2;
                    }

                } else if(strcmp(argv[i], "-transpose") == 0) {
                    which_command = 3;
                } else if (strcmp(argv[i], "-time") == 0) {
                        measure_time = true;
                        time_file_name = argv[++i];
                } else if(*argv[i] != '-') {
                    file_name_given = true;
                    stream = fopen(argv[i], "r");
                    assert(stream != NULL);

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

        if (!file_name_given) {
            stream = stdin;
            ppm_image = initialize_array(&methods, stream);
        }

        double cpuTime = 0.0;
        (void) cpuTime; /* Only set in cases */

        switch(which_command) {
            case 0: cpuTime = rotate_0(ppm_image, time);
                break;
            case 90: cpuTime = rotate_90(ppm_image, time);
                break;
            case 180: cpuTime = rotate_180(ppm_image, time);
                break;
            case 270: cpuTime = rotate_270(ppm_image, time);
                break;
            case 1: cpuTime = flip_horizontal(ppm_image, time);
                break;
            case 2: cpuTime = flip_vertical(ppm_image, time);
                break;
            case 3: cpuTime = transpose(ppm_image, time);
                break;
            default:
                break;
        }


        print_array_helper(ppm_image);
        //free memory
        Pnm_ppmfree(&ppm_image);
        CPUTime_Free(&time);

        fclose(stream);
        exit(EXIT_SUCCESS);
}

Pnm_ppm initialize_array(A2Methods_T *methods, FILE *img)
{
    return Pnm_ppmread(img, *methods);
}

double rotate_0(Pnm_ppm image, CPUTime_T time)
{
    CPUTime_Start(time);
    /*rotate_0 doesn't do anything to the original image*/
    (void) image;
    return CPUTime_Stop(time);
}

double rotate_90(Pnm_ppm image, CPUTime_T time)
{
    /*creating a temp image in order to hold the original image and rotated one will
    be stored in the original image ppm instance*/
    A2Methods_UArray2 original_image;
    original_image = image -> pixels;


    int width = image->width;
    int height = image->height;

    /*in case the image was a rectangle, the rotated image have dimensions
        that are reversed compared to original image*/
    image -> width = height;
    image -> height = width;

    /*Initialize new array for rotated image
    Note: height and width are switched */
    image -> pixels = image -> methods -> new(height, width, sizeof(int));

    CPUTime_Start(time);
    /*calling apply function in order to rotate every pixed in the image*/
    image -> methods -> map_row_major(original_image, apply_rotation_90, image);

    double cpuTime = CPUTime_Stop(time);

    /*free memory taken by temp array*/
    image -> methods -> free(&original_image);

    return cpuTime;
}

double rotate_180(Pnm_ppm image, CPUTime_T time)
{
    /*creating a temp image in order to hold the original image and rotated one will
    be stored in the original image ppm instance*/
    A2Methods_UArray2 original_image;
    original_image = image -> pixels;


    int width = image->width;
    int height = image->height;

    /*Initialize new array for rotated image*/
    image -> pixels = image -> methods -> new(width, height, sizeof(int));

    CPUTime_Start(time);
    /*calling apply function in order to rotate every pixed in the image*/
    image -> methods -> map_row_major(original_image, apply_rotation_180, image);

    double cpuTime = CPUTime_Stop(time);

    /*free memory taken by temp array*/
    image -> methods -> free(&original_image);

    return cpuTime;
}

double rotate_270(Pnm_ppm image, CPUTime_T time)
{
    /*270 rotation = 180 + 90*/
    return (rotate_90(image, time) + rotate_180(image, time));
}

double flip_horizontal(Pnm_ppm image, CPUTime_T time)
{
    /*creating a temp image in order to hold the original image and rotated one will
    be stored in the original image ppm instance*/
    A2Methods_UArray2 original_image;

    original_image = image -> pixels;

    int width = image->width;
    int height = image->height;

    /*Initialize new array for rotated image*/
    image -> pixels = image -> methods -> new(width, height, sizeof(int));

    CPUTime_Start(time);
    /*calling apply function in order to rotate every pixed in the image*/
    image -> methods -> map_row_major(original_image, apply_horizontal_flip, image);

    double cpuTime = CPUTime_Stop(time);

    /*free memory taken by temp array*/
    image -> methods -> free(&original_image);

    return cpuTime;
}

double flip_vertical(Pnm_ppm image, CPUTime_T time)
{
    /*creating a temp image in order to hold the original image and rotated one will
    be stored in the original image ppm instance*/
    A2Methods_UArray2 original_image;
    original_image = image -> pixels;

    int width = image->width;
    int height = image->height;

    /*Initialize new array for rotated image*/
    image -> pixels = image -> methods -> new(width, height, sizeof(int));

    CPUTime_Start(time);

    /*calling apply function in order to rotate every pixed in the image*/
    image -> methods -> map_row_major(original_image, apply_vertical_flip, image);

    double cpuTime = CPUTime_Stop(time);

    /*free memory taken by temp array*/
    image -> methods -> free(&original_image);

    return cpuTime;
}


double transpose(Pnm_ppm image, CPUTime_T time)
{
    return (flip_vertical(image, time) +
            flip_horizontal(image, time));
}


void print_array_helper(Pnm_ppm image)
{
    //image -> methods -> map_row_major(image -> pixels, print_array, image);
    //printf("\n\n");


    /*write output on ppm file*/
    //FILE *output = fopen("Rotated90.ppm", "w");
    Pnm_ppmwrite(stdout, image);

    //fclose(output);
}

void apply_rotation_90(int i, int j, A2Methods_UArray2 array2b, void *value, void *cl) 
{
    (void) array2b; /* we don't use this array here  because
    we only need the current value and it passed in as a void pointer*/
    
    Pnm_ppm ppm = (Pnm_ppm) cl;

    /*Note: height and width of original image were swapped in the rotated image
        Thus, h of original image is same as width of rotated image*/
    int h = ppm -> width;
    

    /*finding new location for the current value in the rotated image*/
    int new_i = h - j - 1;
    int new_j = i;


    /*finding position for current element in rotated image*/
    int *new_location = ppm -> methods -> at(ppm -> pixels, new_i, new_j);

    /*setting the value from original image to its rocaiton in rotated image*/
    *new_location  = *((int *) value);
}


void apply_rotation_180(int i, int j, A2Methods_UArray2 array2b, void *value, void *cl)
{
    (void) array2b; /* we don't use this array here  because
    we only need the current value and it passed in as a void pointer*/

    Pnm_ppm ppm = (Pnm_ppm) cl;


    int h = ppm -> height;
    int w = ppm -> width;
    
    /*finding new location for the current value in the rotated image*/
    int new_i = w - i - 1;
    int new_j = h - j - 1;


    /*finding position for current element in rotated image*/
    int *new_location = ppm -> methods -> at(ppm -> pixels, new_i, new_j);

    /*setting the value from original image to its rocaiton in rotated image*/
    *new_location  = *((int *) value);
}

void apply_horizontal_flip(int i, int j, A2Methods_UArray2 array2b, void *value, void *cl)
{
    (void) array2b; /* we don't use this array here  because
    we only need the current value and it passed in as a void pointer*/

    Pnm_ppm ppm = (Pnm_ppm) cl;

    //which is same as height of original image
    int h = ppm -> height;
    
    /*finding new location for the current value in the flipped image*/
    int new_i = h - i - 1;
    int new_j = j;


    /*finding position for current element in flipped image*/
    int *new_location = ppm -> methods -> at(ppm -> pixels, new_i, new_j);

    /*setting the value from original image to its rocaiton in flipped image*/
    *new_location  = *((int *) value);
}

void apply_vertical_flip(int i, int j, A2Methods_UArray2 array2b, void *value, void *cl)
{
    (void) array2b; /* we don't use this array here  because
    we only need the current value and it passed in as a void pointer*/

    Pnm_ppm ppm = (Pnm_ppm) cl;

    /*which is same as width of original image*/
    int w = ppm -> width;
    
    /*finding new location for the current value in the flipped image*/
    int new_i = i;
    int new_j = w - j - 1;


    /*finding position for current element in flipped image*/
    int *new_location = ppm -> methods -> at(ppm -> pixels, new_i, new_j);

    /*setting the value from original image to its rocaiton in flipped image*/
    *new_location  = *((int *) value);
}

void print_array(int i, int j, A2Methods_UArray2 array2b, void *value, void *cl)
{
    (void) i;
    (void) j;
    (void) array2b;
    (void) value;
    printf("%d ", (*((int *) value)));
    if ((unsigned) i == (((Pnm_ppm) cl) -> width) - 1){
        printf("\n");
    }
}

#undef SET_METHODS