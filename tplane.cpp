//matthew furlong cpsc 1020
//october 28 2014
//mfurlon 
//file with functions for tiled planes in raytracer program

#include "ray.h"

pparm_t tplane_parse[] =
{
	{"xdir", 3, sizeof(double), "%lf", 0},
	{"dimensions", 2, sizeof(double), "%lf", 0},
	//{"altmaterial", 1, NAME_LEN, "%s", 0},
};

#define NUM_ATTRS (sizeof(tplane_parse) / sizeof(pparm_t))

tplane_t::tplane_t(FILE *in, model_t *model, int attrmax) : plane_t(in,model,2)
{
	int mask;
	char name[32];

	vec_t projxdir;

	//parse tiled plane data
	tplane_parse[0].loc = &xdir;
	tplane_parse[1].loc = &dims;
	//tplane_parse[2].loc = &altname;
	mask = parser(in, tplane_parse, NUM_ATTRS, 2);
	
	assert(mask == 3);

	fscanf(in,"%s",name);

	fscanf(in, "%s",altname);

	//ask material_getbyname to return a pointer to the alt background mat
	altmat = material_getbyname(model,altname);
	
	fscanf(in, "%s", name);
	assert(name[0] == '}');

	//project xdir into the plane and make it a unit vector
	vec_project(&normal, &xdir, &projxdir);
	vec_unit(&projxdir, &projxdir);

	//build a rotation matrix that rotates the plane normal
	//into the z axis and the projected xdir into the x axis
	vec_copy(&normal, &rot.row[2]);
	vec_copy(&projxdir, &rot.row[0]);
	vec_cross(&rot.row[0], &rot.row[2], &rot.row[1]);

	//copy tiled plane to objtype
	strcpy(obj_type, "tiled plane");
}

//dump contents of tiled plane
void tplane_t::printer(FILE *out)
{
	plane_t::printer(out);

	fprintf(out, "dimensions %8.1lf %8.1lf \n", dims[0], dims[1]);
	vec_print(out, "xdir", &xdir);
	//vec_print(out, "projected", &rot.row[0]);
	fprintf(out, "altmaterial %s\n", altname);
}

//get diffuse function for tplane
void tplane_t::getdiffuse(drgb_t *value)
{
	if(select() == 1)
		mat->material_getdiffuse(value);
	else
		altmat->material_getdiffuse(value);
}

//get ambient function for tplane
void tplane_t::getambient(drgb_t *amb)
{
	if(select() == 1)
		mat->material_getambient(amb);
	else
		altmat->material_getambient(amb);
}

int tplane_t::select(void)
{
	int x, y;
	vec_t newloc;

	//apply the rot matrix to last_hit to rotate it into the plane
	//having normal 0, 0, 1 with the project xdir fo tiling parallel
	//to the x axis

	vec_xform(&rot, &last_hitpt, &newloc);

	//add 100000 to newloc.x and newloc.y
	//x = (last_hitpt.x + 100000) / dims[0];
	//y = (last_hitpt.y + 100000) / dims[1];
	newloc.x += 100000;
	newloc.y += 100000;

	//divide newloc.x by dims[0] and newloc.y by dims[1] to compute 
	//relative tile number in each direction 
	newloc.x /= dims[0];
	newloc.y /= dims[1];
	x = newloc.x;
	y = newloc.y;

	if( (x+y) % 2 == 0)
		return 1;
	else
		return 0;
}
