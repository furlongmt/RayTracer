/* list.c */

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

link_t::link_t(void *newentity)
{
	next = NULL;
	entity = newentity;
}

link_t::~link_t(void)
{
   fprintf(stderr, "in link destructor \n");
}

void link_t::set_next(link_t *new_next)
{
	next = new_next;
}

link_t * link_t::get_next()
{
	return next;
}

void * link_t::get_entity()
{
	return entity;
}


list_t::list_t()
{
	first = NULL;
	last = NULL;
	current = NULL;
}


void list_t::add(void *entity)
{
	/* Create a new link passing it the entity pointer */
	link_t *link;
	link = new link_t(entity);

	/* Now add the link to the list using generally the */
	/* same approach as the C version                   */
	if(first == NULL)
		first = link;
	else
		last->set_next(link);
		//current->get_next() = link;
	
	current = link;
	last = link;
	current->set_next(NULL);
	//link->get_next() = NULL;
}

void list_t::reset(void)
{
	current = first;
}

int list_t::not_end(void)
{
	if(current != NULL)
		return 1;
	else 
		return 0;
}

void list_t::next_link(void)
{
   assert(current != NULL);
	current = current->get_next();
}

void *list_t::get_entity(void)
{
   assert(current != NULL);
	return current->get_entity();
}

list_t::~list_t()
{
   current = first;
	link_t *kill;
   fprintf(stderr, "in list destructor \n");

	/* Delete each link_t in the list */	
	while(not_end()==1)
	{
		kill = current;
		//current = current->get_next();
		next_link();
		delete kill;
	}
}

void list_t::insert(void *entity)
{
	link_t *traverse, *link;
	
	link = new link_t(entity);
	if((current == NULL) && (first == NULL) &&  (last == NULL))
	{
		first = link;
		last = link;
	}
	else if (current == first)
	{
		link->set_next(first);
		first = link;
	}
	else{
		traverse = first;
		while(not_end() == 1)
		{
			if(traverse->get_next() == current)
				break;
			traverse = traverse->get_next();
		}
		traverse->set_next(link);
		link->set_next(current);
	}
	current = link;
}

void list_t::remove(void)
{
	fprintf(stderr,"destroying link_t\n");

	link_t *traverse;

	if(first == NULL)
		return;
	else if(first == last)
	{
		first = NULL;
		current = NULL;
		last = NULL;
	}
	else if(current == first)
	{
		first = first->get_next();
		current = first;
	}
	/*else if (current == last)
	{
		while(not_end() == 1)
		{
			if(traverse->get_next() == current)
				break;
		}
	}*/
	else {
		traverse = first;
		while(not_end() == 1)
		{
			if(traverse->get_next() == current)
				break;
			traverse = traverse->get_next();
		}
		if(current == last)
		{
			last = traverse;
			traverse->set_next(NULL);
		}
		else{
			traverse->set_next(current->get_next());	
		}
		current = traverse;
	}
}
