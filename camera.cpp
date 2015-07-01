//matthew furlong cpsc 1020
//october 26 2014
//mfurlon T-Th 12:30 - 1:45
//file with functions for camera in raytracer program

#include "ray.h"

//cam_parse array 
pparm_t camera_parse[] =
{
	{"pixeldim", 2, sizeof(int), "%d", 0},
	{"worlddim", 2, sizeof(double), "%lf", 0},
	{"viewpoint", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(camera_parse) / sizeof(pparm_t))

//constructor for camera object
camera_t::camera_t(FILE *in)
{
	char buf[256];
	int mask;

	assert (fscanf(in, "%s", name) == 1);
	
	fscanf(in, "%s", buf);
	assert(buf[0] == '{');

	//check to make sure it's a camera
	cookie = CAM_COOKIE;
	
	//set camera_parse locations
	camera_parse[0].loc = &pixel_dim;
	camera_parse[1].loc = &world_dim;
	camera_parse[2].loc = &view_point;
	
	//load the attributes from the file to the camera object
	//uses parser function in parser.c
	mask = parser(in, camera_parse, NUM_ATTRS, 0);
	assert(mask == 7);
	
	/* Allocate a pixmap to hold the ppm image data */
	pixmap = (irgb_t *)malloc(sizeof(irgb_t) * pixel_dim[0] * pixel_dim[1]);
}

//function to get vec_units for camera coordinates
void camera_t::getdir(int x, int y, vec_t *dir)
{
	assert(cookie == CAM_COOKIE);
	vec_t world;
	double dx = x;
	double dy = y;

	if(AA_SAMPLES > 1)
	{
		dx = randomize(dx);
		dy = randomize(dy);
	}

	world.x = (float) dx / (pixel_dim[X] - 1) * world_dim[X];
	world.y = (float) dy / (pixel_dim[Y] - 1) * world_dim[Y];
	world.z = 0.0;

	//functions from vector.c
	vec_diff(&view_point, &world, dir);
	vec_unit(dir, dir);
}

void camera_t::store_pixel(int x, int y, drgb_t *pix)
{
	 int maprow;
	 irgb_t *maploc;
	 
	 maprow = pixel_dim[1] - y - 1;
	 maploc = pixmap + maprow * pixel_dim[0] + x;
	 
	 //scale and clamp function below
	 scale_and_clamp(pix);
	 
	 maploc->r = (unsigned char)pix->r;
	 maploc->g = (unsigned char)pix->g;
	 maploc->b = (unsigned char)pix->b;
}

//getter function to return x dimension
int camera_t::getxdim(void)
{
	return pixel_dim[0];
}

//getter function to return y dimension
int camera_t::getydim(void)
{
	return pixel_dim[1];
}

//getter function to set vec_t view = to the unit vec of viewpt
void camera_t::getviewpt(vec_t *view)
{
	vec_copy(&view_point, view);
}

//function to dump camera contents
void camera_t::camera_print(FILE *out)
{
	assert(cookie == CAM_COOKIE);

	fprintf(out, "camera %s \n", name);
	fprintf(out, "pixeldim %d %d \n", pixel_dim[X], pixel_dim[Y]);
	fprintf(out, "worlddim %.1lf %.1lf \n", world_dim[X], world_dim[Y]);

	//function from vector.c
	vec_print(out, "viewpoint", &view_point);
}

//function to write PPM final image
void camera_t::camera_write_image(FILE *out)
{
	//print header for ppm
	fprintf(out, "P6 \n %d %d 255\n", pixel_dim[X], pixel_dim[Y]);

	//write image to field
	fwrite(pixmap, sizeof(irgb_t), pixel_dim[X] *  pixel_dim[Y], out);
}

//scale drgb_t pixels to make them rgb pixels
//then clamp them to make sure they're not over 255 or less then 0
void scale_and_clamp(drgb_t *pix)
{
	//scale pixels
	pix -> r = pix->r * 255 + .5;
	pix -> g = pix->g * 255 + .5;
	pix -> b = pix->b * 255 + .5;

	//clamp pixels to be sure they're less than 255 and greater than 0
	if(pix->r > 255)
		pix->r = 255;
	
	if(pix->g > 255)
		pix->g = 255;
	
	if(pix->b > 255)
		pix->b = 255;
	
	if(pix->r < 0.0)
		pix->r = 0.0;
	
	if(pix->g < 0.0)
		pix->g = 0.0;

	if(pix->b < 0.0)
		pix->b = 0.0;
}

double randomize(double coord)
{
	int a;
	double b;

	a = rand();
	b = (double) a;
	b /= 0x7fffffff;
	b -= .5;

	return b + coord;
	
}

void camera_t::getpixsize(double &x, double &y)
{
	x = world_dim[X] / pixel_dim[X];
	y = world_dim[Y] / pixel_dim[Y];
}
