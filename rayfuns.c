/* rayfuns.h */


#include "ray.h"

/* return the index of the target string in the   */
/* table of string pointers. return -1 on failure */
int getndx(char *idtab[], int count, char *target) {
   int i;

   int rc = -1;

   for (i = 0; i < count; i++) {
      if (strcmp(target, idtab[i]) == 0)
         return(i);
   }
   return(rc);
}


/* int count is the number of attributes */
/* char *target is the candidate attribute name */
int getndxp(pparm_t *parsetab, int  count,  char *target) {
   int i;
   pparm_t *ppe = parsetab;

   int rc = -1;

   for (i = 0; i < count; i++) {

      if (strcmp(target, ppe->attrname) == 0)
         return(i);
      ppe += 1;
   }
   return(rc);
}

