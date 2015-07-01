/* == light.cpp == */
void light_list_print(model_t *, FILE *);

void add_illumination(model_t *model, vec_t *base, object_t *hitobj,
								drgb_t *pixel);

/* == camera.cpp == */

void scale_and_clamp(drgb_t *pix);

/*  == material.c  ==  */

material_t *material_getbyname(model_t  *, char  *);

void material_list_print(model_t  *, FILE  *);

/*  == object.c ==  */
void object_list_print(model_t  *, FILE  *);


/*  == image.c == */
void ray_trace (
   model_t *model,
   vec_t *base,             //  view point
   vec_t *dir,              //  unit direction vector
   drgb_t *pix,             //  pixel return location
   double total_dist,       //  distance ray has travelled so far
   object_t *last_hitobj);  //  most recently hit object

void make_pixel(model_t  *model, int  x, int  y);

void make_row(model_t  *, int);

void image_create(model_t  *, FILE  *);

//ask about this function below
object_t *find_closest_object(model_t *model, vec_t *base, vec_t *dir, 
										object_t *last_hit, double *retdist);

/*==  parser.c ==  */
int parser(
FILE    *in,
pparm_t *pct,         /* parser control table                */
int      numattrs,    /* number of legal attributes          */
int      attrmax);    /* Quit after this many attrs if not 0 */

double randomize(double);

/*==   texture.cpp   ==*/
texture_t *texture_getbyname(model_t *model, char *name);

/* == pixel.c   ==*/
double pix_max(drgb_t *pix);
