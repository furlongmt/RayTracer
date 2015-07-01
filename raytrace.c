/*  raytrace.c  */

/*  find_closest_object() function
    arguments:
       model_t *model - the model structure
       vect_t *base - base of the ray (viewpoint)
       vect_t *dir - unit direction vector of the ray
       object_t *last_hit - the last hit object
       double *retdist - store distance to the hit point here

     	 if no object is hit, then retdist will have the value -1, 
	       and NULL will be returned 
		 if an object is hit, retdist will end up with the mindist
		    and minobj will be a non-NULL object that is returned 
*/


#include "ray.h"


object_t *find_closest_object(model_t  *model, vec_t  *base, vec_t  *dir,
                              object_t  *last_hit, double  *retdist)     {
	object_t *minobj = NULL;
	double mindist = -1.0;
	// other declarations
	object_t *obj;
	double dist = 0;
	
	
	// process entire list of objects to see if any are hit by the ray
	// how do you process a list??
	model->objs->reset();
	
	while(model->objs->not_end() == 1)
	{
		obj = (object_t *) model->objs->get_entity();
		
		if(obj != last_hit)
		{
			dist = obj->hits(base,dir);
			if(dist > 0 && (dist < mindist || mindist == -1))
			{
				mindist = dist;
				minobj = obj;
			}
		}
		
		model->objs->next_link();
	}
	
	*retdist = mindist;
	//if(minobj == NULL)
	//	minobj = obj;	
	
	return minobj;
}

void add_illumination(
	model_t *model,   //object list
	vec_t *base,      //base of ray.. not used yet
	object_t *hitobj, //object that was hit by the ray
	drgb_t *pixel 		//Where to add intensity 
)
{
	light_t *light;
	list_t *list;

	list = model->lgts;
	list->reset();
	while(list->not_end() == 1)
	{
		light = (light_t *) list->get_entity();
		light -> illuminate(model, base, hitobj, pixel);
		list->next_link();
	}
}
