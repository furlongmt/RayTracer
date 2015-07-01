/*  image.c  */

/*  functions:
    	ray_trace() - code to add
		make_pixel() - complete function provided
		make_row() - code to add
		image_create() - code to add
*/

#include "ray.h"


//=============================================================================
// ******* ray_trace( ) *******
// 
// 
//=============================================================================
void ray_trace (
   model_t *model,
   vec_t *base,                // view point
   vec_t *dir,                 // unit direction vector
   drgb_t *pix,                // pixel return location
   double total_dist,          // distance ray has travelled so far
   object_t *last_hit)         // most recently hit object
{
	double dist;
	drgb_t thispix = {0.0, 0.0, 0.0};
	object_t *obj;
	double specref = 0.0;
	double trans;

	//conditional to test for specular light
	if(total_dist > MAX_DIST)
		return;

   /* Get the object (plane or sphere).        */
	/* Later we will use find_closest_object()  */
   /* to get the closest object                */
   //list_reset(model->objs);
	//obj = (object_t *)list_get_entity(model->objs);
	obj = (object_t *) find_closest_object(model, base, dir, last_hit, &dist);
	
	if(obj == NULL)
		return;
	/* get the distance and the color */
   //obj->ambient(obj, obj->mat, &thispix);
	obj->getambient(&thispix);
	add_illumination(model, base, obj, &thispix);

   /**  check to see if object was hit.  if so
        1)  add this distance to the total distance travelled so far
        2)  scale thispix by 1/total_distance
        3)  add thispix to pix
   **/   
	
	//if statement checks to see if object was hit
	//obj->hits reutrns -1 if no object was hit
	if(dist != -1)
	{
		total_dist += dist;
		pix_scale(1/total_dist,&thispix, &thispix);
		//pix_sum(&thispix, pix, pix);
	}

	obj->gettrans(&trans); //see if object has transparency

	if(trans != 0)
	{
		double maxpix;
		drgb_t transint = {0.0,0.0,0.0};
		drgb_t diffcolor = {0.0,0.0,0.0}; //diffuse color of transparent object
		vec_t last_hits;
		
		//scale this pix by 1 - trans
		pix_scale((1-trans),&thispix, &thispix);

		//get last hit from obj
		obj->getlast_hitpt(&last_hits);
		ray_trace(model, &last_hits, dir, &transint, total_dist, obj);
		
		//scale transint by trans
		pix_scale(trans, &transint, &transint);

		//compute diffcolor and maxpix
		maxpix = pix_max(&thispix);	
		obj->getdiffuse(&diffcolor);

		//scale diffcolor by trans/maxpix	
		pix_scale((trans/maxpix), &diffcolor, &diffcolor);
		
		//multiply componenet wise transint by scalled dif color
		pix_prod(&transint, &diffcolor, &diffcolor);
		
		//add transint to this pix
		pix_sum(&transint, &thispix, &thispix);
	}

	//see if object has specular reflectivity
	obj->getspecular(&specref); 
	if(specref != 0)
	{
		drgb_t specint = {0.0, 0.0, 0.0};	
		vec_t ref_dir, last_norm, last_hit;
	
		//get last_normal from object
		obj->getlast_normal(&last_norm);

		//get last hit from object
		obj->getlast_hitpt(&last_hit);

		//compute direction ref_dir of the reflected ray
		vec_reflect(&last_norm,dir, &ref_dir);

		ray_trace(model, &last_hit, &ref_dir, &specint, total_dist, obj);

		//scale specint by specref
		//pix_prod(&specref, &specint, &specint);
		pix_scale(specref, &specint, &specint);	

		//add specint to thispix
		pix_sum(&specint, &thispix, &thispix);
	}
	pix_sum(&thispix, pix, pix);
}


//=============================================================================
// ******* make_pixel( ) *******
// 
// 
//=============================================================================
void make_pixel(model_t  *model, int  x, int  y) {
   vec_t raydir, viewpoint;
   drgb_t pix = {0.0, 0.0, 0.0};
   camera_t *cam = model->cam;
	int i;

	cam -> getviewpt(&viewpoint);
 	
	for(i = 0; i < AA_SAMPLES; i++)
	{
		cam -> getdir(x, y, &raydir);
   	
		/*  The ray_trace function determines the pixel color in */
  		/*  d_rgb units.   The last two parameters are used ONLY */
   	/*  in the case of specular (bouncing) rays              */
		ray_trace(model, &viewpoint, &raydir, &pix, 0.0, NULL);
	}

	pix_scale(1.0 / AA_SAMPLES, &pix, &pix); 

   cam -> store_pixel(x, y, &pix);

   return;
}


//=============================================================================
// ******* make_row( ) *******
// 
// 
//=============================================================================
void make_row(model_t  *model, int y) 
{
  	int x;
	camera_t *cam = model->cam;

   /*  for each pixel in the row, invoke make_pixel() */
   /*  to paint the pixel.                            */
	//  CODE HERE
	for(x=0;x < cam->getxdim(); x++)
	{
		//call make pixel function from above
		make_pixel(model,x,y);
	}
}


//=============================================================================
// ******* image_create( ) *******
// 
// 
//=============================================================================
void image_create(model_t  *model, FILE  *out) 
{
   int y;
   camera_t *cam = model->cam;

   /*  fire rays through each pixel in the window; */
   /*  for each row, invoke make_row() to paint    */
   /*  the row of pixels.                          */
	for(y=0; y < cam->getydim(); y++)
	{
		//call make row from function above
		make_row(model, y);
	}
   /*  write the image from camera.c file                          */
   cam->camera_write_image(out);
}

