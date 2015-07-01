//matthew furlong
//october 26 2014 cpsc 1020
//functions for sphere objects in raytracer program

#include "ray.h"

pparm_t sphere_parse [] =
{
	{"center", 3, sizeof(double), "%lf", 0},
	{"radius", 1, sizeof(double), "%lf", 0},
	{"scale", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(sphere_parse) / sizeof(pparm_t))

//constructor for sphere object
sphere_t::sphere_t(FILE *in, model_t *model, int attrmax) : object_t(in,model)
{
	int mask;

	strcpy(obj_type, "sphere");

	//use parser function load attributes into sphere object
	sphere_parse[0].loc = &center;
	sphere_parse[1].loc = &radius;
	sphere_parse[2].loc = &scale;
	mask = parser(in, sphere_parse, NUM_ATTRS, attrmax);

	assert(mask > 0);
}

double sphere_t::hits(vec_t *base, vec_t *dir)
{
	assert(cookie = OBJ_COOKIE);

	//new local variables for function calculations
	double a, b, c, discr, quadratic;
	vec_t newBase, hitpoint, temp1, temp2;

	//functions from vector.c
	vec_diff(&center, base, &newBase);
	//a = DIRdotDIR
	a = vec_dot(dir, dir);
	//b = 2 * DIR dot NEWBASE
	b = 2 * vec_dot(&newBase,dir);
	//c = NEWBASEdotNEWBASE - radius^2
	c = vec_dot(&newBase, &newBase) - (radius * radius);
	//discriminant from quadratic equation
	discr = (b *b) - (4 * a * c);

	//check to make sure the vector hit
	if(discr < 0)
	{
		return -1;
	}

	//quadratic equation
	quadratic = (-b - sqrt(discr)) / (2 * a);

	//vector calculations for object last hit
	vec_scale(quadratic, dir, &hitpoint);
	vec_sum(base, &hitpoint, &temp1);

	//vec_copy(&last_hitpt, &temp1);
	vec_copy(&temp1, &last_hitpt);

	//vector calculations for object last normal
	//temp1.x -= radius;
	//temp1.y -= radius;
	//temp1.z -= radius;

	vec_diff(&center, &temp1, &temp1);

	//unit function and copy to objects last normal
	vec_unit(&temp1, &temp2);
	//vec_copy(&last_normal, &temp1);
	vec_copy(&temp2, &last_normal);
	
	return quadratic;
}

//function to dump contents of sphere object
void sphere_t::printer(FILE *out)
{
	vec_print(out, "center", &center);
	fprintf(out, "radius %8.1lf\n\n", radius);
}
