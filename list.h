
#include <assert.h>

class link_t
{
	public:
   	link_t(void);     				// constructor
   	link_t(void *);   				// constructor
  		~link_t(void);    				// destructor
   	void     set_next(link_t *);  // setter
   	link_t  *get_next(void);      // getter
   	void    *get_entity(void);    // getter

	private:
   	link_t  *next;
   	void    *entity;

};

typedef class list_t
{
	public:
   	list_t(void);                 // constructor
   	list_t(const list_t &);       // copy constructor
  		~list_t(void);						// destructor
   	void   add(void *entity);     // add entity to end of list
   	void   reset(void);           // set current to start of list
   	void   *get_entity(void);     // get current entity in list
   	int    not_end(void);
   	void   next_link(void);

		/* New methods for lab 10 */
   	void   insert(void *);        // insert entity before current
   	void   remove(void );         // remove current entity

	private:
   	link_t *first;
   	link_t *last;
   	link_t *current;
} list_t;

