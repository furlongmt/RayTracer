/**  vector.c   **/

#include "vector.h"

/*  This file contains function definitions for    */
/*  for 3D vector operations                       */

/* Compute the sum of two vectors */
/* v3 = v2 + v1; */
void vec_sum(
vec_t *v1, /* addend */
vec_t *v2, /* addend */
vec_t *v3) /* result */
{
	v3->x = v2->x + v1->x;
	v3->y = v2->y + v1->y;
	v3->z = v2->z + v1->z;
}

/* Compute the difference of two vectors */
/* v3 = v2 - v1 */
void vec_diff(
vec_t *v1, /* subtrahend */
vec_t *v2, /* minuend */
vec_t *v3) /* result */
{
  	v3->x = v2->x - v1->x;
	v3->y = v2->y - v1->y;
	v3->z = v2->z - v1->z;
}

/* Compute the componentwise product of two vectors */
/* v3 = v2 * v1; */
void vec_mult( 
vec_t *v1,   /* Input vector 1 */
vec_t *v2,   /* Input vector 2 */
vec_t *v3)   /* result         */
{
  	v3->x = v2->x * v1->x;
	v3->y = v2->y * v1->y;
	v3->z = v2->z * v1->z;
}

/* Return the (dot) inner product of two input vectors     */
double vec_dot(
vec_t *v1,   /* Input vector 1 */
vec_t *v2)   /* Input vector 2 */
{
	double dot;
	dot = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
	return dot;
}

/* Return length of a 3d vector */
double vec_len(
vec_t *v1)  /* Vector whose length is desired */
{
 	double length;
	length = sqrt(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z);
	return length;
}

/* Scale a 3d vector by multiplying each element by scale factor */
void vec_scale(
double fact,   /* Scale factor */
vec_t *v1,      /* Input vector */
vec_t *v2)      /* Output vector */
{
   v2->x = v1->x * fact;
	v2->y = v1->y * fact;
	v2->z = v1->z * fact;
}

/* Construct a unit vector v2 in direction of input vector v1 */
void vec_unit(
vec_t *v1,    /* Input vector      */
vec_t *v2)    /* output unit vec   */
{
	double length = 1/vec_len(v1);
	vec_scale(length,v1,v2);
}

/* Copy vector v1 to v2 another */
void vec_copy(
vec_t *v1,   /* input vector */
vec_t *v2)   /* output vector */
{
   v2->x = v1->x;
	v2->y = v1->y;
	v2->z = v1->z;
}

//Computer the outer product of two input vectors
void vec_cross(
vec_t *v1, //Left input vector
vec_t *v2, //Right input vector
vec_t *v3) //Output vector
{
	vec_t temp;

	temp.x = (v1->y * v2->z) - (v1->z * v2->y);
	temp.y = (v1->z * v2->x) - (v1->x * v2->z);
	temp.z = (v1->x * v2->y) - (v1->y * v2->x);

	vec_copy(&temp,v3);
}

//project a vector onto a plane
void vec_project(
vec_t *n, //plane normal
vec_t *v, //input vector
vec_t *p) //project vector
{
	double ndotv;
	vec_t temp;
	
	ndotv = vec_dot(n,v);
	vec_scale(ndotv,n,&temp);
	
	vec_diff(&temp,v,p);
}

//angle of reflection for vectors
void vec_reflect(
vec_t *n, //outward surface normal
vec_t *w,	  //unit vector in incoming direction of the ray
vec_t *v)  //unit vector in the outgoing direction of the ray
{
	vec_t temp1, u;
	double udotn;

	vec_scale(-1,w,&u);
	udotn = vec_dot(n,&u);
	udotn *= 2;
	vec_scale(udotn,n,&temp1);
	
	vec_diff(&u,&temp1,v);
}

//multiply a vector by a matrix
void vec_xform(
mtx_t *m,  //input matrix
vec_t *v1, //matrix to be transformed
vec_t *v2) //output vector
{
	vec_t v3;
	
	//v3.x = m->row[0].x * v1->x + m->row[0].y * v1->y + m->row[0].z * v1->z;
	//v3.y = m->row[1].x * v1->x + m->row[1].y * v1->y + m->row[1].z * v1->z;
	//v3.z = m->row[2].x * v1->x + m->row[2].y * v1->y + m->row[2].z * v1->z;
	
	v3.x = vec_dot(&m->row[0], v1);
	v3.y = vec_dot(&m->row[1], v1);
	v3.z = vec_dot(&m->row[2], v1);
	
	vec_copy(&v3,v2);
}

//transpose a matrix
void mtx_transpose(
mtx_t *m1, //input matrix
mtx_t *m2) //output transpose
{
	mtx_t m3; //avoid aliasing problems
	
	m3.row[0].x = m1->row[0].x;
	m3.row[0].y = m1->row[1].x;
	m3.row[0].z = m1->row[2].x;

	m3.row[1].x = m1->row[0].y;
	m3.row[1].y = m1->row[1].y;
	m3.row[1].z = m1->row[2].y;

	m3.row[2].x = m1->row[0].z;
	m3.row[2].y = m1->row[1].z;
	m3.row[2].z = m1->row[2].z;

	*m2 = m3;
}

/* Read in values of vector from file */
/* Return the # of items read	*/
int vec_read(
FILE *in,     /* input file  */
vec_t *v1)    /* vector from input file */
{
	int numb;
	numb = fscanf(in,"%lf %lf %lf", &v1->x, &v1->y, &v1->z);
	assert(numb == 3);
	return 0;
}

/* Print values of vector to file */
void vec_print(
FILE *out,     /* output file */
const char *label,   /* label string */
vec_t *v1)      /* vector to print */
{
	fprintf(out,"%s    %8.3lf    %8.3lf    %8.3lf\n",label,v1->x,v1->y,v1->z);
}

