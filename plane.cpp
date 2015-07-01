//matthew furlong cpsc 1020
//october 26 2014
//mfurlon T-Th 12:30 - 1:45 class
//file with functions for planes in raytracer program

#include "ray.h"
#include <math.h>

pparm_t plane_parse[] =
{
	{"normal", 3, sizeof(double), "%lf", 0},
	{"point", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(plane_parse) / sizeof(pparm_t))

plane_t::plane_t(FILE *in, model_t *model, int attrmax) : object_t(in, model)
{
	int mask;
	
	strcpy(obj_type, "plane");
	
	/* The parser is fairly generic but the address of where */
	/* to put the data must be updated for each new object */
	plane_parse[0].loc = &normal;
	plane_parse[1].loc = &point;
	mask = parser(in, plane_parse, NUM_ATTRS, attrmax);
	
	assert(mask == 3);
	
	//functions from vector.c
	vec_unit(&normal, &normal);
	vec_copy(&normal, &last_normal);
	
	ndotq = vec_dot(&point, &normal);
}

//function to determine if a vector project from location *base
//in direction *dir hits the plane
//if so, it returns the distance from base to the point of contact
double plane_t::hits(
vec_t *base, /* ray base */
vec_t *dir) /* unit direction vector */
{
	double ndotd;
	double t;
	double ndotb;
	
	ndotq = vec_dot(&normal, &point);
	ndotd = vec_dot(dir, &normal);
	
	/* ndotd = 0 -> ray is parallel to the plane */
	if (ndotd == 0)
		return(-1);
		
	ndotb = vec_dot(&normal, base);
	
	t = (ndotq - ndotb) / ndotd;
		
	if (t <= 0)
		return(-1);
			
	vec_scale(t, dir, &last_hitpt);
	vec_sum(&last_hitpt, base, &last_hitpt);
	
	if ((last_hitpt.z > 0.01) && (strcmp(obj_type, "projector")))
		return(-1);
	
	//vec_copy(&normal, &last_normal);

	return(t);
}

//prints a formatted dump of the plane content
void plane_t::printer(FILE *out)
{
	//functions from vector.c
	vec_print(out,"normal",&normal);
	vec_print(out,"point",&point);
}
