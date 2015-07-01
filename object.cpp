/* object.c - this file is given */

#include "ray.h"

int oparse = 1;


/* Create a new object description */
object_t::object_t(FILE  *in, model_t  *model) {
   char name[NAME_LEN];
   int count;

   cookie = OBJ_COOKIE;

   if (!oparse)
      return;

	/* Read the descriptive name of the object */
	/* left_wall, center_sphere, etc.          */
   count = fscanf(in, "%s", obj_name);
   assert(count == 1);

	/* Consume the delimiter */
    count = fscanf(in, "%s", name);
    assert(*name == '{');

	/* First attribute must be material */
   count = fscanf(in, "%s", name);
   assert(count == 1);
   count = strcmp(name, "material");
   assert(count == 0);
   count = fscanf(in, "%s", name);
   assert(count == 1);

   mat = material_getbyname(model, name);
   assert(mat != NULL);

	/* Add the object to the list */
   model->objs->add((void *)this);
}


void object_t::getambient(drgb_t *amb) {
   mat->material_getambient(amb);
}

void object_t::getdiffuse(drgb_t *diff) {
   mat->material_getdiffuse(diff);
}

void object_t::getspecular(double *spec) {
   mat->material_getspecular(spec);
}

void object_t::getshine(double *shiny) {
   mat->material_getshine(shiny);
}

void object_t::gettrans(double *trans) {
   mat->material_gettrans(trans);
}

inline void object_t::printer(FILE  *out) {
   assert(cookie == OBJ_COOKIE);
   fprintf(out, "%-12s %s \n", obj_type, obj_name);
   if (this->mat)
      fprintf(out, "%-12s %s \n", "material",
                   this->mat->material_getname());
}



/* This function can't be a class method because */
/*  1- it must process EVERY object and          */
/*  2- it doesn't need to access private data    */
/*     of ANY object                             */
void object_list_print(model_t  *model, FILE  *out) {
   object_t *obj;
   list_t *list = model->objs;

   list->reset();
   while (list->not_end())
   {
      obj = (object_t *)list->get_entity();
      obj->printer(out);
      fprintf(out, "\n");
      list->next_link();
   }
}


