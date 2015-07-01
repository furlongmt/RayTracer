//matthew furlong cpsc 1020
//november 10 2014
//mfurlon
//contains functions for the texplane plane in the raytracer program

#include "ray.h"
#include <math.h>

pparm_t texplane_parse[] =
{
	{"mode", 1, sizeof(int), "%d", 0},
	{"texname",1, NAME_LEN, "%s", 0},
};

#define NUM_ATTRS (sizeof(texplane_parse) / sizeof(pparm_t))

texplane_t::texplane_t(FILE *in, model_t *model, int attrmax) 
														: fplane_t (in, model, 2)
{
	int mask;
	
	texplane_parse[0].loc = &mode;
	texplane_parse[1].loc = texname;
	mask = parser(in, texplane_parse, NUM_ATTRS, attrmax);
	
	assert(mask == 3);
	
	texture = texture_getbyname(model,texname);
	
	strcpy(obj_type, "texplane");
}

void texplane_t::printer(FILE *out)
{
	fplane_t::printer(out);
	fprintf(out, "texname %s \n", texname);
	fprintf(out, "mode %d \n", mode);
}

void texplane_t::getdiffuse(drgb_t *pix)
{
		drgb_t matdiff; //diffuse reflectivity of material
		drgb_t texel;
		vec_t lasthit;
		
		fplane_t::get_rothit(&lasthit);
		//acquire the diffuse reflectivity mat of of the underlying object
		mat->material_getdiffuse(&matdiff);
		
		//ask texture fit to return value of the texel hit
		if(mode == 0)
			texture->texture_fit((lasthit.x/dims[0]), 
									(lasthit.y/dims[1]),&texel);
		else
			texture->texture_tile(lasthit.x, lasthit.y, &texel);
			
		//store componenet wise product of matdiff and texel in pix
		pix_prod(&matdiff, &texel, pix);
}

void texplane_t::getambient(drgb_t *pix)
{
		drgb_t matdiff; //diffuse reflectivity of material
		drgb_t texel;
		vec_t lasthit;
		
		fplane_t::get_rothit(&lasthit);
		
		//acquire the diffuse reflectivity mat of of the underlying object
		mat->material_getambient(&matdiff);
		
		//ask texture fit to return value of the texel hit
		if(mode == 0)
			texture->texture_fit((lasthit.x/dims[0]), 
									(lasthit.y/dims[1]), &texel);
		else
			texture->texture_tile(lasthit.x, lasthit.y, &texel);
			
		//store componenet wise product of matdiff and texel in pix
		pix_prod(&matdiff, &texel, pix);
}
