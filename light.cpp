//matthew furlong
//october 27 2014 cpsc 1020
//functions for light object in raytracer program

#include "ray.h"
#include "rayhdrs.h"
#include <math.h>

pparm_t light_parse [] =
{
	//{"name", 1, sizeof(NAME_LEN), "%s", 0},
	{"location", 3, sizeof(double), "%lf", 0},
	{"emissivity", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(light_parse) / sizeof(pparm_t))

//light_t constructor
light_t::light_t(FILE *in, model_t *model, int attrmax) 
{
	int mask;
	char buff[20];

	fscanf(in, "%s", name);

	fscanf(in, "%s", buff);
	assert(buff[0] == '{');

	light_parse[0].loc = &location;
	light_parse[1].loc = &emissivity;
	mask = parser(in, light_parse, NUM_ATTRS, attrmax);

	assert(mask > 0);

	model->lgts->add((void *)this);
}

//getter function for emmissivity
void light_t::getemiss(drgb_t *emis)
{
	pix_copy(&emissivity, emis);
}

//getter function for location 
void light_t::getloc(vec_t *loc)
{
	vec_copy(&location, loc);
}

//printer function for light
void light_t::printer(FILE * out)
{
	fprintf(out, "%s \n", name);

	//function from vector.c
	vec_print(out, "location", &location);
	
	fprintf(out, "emissivity %8.1lf %8.1lf %8.1lf \n\n", emissivity.r, 
															emissivity.g, emissivity.b);
}

//produce a formatted print of the light list 
void light_list_print(model_t *model, FILE *out)
{
	list_t *list;
	light_t *light;

	list = model->lgts;
	list->reset();

	while(list->not_end() == 1)
	{
		light = (light_t *) list->get_entity();
		light->printer(out);
		list->next_link();
	}
}

//illumination function
void light_t::illuminate(
	model_t *model,
	vec_t *base,
	object_t *hitobj, //The object hit by the ray
	drgb_t *pixel		//add illumination here
)
{
	vec_t dir;        //unit direction to light from hitpt
	object_t *obj;          //closest object in dir to light
	double close;		//dist to closest object in dir to light
	double cos;		// of angle between normal and dir to light
	double dist; 		//distance to the light from the hitpoint
	vec_t lasthit; 	//from hitobj to arguement
	vec_t lastnorm;
	drgb_t diffuse = {0.0, 0.0, 0.0};
	int vcheck;

	//copy last_hitpt from hitobj to hitpts copy variable
	hitobj->getlast_hitpt(&lasthit);
	
	//get the vector from last_hitpt to the light's location
	vec_diff(&lasthit, &location, &dir); 
	//vec_diff(&location, &lasthit, &dir);

	//compute the length of the previous vector
	dist = vec_len(&dir);

	//get the the unit vector of dir
	vec_unit(&dir, &dir);
	
	//test for self occluding light
	//dot product of vector from hitpoint to the light must be postive
	hitobj->getlast_normal(&lastnorm);
	cos = vec_dot(&dir, &lastnorm);
	
	if(cos <= 0)
		return;
	
	//call vischeck to see if spotlight or other light source
	vcheck = vischeck(&lasthit);
	
	//if vcheck = 0 return
	if(vcheck == 0)
		return; 

	//ask find_closest_object if a ray fired toward the 
	//light hits another object
	obj = find_closest_object(model, &lasthit, &dir, hitobj, &close);

	//make sure obj is hit and that the new distance is less than the previous
	if((obj != NULL) && (close < dist))
		return;

	//use hitobj function to return diffuse reflectivity into diffuse
	hitobj->getdiffuse(&diffuse);

	//multiply the diffuse reflectivity by the emissivity of the light
	pix_prod(&diffuse, &emissivity, &diffuse);
	
	//scale the resulting diffuse by cos/dist
	pix_scale((cos / dist), &diffuse, &diffuse);

	//add scaled value to pixel
	pix_sum(&diffuse, pixel, pixel);

	add_glint(hitobj, base, &dir, pixel);
}

int light_t::vischeck(vec_t *)
{
	return 1;
}

void light_t::add_glint(object_t *hitobj, vec_t *base, vec_t *dir, drgb_t * pixel)
{
	double shine, dot, spec;
	vec_t dir2, lasthit, lastnorm;
	drgb_t glint;

	hitobj->getshine(&shine);

	if(shine == 0)
		return;

	//copy last_hitpt from hitobj to hitpts copy variable
	hitobj->getlast_hitpt(&lasthit);
	
	//compute a unit vector from the hitpoint to the source of the ray
	vec_diff(&lasthit, base, &dir2);
	vec_unit(&dir2, &dir2);

	//vecsum previous two vectors
	vec_sum(&dir2, dir, dir);

	//vecunit to unitize that sum
	vec_unit(dir, dir);

	//compute dot equal to unit sum with the unit normal at that hitpoint
	hitobj->getlast_normal(&lastnorm);
	dot = vec_dot(&lastnorm, dir);

	//raise this variable dot to the value of shine
	dot = pow(dot, shine);
	
	//use getspecular to get the specular value from the hitobj
	hitobj->getspecular(&spec);
	
	//scale emissivity by dot time specular
	pix_scale((dot * spec), &emissivity, &glint);

	//add the scaled value to the pixel
	pix_sum(&glint, pixel, pixel);
}
