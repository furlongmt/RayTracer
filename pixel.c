/* pixel.c */
/* pixel functions:
		pix_scale()
		pix_prod()
		pix_sum()
		pix_copy()
		pix_print()
*/

/* Two pixel value structures are used.                   */
/* Computations are done in the double precision domain   */
/* where 0.0 means black and 1.0 means maximum intensity. */

/* The raytracing algorithm can DEFINITELY produce        */
/* pixel values that exceed 1.0 - scale and then clamp    */
/* them before assigning them to irgb_types.              */

#include "pixel.h"

//=============================================================================
// ******* pix_scale( ) *******
// 
// compute p2 = s * p1 where s is a scalar
//=============================================================================
void pix_scale(double s, drgb_t  *p1, drgb_t  *p2)
{
   p2->r = s * p1->r;
   p2->g = s * p1->g;
   p2->b = s * p1->b;
}

//=============================================================================
// ******* pix_prod( ) *******
// 
// compute componentwise product of two rgb values 
//=============================================================================
void pix_prod(drgb_t *p1, drgb_t *p2, drgb_t *p3)
{
   p3->r = p1->r * p2->r;
   p3->g = p1->g * p2->g;
   p3->b = p1->b * p2->b;
}

//=============================================================================
// ******* pix_sum( ) *******
// 
// compute componentwise sum of two rgb values 
//=============================================================================
void pix_sum(drgb_t *p1, drgb_t *p2, drgb_t *p3)
{
   p3->r = p1->r + p2->r;
   p3->g = p1->g + p2->g;
   p3->b = p1->b + p2->b;
}

//=============================================================================
// ******* pix_copy( ) *******
// 
// compute componentwise one pixel to another
//=============================================================================
void pix_copy(drgb_t *p1, drgb_t *p2)
{
   p2->r = p1->r;
   p2->g = p1->g;
   p2->b = p1->b;
}


//=============================================================================
// ******* pix_print( ) *******
// 
// print the rgb values to a file
// 	out is the out file
//		label is the label string
// 	pix is the vector to print
//=============================================================================
void pix_print(FILE *out, char *label, drgb_t *pix)
{
	fprintf(out, "%s %8.3lf %8.3lf %8.3lf\n",
   		       label, pix->r, pix->g, pix->b);
}


//return maximum value from rgb 
double pix_max(drgb_t *pix)
{
	if((pix->r > pix->g) && (pix->r > pix->b))
		return pix->r;
	else if(pix->g > pix-> b)
		return pix->g;
	else
		return pix->b;
}
