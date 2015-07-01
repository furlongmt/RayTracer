/* pixel.h */

/* Two pixel value structures are used.                   */
/* Computations are done in the double precision domain   */
/* where 0.0 means black and 1.0 means maximum intensity. */

/* The raytracing algorithm can DEFINITELY produce        */
/* pixel values that exceed 1.0  - scale and then clamp   */
/* them before assigning them to irgb_types.              */

#include <stdio.h>

#define R 0
#define G 1
#define B 2

typedef struct drgb_type
{
   double r;
   double g;
   double b;
} drgb_t;


/* The .ppm file requires the usual r-g-b values in the  */
/* range 0-255.  This structure maps one of them.        */
typedef struct irgb_type
{
   unsigned char r;
   unsigned char g;
   unsigned char b;
} irgb_t;


/* Compute p2 = s * p1 where s is a scalar. */
void pix_scale(double s, drgb_t *p1, drgb_t *p2);


/* Compute componentwise product of two rgb values. */
void pix_prod(drgb_t *p1, drgb_t *p2, drgb_t *p3);


/* Compute componentwise sum of two rgb values. */
void pix_sum(drgb_t *p1, drgb_t *p2, drgb_t *p3);


/* Compute componentwise sum of two rgb values. */
void pix_copy(drgb_t *p1, drgb_t *p2);

/* Print the rgb values to a file. */
/* 	out is the output file       */
/* 	label is the label string    */
/* 	pix is the pixel to print    */
void pix_print(FILE *out, char *label, drgb_t *pix);   
