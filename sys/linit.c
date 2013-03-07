#include <lock.h>
#include <q.h>
extern int nextlock;
void linit()
{
	struct lentry *lptr;
	int i=0;
	for(i=0;i<NLOCKS;i++)
	{
		lockarr[i].lstate=LFREE;
		(lptr=&lockarr[i])->lstate=LFREE;
		lptr->ltail=1+(lptr->lhead=newqueue());
		lptr->r_count=0;
		lptr->max_count_reader=0;
		lptr->lprio=0;
	}	
	nextlock=NLOCKS-1;

}
