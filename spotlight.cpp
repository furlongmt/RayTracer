//matthew furlong 
//october 29 2014
//cpsc 1020 mfurlon
//functions for spotlight in raytracer program

#include "ray.h"

pparm_t spotlight_parse [] = 
{
	{"point", 3, sizeof(double), "%lf", 0},
	{"theta", 1, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(spotlight_parse) / sizeof(pparm_t))

spotlight_t::spotlight_t(FILE *in, model_t *model, int attrmax) : light_t(in, model, 2)
{
	int mask;

	//parse attributes for spotlight
	spotlight_parse[0].loc = &point;
	spotlight_parse[1].loc = &halftheta;
	mask = parser(in, spotlight_parse, NUM_ATTRS, attrmax);

	assert(mask == 3);

	//compute a unit vector in the centerline direction and store into direction
	vec_diff(&location, &point, &direction);
	vec_unit(&direction, &direction);
	
	//convert theta to radians and store its cosine in costheta
	halftheta *= M_PI / 180;
	costheta = cos(halftheta);
	//costheta = (costheta * (M_PI / 180));
}

void spotlight_t::printer(FILE * out)
{
	light_t::printer(out);
	vec_print(out, "point", &point);
	fprintf(out, "theta %lf \n", halftheta);
}

int spotlight_t::vischeck(vec_t *hitloc)
{
	double cos;
	vec_t unit;

	//compute a unit vector from 
	//the location of the spotlight to the hitloc
	vec_diff(&location, hitloc, &unit);
	vec_unit(&unit, &unit);

	//take dot product of this vector with a unit vector in the direction
	//of the centerline of the spotcone
	cos = vec_dot(&unit, &direction);

	//if this value is larger than costheta the hit location is illuminated
	if(cos > costheta)
		return 1;
	else
		return 0;
}
