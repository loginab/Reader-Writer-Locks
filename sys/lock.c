#include<kernel.h>
#include<q.h>
#include<stdio.h>
#include<proc.h>
#include <lock.h>
extern struct lentry lockarr[];
extern int newlock ;
int lock(int ldes1,int type,int priority)
{

	struct lentry *lptr;
	struct pentry *pptr;
	struct pentry *p;
	int next,tail;
	int max=-1;
	
	pptr=&proctab[currpid];
													//check for bad locks and descriptors
	if(isbadlock(ldes1) || (lptr = &lockarr[ldes1])->lstate==LFREE || pptr->pwaitret==DELETED )
	{
		pptr->pwaitret=OK;
		return (SYSERR);
	}

	if (lptr->lstate==READ)
	{	

		if (type==READ)
		{		
				//check for highest writer in the queuue
			next=q[lptr->lhead].qnext;
			tail=lptr->ltail;
							//get maximum reader
			while(next<tail)
			{
				p= &proctab[next];
				if (max < priority && p->wait_type==WRITE)
					max=q[next].qkey;
				next=q[next].qnext;
			}
				//if granted access add the process to the lock holder list;

			if (max >priority)			//check if higher prio writer present in the queue
			{
				pptr->pstate=PRWAIT;
				pptr->wait_lock=ldes1;
				insert_queue(currpid,lptr->lhead,priority);
				pptr->pwaitret=OK;
				pptr->wait_type=type;
				resched();
			  	if (pptr->deleted[ldes1]==1)
        			{

			                pptr->deleted[ldes1]==0;
                			pptr->pwaitret=OK;
					return (DELETED);
        			}	
				return pptr->pwaitret;
			}

			lptr->holder[currpid]=1;
			pptr->lockdesc[ldes1]=1;
			lptr->r_count++;
			return OK;
		}
		else
		{	
			
								//check for change priority and keep the process in the waiting queue.
			changePriority(ldes1,pptr->pprio);
			pptr->pstate=PRWAIT;
			pptr->wait_lock=ldes1;

			insert_queue(currpid,lptr->lhead,priority);
			pptr->pwaitret=OK;
			pptr->wait_type=type;
			resched();			//In case the lock has been deleted
			if (pptr->deleted[ldes1]==1)
                        {
				pptr->deleted[ldes1]==0;
				pptr->pwaitret=OK;
                                return (DELETED);
                        }	
			return pptr->pwaitret;
		}
	}
	else if (lptr->lstate==WRITE)
	{	
		

		changePriority(ldes1,pptr->pprio);

		pptr->pstate=PRWAIT;
		pptr->wait_lock=ldes1;

		insert_queue(currpid,lptr->lhead,priority);
		pptr->pwaitret=OK;
		pptr->wait_type=type;
		resched();			//In case the lock has been deleted
		if (pptr->deleted[ldes1]==1)
                {

                    pptr->deleted[ldes1]==0;
                    
		    pptr->pwaitret=OK;
			return (DELETED);

                 }	
		return pptr->pwaitret;
	}
	else
	{
			//first time in grant the lock
		lptr->lstate=type;
		lptr->holder[currpid]=1;
		lptr->lprio=(&proctab[currpid])->pprio;
		return(OK);
	}
	
	return OK;

}
