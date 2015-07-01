/* material.cpp - this file is given 

	uses the generalized parser; if you don't want to use the
	  generalized parser, you can convert your material.c to
	  a material.cpp file, or change this file to use the old
	  way of parsing (load_attributes function)

	functions in this file:
		material_t::material_t() - constructor
		material_t::material_getname() - getter - returns the name
		friend function material_getbyname() - not a class function
		material_list_print() - still a C function; not a friend
			function, so prototype is in rayhdrs.h
		material_t::material_print()
		other getter functions:
			material_t::material_getambient()
			material_t::material_getdiffuse()
			material_t::material_getspecular()
			material_t::material_getshine()
			material_t::material_gettrans()
*/


#include "ray.h"

// for the generalized parsher
static pparm_t mat_parse[] =
{
   {"ambient",      3, 8, "%lf", 0},
   {"diffuse",      3, 8, "%lf", 0},
   {"specular",     1, 8, "%lf", 0},
   {"shininess",    1, 8, "%lf", 0},
   {"transparency", 1, 8, "%lf", 0},
};

#define NUM_ATTRS (sizeof(mat_parse) / sizeof(pparm_t))


//=============================================================================
// material constructor - takes the place of material_init() function
// *** create a new material description ***
//=============================================================================
material_t::material_t(FILE  *in, model_t  *model, int  attrmax) {
   char attrname[NAME_LEN];
   int count;
   int mask;

	/* Create a new material structure and initialize it */
   cookie = MAT_COOKIE;

	/* Read the descriptive name of the material */
	/* (dark_red, light_blue, etc.               */
   count = fscanf(in, "%s", name);
   assert(count == 1);

   count = fscanf(in, "%s", attrname);
   assert(*attrname == '{');

   mat_parse[0].loc = &ambient;
   mat_parse[1].loc = &diffuse;
   mat_parse[2].loc = &specular;
   mat_parse[3].loc = &shininess;
   mat_parse[4].loc = &transparency;

	// calls the generic parser
   mask = parser(in, mat_parse, NUM_ATTRS, 0);

   assert(mask != 0);

	// add to list of materials
   model->mats->add((void *)this);
}


//=============================================================================
// getter function for name of the material 
// called from within material_getbyname() function
//=============================================================================
char * material_t::material_getname() {
   return(name);
};


//=============================================================================
// material_getbyname() function 
//
// still a C function 
//   defined as a "friend" function in the material class
//	  not a class function because it has to look through
//	    all the materials to find the one it's looking for
// since this function is defined as a "friend" function,
//	  it doesn't have to use the getter function as it does below
//	  it could say the following instead:
//       if (strcmp(name, mat->name) == 0)
//=============================================================================
material_t *material_getbyname(model_t  *model, char  *name) {
   material_t *mat;
   list_t *list = model->mats;

   list->reset();
   while (list->not_end()) {
      mat = (material_t *)list->get_entity();
      if (strcmp(name, mat->material_getname()) == 0)
         return(mat);

      list->next_link();
   }

   return(NULL);
}


//=============================================================================
// still a C function  
//   not a friend function, so prototype in rayhdrs.h 
//   instead of inside of class definition
//=============================================================================
void material_list_print(model_t  *model, FILE  *out) {

   material_t *mat;
   list_t *list = model->mats;
   list->reset();
   while (list->not_end())
   {
      mat = (material_t *)list->get_entity();
      mat->material_print(out);
      list->next_link();
   }
}


//=============================================================================
// replaces material_print() function
//=============================================================================
void material_t::material_print(FILE  *out) {

   fprintf(out, "\n%-12s %s \n", "material", name);
   fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "ambient",
                 ambient.r, ambient.g, ambient.b);
   fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "diffuse",
                 diffuse.r, diffuse.g, diffuse.b);

   fprintf(out, "%-12s %5.1lf \n\n", "specular",
                 specular);

   fprintf(out, "%-12s %5.1lf \n\n", "shininess",
                 shininess);
   fprintf(out, "%-12s %5.1lf \n\n", "transparency",
                 transparency);
}


//=============================================================================
// getter function for ambient value
//	  uses pix_copy instead of returning
//=============================================================================
void material_t::material_getambient(drgb_t  *dest) {

   assert(cookie == MAT_COOKIE);
   pix_copy(&ambient, dest);
}


//=============================================================================
// getter function for diffuse value
//	  uses pix_copy instead of returning
//=============================================================================
void material_t::material_getdiffuse(drgb_t  *dest) {

   assert(cookie == MAT_COOKIE);
   pix_copy(&diffuse, dest);
}


//=============================================================================
// getter function for specular value
//	  uses assignment instead of returning
//=============================================================================
void material_t::material_getspecular(double  *spec) {

   assert(cookie == MAT_COOKIE);
   *spec = specular;
}


//=============================================================================
// getter function for shine value
//	  uses assignment instead of returning
//=============================================================================
void material_t::material_getshine(double  *shiny) {
   *shiny = shininess;
}


//=============================================================================
// getter function for transparency value
//	  uses assignment instead of returning
//=============================================================================
void material_t::material_gettrans(double  *trans) {
   *trans = transparency;
}

