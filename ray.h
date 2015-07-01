/* ray.h */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <cstring>

#define NAME_LEN   32        /* max length of entity names */

#define OBJ_COOKIE 12345678
#define MAT_COOKIE 32456123
#define LGT_COOKIE 30492344
#define CAM_COOKIE 49495923

#define MAX_DIST   32
#define AA_SAMPLES  7

#include "vector.h"
#include "list.h"
#include "pixel.h"

class camera_t
{
	public:  // member functions
   	camera_t();
   	camera_t(FILE *in);
   	void getdir(int x, int y, vec_t *dir);
   	void store_pixel(int x,int y, drgb_t *pix);
   	int  getxdim(void);
   	int  getydim(void);
   	void getviewpt(vec_t *view);
   	void camera_print(FILE *out);
   	void camera_write_image(FILE *out);
   	void getpixsize(double &x, double &y);

	private:  // member variables
   	int    cookie;
   	char   name[NAME_LEN];
   	int    pixel_dim[2];    /* Projection screen size in pix */
   	double world_dim[2];    /* Screen size in world coords   */
   	vec_t  view_point;      /* Viewpt Loc in world coords    */
   	irgb_t *pixmap;         /* Build image here              */
};


class model_t
{
	public:  // member variables
   	model_t(FILE *);
   	void print(FILE *);
   	camera_t  *cam;
   	list_t    *mats;
   	list_t    *objs;
   	list_t    *lgts;
	list_t	  *texs;

	private:  // member methods
   	inline void model_load_entity(FILE *, char *);
   	inline void model_load(FILE *);

};


/* Used for the generalized attributed parser */
typedef struct pparm_type
{
   const char *attrname;    /* Attribut name                  */
   int  numvals;            /* Number of attribute values     */
   int  valsize;            /* Size of attribute in bytes     */
   const char *fmtstr;      /* Format string to use           */
   void *loc;               /* Where to store first attribute */
}  pparm_t;


class material_t
{

	friend material_t *material_getbyname(model_t *, char *);

	public: // member functions
   	material_t(){};
   	material_t(FILE *in, model_t *model, int attrmax);
   	void   material_getambient(drgb_t   *dest);
   	void   material_getdiffuse(drgb_t  *dest);
   	void   material_getspecular(double  *spec);
   	void   material_getshine(double *shiny);
   	void   material_gettrans(double *trans);
   	char   *material_getname();
   	void   material_print(FILE *out);
	
	private:  // member variables
   	int     cookie;
   	char    name[NAME_LEN];
   	drgb_t  ambient;         /* Reflectivity for materials  */
   	drgb_t  diffuse;
   	double  specular;
   	double  shininess;
   	double  transparency;
};


class model_t;


class texture_t
{
	friend texture_t *texture_getbyname(model_t *, char *);
	
	public:
   	texture_t();
   	texture_t(FILE *in, model_t *model, int attrmax);
   	void  texture_fit(double relx, double rely, drgb_t *);
   	void  texture_tile(double worldx, double worldy, drgb_t *);
	void load_texture(void);
	
	private:
   	void  gettexel(int, int, drgb_t *);
   	char name[NAME_LEN]; //descriptive name
   	char filename[NAME_LEN]; //file name
   	double  pix_x_size;   // pixel size in world coords
   	double  pix_y_size;
   	int     xdim;         // of the texture
   	int     ydim;
   	irgb_t  *imagebuf;
};


class object_t
{
	public:
   	object_t(){};
   	object_t(FILE *in, model_t *model);
   	virtual ~object_t(){};
	
   	virtual  double hits(vec_t *base, vec_t *dir){return(-1.0);};
   	virtual  void   printer(FILE*);
	
   	virtual  void   getlast_hitpt(vec_t *loc){ vec_copy(&last_hitpt, loc); }
   	virtual  void   getlast_normal(vec_t *loc){ vec_copy(&last_normal, loc); }
   	const    char   *getobjname(void) { return obj_name; };
	
   	virtual  void   getambient(drgb_t *);
   	virtual  void   getdiffuse(drgb_t *);
   	virtual  void   getspecular(double *);
   	virtual  void   gettrans(double *);
   	virtual  void   getshine(double *);
	
	protected:
   	vec_t   last_hitpt;         /* Last hit point              */
   	vec_t   last_normal;        /* Normal at hit point         */
   	int     cookie;
   	char    obj_type[NAME_LEN];  /* plane, sphere, ...       */
   	material_t *mat;    /* Surface reflectivity data */

	private:
   	char   obj_name[NAME_LEN];  /* left_wall, center_sphere */
};


class light_t
{
	public:
   	light_t(){};
   	light_t(FILE *in, model_t *model, int attrmax);
	
   	virtual ~light_t(){};
	
   	virtual void  getemiss(drgb_t *);
   	virtual void  getloc(vec_t *);
   	virtual int   vischeck(vec_t *);
   	virtual void  illuminate(model_t *, vec_t *, object_t *, drgb_t *);
   	virtual void  add_glint(object_t *, vec_t *, vec_t *, drgb_t *);
   	virtual void  printer(FILE *);

	protected:
   	vec_t   location;
   	drgb_t  emissivity;
   	char    name[NAME_LEN];

	private:
   	int   cookie;
};


class plane_t: public object_t
{
	public:
   	plane_t(){};
   	plane_t(FILE *, model_t *, int);

   	virtual double  hits(vec_t *base, vec_t *dir);
   	virtual void    printer(FILE *);

	protected:
   	vec_t  normal;
   	vec_t  point;

	private:
   	double  ndotq;
};


class revsurf_t: public object_t
{
	public:
   	revsurf_t(){};
   	revsurf_t(FILE *, model_t *, int);

   	virtual double  hits(vec_t *base, vec_t *dir);
   	virtual void    printer(FILE *);

	private:
   	double  s0_hits(vec_t *base, vec_t *dir);
   	int     surfer;
   	vec_t   base;         /* base location */
   	vec_t   direction;    /* centerline direction */
   	double  height;
   	material_t   rot;
   	material_t   irot;
   	double  surferp0;
   	double  surferp1;
};


class pplane_t: virtual public plane_t
{
	public:
   	pplane_t();
   	pplane_t(FILE *, model_t *, int);

   	virtual void  printer(FILE *);
   	virtual void  getambient(drgb_t *w);
   	virtual void  getdiffuse(drgb_t *w);

	private:
   	void s0_ambient(drgb_t *value);
   	void s1_ambient(drgb_t *value);
   	void s2_ambient(drgb_t *value);
   	void s3_ambient(drgb_t *value);
   	void s4_ambient(drgb_t *value);

   	int  shader;
};

class triangle_t: virtual public plane_t
{
	public:
	triangle_t();
	triangle_t(FILE *, model_t *, int);

	virtual void printer(FILE *);
	virtual double hits(vec_t *, vec_t *);

	private:
	vec_t point1;
	vec_t point2;
	vec_t point3;
};

class fplane_t: public plane_t
{
	public:
   	fplane_t();
   	fplane_t(FILE *, model_t *, int);

   	virtual double  hits(vec_t *base, vec_t *dir);
   	virtual void    printer(FILE *);
      void  get_dims(double *);
      void  get_rothit(vec_t *);

	protected:
   	vec_t  rothit;     /* last rotated hit point */
   	double dims[2];

	private:
   	mtx_t       rot;   /* rotation matrix        */
   	vec_t       xdir;

};

class hplane_t: public fplane_t
{
	public:
   	hplane_t();
   	hplane_t(FILE *, model_t *, int);

   	virtual double  hits(vec_t *base, vec_t *dir);
   	virtual void    printer(FILE *);

	private:
   	double hdims[2];  /* spacing of hole grid */
   	double radius;
};


class tplane_t: public plane_t
{
	public:
   	tplane_t();
   	tplane_t(FILE *, model_t *, int);
		virtual void    printer(FILE *);
   	virtual void    getambient(drgb_t  *);
   	virtual void    getdiffuse(drgb_t *);
		// virtual void    getspecular(drgb_t *);

	private:
   	int select(void);
   	double dims[2];
   	material_t *altmat;
   	mtx_t  rot;      /* rotation matrix        */
   	vec_t  xdir;
		char altname[32];
};


class pfplane_t: public pplane_t, public fplane_t
{
	public:
   	pfplane_t();
   	pfplane_t(FILE *, model_t *, int);
   	virtual void  printer(FILE *);
};


#if 0
typedef struct pshader_type
{
   void    (*getambient) (drgb_t *);
   void    (*getdiffuse) (drgb_t *);
}  pshader_t;
#endif


class texplane_t: public fplane_t
{
	public:
   	texplane_t(FILE *, model_t *, int);
   	texplane_t();
   	virtual void  printer(FILE *);
   	virtual void  getdiffuse(drgb_t *pix);
   	virtual void  getambient(drgb_t *pix);

	protected:
   	int           mode;
   	texture_t     *texture;

	private:
   	char          texname[64];
};


class projector_t: public light_t, public texplane_t
{
	public:
   	projector_t(FILE *, model_t *, int);
   	virtual int     vischeck(vec_t *);
   	virtual void    printer(FILE *);
   	virtual void    getemiss(drgb_t  *);
   	virtual double  hits(vec_t *base, vec_t *dir){ return(-1);}

	private:
   	vec_t direction;
};


class spotlight_t: public light_t
{
	public:
   	spotlight_t(FILE *, model_t *, int);
   	virtual int     vischeck(vec_t *);
   	virtual void    printer(FILE *);

	private:
   	double  halftheta;  // half angle in degrees
   	vec_t   point;      // point the centerline hits
   	vec_t   direction;  // centerline direction
   	double  costheta;   // cosine of the cone's half angle
};


class sphere_t: public object_t
{
	public:
   	sphere_t();
   	sphere_t(FILE *, model_t *, int);

   	virtual double  hits(vec_t *base, vec_t *dir);
   	virtual void    printer(FILE *);

	protected:
   	vec_t   center;
   	double  radius;
   	vec_t   scale;

	private:

};


class ellipsoid_t: public sphere_t
{
	public:
   	ellipsoid_t();
   	ellipsoid_t(FILE *, model_t *, int);

   	virtual void    printer(FILE *);

	private:

};


class cylinder_t: public object_t
{
	public:
   	cylinder_t();
   	cylinder_t(FILE *, model_t *, int);
	
   	virtual double  hits(vec_t *base, vec_t *dir);
   	virtual void    printer(FILE *);

	protected:
   	vec_t   center;
   	vec_t   centerline;
   	double  radius;
   	double  height;

	private:
   	material_t   rot;
   	material_t   irot;
};

class cone_t: public object_t
{
	public:
   	cone_t();
   	cone_t(FILE *, model_t *, int);

   	virtual double  hits(vec_t *base, vec_t *dir);
   	virtual void    printer(FILE *);

	protected:
   	vec_t  center;
   	vec_t  centerline;
   	double radius;
   	double height;

	private:
   	material_t  rot;
   	material_t  irot;
};

#if 0
class cone_t: public cyl_t
{
	public:
   	cone_t();
   	cone_t(FILE *, FILE *, int otype, int oid);
   	virtual double  hits(double *base, double *dir);
   	virtual void    print_obj(FILE *);
};


class parab_t: public cyl_t
{
	public:
   	parab_t();
   	parab_t(FILE *, FILE *, int otype, int oid);
   	virtual double  hits(double *base, double *dir);
   	virtual void    print_obj(FILE *);
};
#endif

class psphere_t: public sphere_t
{
	public:
   	psphere_t();
   	psphere_t(FILE *, model_t *, int);
   	virtual void getambient(drgb_t *);

	private:
   	int procedure;
};


#include "rayhdrs.h"

