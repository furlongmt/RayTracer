//matthew furlong cpsc 1020
//november 10 2014
//mfurlon
//contains functions for the texture class in the raytracer program

#include "ray.h"


texture_t::texture_t(FILE *in, model_t *model, int attrmax)
{
	char buff[32];
	
	//read the texture name
	fscanf(in, "%s", name);
	
	//consume the delimeter
	fscanf(in, "%s", buff);
	assert(buff[0] == '{');
	
	fscanf(in, "%s", buff);
	fscanf(in, "%s", filename);
	
	fscanf(in, "%s", buff);
	assert(buff[0] == '}');
	
	load_texture();
	
	model->cam->getpixsize(pix_x_size, pix_y_size);

	model->texs->add((void *)this);
}

void  texture_t::texture_fit(double relx, double rely, drgb_t *pix)
{
	relx *= xdim;
	rely *= ydim;
	
	gettexel(relx, rely, pix);
}

void  texture_t::texture_tile(double worldx, double worldy, drgb_t *pix)
{
	double dx, dy;
	
	dx = worldx / pix_x_size;
	dy = worldy / pix_y_size;

	dx = (int) dx % xdim;
	dy = (int) dy % ydim;
	
	gettexel(dx, dy, pix);
}

void  texture_t::gettexel(int xpix, int ypix, drgb_t *texel)
{
	int maprow;
	irgb_t maploc;
	
	maprow = ydim - ypix - 1;
	maploc = imagebuf[maprow * ydim + xpix];

	texel->r = (double) maploc.r;
	texel->g = (double) maploc.g;
	texel->b = (double) maploc.b;

	texel->r /= 255;
	texel->g /= 255;
	texel->b /= 255;

	//pix_scale((1/255), texel, texel);
}


void texture_t::load_texture(void)
{
	char buff[80];
	int color_comp;
	
	FILE *ppmFile = fopen(filename, "r");
	//FILE *temp = fopen("temp.ppm", "w");
	
	if(ppmFile == NULL)
	{
		fprintf(stderr, "ERROR opening input file %s\n", filename);
		exit(1);
	}
	
	fgets(buff, 80, ppmFile);
	
	do{
		fgets(buff, 80, ppmFile);
	} while(buff[0] == '#');
	
	sscanf(buff, "%d %d %d", &xdim, &ydim, &color_comp);
	
	do{
		fgets(buff, 80, ppmFile);
	} while(buff[0] == '#');
	
	sscanf(buff,"%d", &color_comp);
		
	imagebuf = (irgb_t *) malloc(sizeof(irgb_t) * xdim * ydim);
	
	fread(imagebuf, sizeof(irgb_t), ydim * xdim, ppmFile);
	
	//fprintf(temp, "P6\n %d %d 255\n", xdim, ydim);
	//fwrite(imagebuf, sizeof(irgb_t), ydim * xdim, temp);
}

texture_t *texture_getbyname(model_t *model, char *name)
{
	texture_t *tex;
	list_t *list = model->texs;
	
	list->reset();
	while(list->not_end())
	{
		tex = (texture_t *) list->get_entity();
		if(strcmp(name, tex->name) == 0)
			return(tex);

		list->next_link();
	}
	return NULL;
}
