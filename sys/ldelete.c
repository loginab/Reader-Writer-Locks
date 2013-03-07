
#include<kernel.h>
#include<q.h>
#include<proc.h>



#include<lock.h>
extern struct lentry lockarr[];



int ldelete(int ldesc)
{
	int next;
	int i;
	struct pentry *pptr;
	struct lentry *lptr;
	lptr=&lockarr[ldesc];
	lptr->lstate=LFREE;
	next=q[lptr->lhead].qnext;
	
	while (next<lptr->ltail)  //setting all the waiters of the lock in deleted state
	{	
		pptr=&proctab[next];
		pptr->deleted[ldesc]=1;
		ready(dequeue(next),RESCHNO);
		next=q[lptr->lhead].qnext;
	}
		
	lptr=&lockarr[ldesc];	//holders
	for(i=0;i<50;i++)		//setting all the holders of the lock to the deleted state
	{
		if(lptr->holder[i]==1)
		{
			(&proctab[i])->deleted[ldesc]=1;
			if ((&proctab[i])->pstate!=PRCURR)
				(&proctab[i])->pstate=DELETED;
		}

		
	}
	return OK;
}
