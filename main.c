/* main.c */

#include "ray.h"

int main(
int argc,
char *argv[])
{
   model_t *model;
   FILE *in;
   FILE *out;
   
   assert(argc >= 3);
   in = fopen(argv[1], "r");
   assert(in != NULL);
   
   out = fopen(argv[2], "w");
   assert(out != NULL);
   

	/* Load and dump the model */
   model = new model_t(in);
   assert(model != NULL);

   model->print(stderr);

	/* Raytrace the image */
   image_create(model, out);

   return(0);
}

