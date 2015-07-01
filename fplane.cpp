//matthew furlong cpsc 1020
//october 28 2014
//mfurlon
//contains functions for the finite plane in the raytracer program

#include "ray.h"
#include <math.h>

pparm_t fplane_parse[] =
{
	{"xdir", 3, sizeof(double), "%lf", 0},
	{"dimensions", 2, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(fplane_parse) / sizeof(pparm_t))

fplane_t::fplane_t(FILE *in, model_t *model, int attrmax) 
														: plane_t (in, model, 2)
{
	int mask;
	vec_t projxdir;

	strcpy(obj_type, "fplane");

	//parse the attributes
	fplane_parse[0].loc = &xdir;
	fplane_parse[1].loc = &dims;
	mask = parser(in, fplane_parse, NUM_ATTRS, attrmax);
	
	assert(mask == 3);

	//project xdir onto the plane creating projxdir
	vec_project(&normal, &xdir, &projxdir);

	//ensure that projxdir is not {0.0,0.0,0.0}
	//assert(projxdir.x != 0.0 && projxdir.y != 0.0 && projxdir.z != 0.0);
	//assert((projxdir.x && projxdir.y && projxdir.z) != 0.0);
	if((projxdir.x == projxdir.y) == projxdir.z)
		assert(projxdir.x != 0.0);
	
	//make projxdir unit length
	vec_unit(&projxdir, &projxdir);

	//copy projxdir to row 0 of rot
	vec_copy(&projxdir, &rot.row[0]);
	
	//copy plane normal to row 2 of rot and make it a unit length
	vec_copy(&normal, &rot.row[2]);
	vec_unit(&rot.row[2], &rot.row[2]);

	//set row of rot to the cross product of row 2 and row 1
	vec_cross(&rot.row[2], &rot.row[0], &rot.row[1]);
}

void fplane_t::printer(FILE *out)
{
	int i;
	plane_t::printer(out);

	fprintf(out,"dimensions %8.1lf %8.1lf \n", dims[0], dims[1]);
	vec_print(out,"xdir",&xdir);
	vec_print(out,"projected",&rot.row[0]);
	
	for(i=0;i<3;i++)
		vec_print(out,"row",&rot.row[i]);
}

double fplane_t::hits(
	vec_t *base, //ray base
	vec_t *dir) //unit direction vector
{
	vec_t newloc;
	double t;

	t = plane_t::hits(base,dir);

	if(t == -1)
		return t;
	
	//translation step by setting newloc to last_hit - point
	vec_diff(&point, &last_hitpt, &newloc);

	//rotate neloc by transforming it with the rot matrix
	vec_xform(&rot, &newloc, &newloc);

	//now perform checks
	if((0 <= newloc.x) && (newloc.x <= dims[0]) && 
					(0 <= newloc.y) && (newloc.y <= dims[1]))
	{
		vec_copy(&newloc, &rothit);
		return t;
	}
	else
		return -1;
}

void fplane_t::get_dims(double *dim)
{
	dim = dims;
}

void fplane_t::get_rothit(vec_t *vec)
{
	//vec = &rothit;
	vec_copy(&rothit, vec);
}
