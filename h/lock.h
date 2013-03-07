/* lock.h - isbadlock */

#ifndef _LOCK_H_
#define _LOCK_H_

#ifndef NLOCKS
#define NLOCKS            50      /* number of locks, if not defined */
#endif

#define LFREE	1           /* this semaphore is free               */
#define READ	2           /* this semaphore is used               */
#define WRITE	3
#define LDELETED 4
#define LUSED	5

#define isbadlock(l)     (l<0 || l>=NLOCKS)

struct  lentry  {               /* semaphore table entry                */
        int    lstate;         /* the state SFREE or SUSED             */
	int 	holder[50];
	int 	r_count;
	int 	owner;	
	int 	max_count_reader;
	int     lhead;         /* q index of head of list              */
        int     ltail;         /* q index of tail of list              */
	int 	lprio;
};
struct  lentry  lockarr[];
int     nextlock;



#endif
