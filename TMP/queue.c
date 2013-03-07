/* queue.c - dequeue, enqueue */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <proc.h>
#include <lock.h>


/*------------------------------------------------------------------------
 * enqueue  --	insert an item at the tail of a list
 *------------------------------------------------------------------------
 */
int enqueue(int item, int tail)
/*	int	item;			- item to enqueue on a list	*/
/*	int	tail;			- index in q of list tail	*/
{
	struct	qent	*tptr;		/* points to tail entry		*/
	struct	qent	*mptr;		/* points to item entry		*/

	tptr = &q[tail];
	mptr = &q[item];
	mptr->qnext = tail;
	mptr->qprev = tptr->qprev;
	q[tptr->qprev].qnext = item;
	tptr->qprev = item;
	return(item);
}


/*------------------------------------------------------------------------
 *  dequeue  --  remove an item from the head of a list and return it
 *------------------------------------------------------------------------
 */
int dequeue(int item)
{
	struct	qent	*mptr;		/* pointer to q entry for item	*/

	mptr = &q[item];
	q[mptr->qprev].qnext = mptr->qnext;
	q[mptr->qnext].qprev = mptr->qprev;
	return(item);
}
int changePriority(int ldes1,int priority)
{
	struct pentry *pptr;
	struct lentry *lptr;
	lptr=&lockarr[ldes1];
	

	int i,k ;
	int max=0;
	struct pentry *p;
	

					//for all holders of a semaphore changing pinh
	
	pptr=&proctab[currpid];
        for(i=0;i<50;i++)       
	{
		if (lptr->holder[i]==1)
		{
			

			p= &proctab[i];

			if (p->pprio <priority || p->pprio <pptr->pinh)
			{	

				
				if (p->pinh <priority || p->pinh<pptr->pinh)
				{

					p->pinh=priority>pptr->pinh?priority:pptr->pinh;
					if (p->pstate==PRREADY)				//for all the ready holders change the prio by deqeue and insert
					{

						insert(dequeue(i),rdyhead,p->pinh);
						
					}
					if(p->pstate==PRWAIT)
					{
							changePriority(p->wait_lock,p->pinh);
					}
					else if (p->pstate!=PRFREE)
					{
				
					}
				}
			}
			
		}
	}
	return OK;
}
						//inserting in the wait queue of the lockkeeping the writes first and then the reader of same priority
int insert_queue(int proc,int head,int key)
{
	struct pentry  *pptr;
	int next;
	int prev;
	next =q[head].qnext;

	while(q[next].qkey < key ||(q[next].qkey==key && (&proctab[next])->wait_type==WRITE))
	{	
		
		pptr=&proctab[next];
		next=q[next].qnext;

	}
	q[proc].qnext = next;
        q[proc].qprev = prev = q[next].qprev;
        q[proc].qkey  = key;
        q[prev].qnext = proc;
        q[next].qprev = proc;
        return(OK);
}


