#include<q.h>
#include<stdio.h>
#include<kernel.h>
#include<lock.h>
#include<proc.h>
extern struct lentry lockarr[];
extern int nextlock;
					//function handeles the  deperived case of writers and pops the process accordingly
int pop_3(int head,int tail,int ldesc1)
{
	struct lentry    *tptr;
        struct pentry  *pptr;
	tptr=&lockarr[ldesc1];
	int max=head,pid,next;
	int count=1;
        int rd[50],i=1;
	next =q[head].qnext;
	int cc=1;	

	if (isEmpty(head))		//if one in the ready queue change the 
	{
		tptr->lstate=LUSED;
		return OK; 
		//continue ;
	}
	

	while(next <tail )		//calculate the max writer
        {	
        	pptr = &proctab[next];
                if (pptr->wait_type==WRITE)
			max=next;
		next=q[next].qnext;
	}

	if (max !=head)
	if ( (&proctab[max])->dep==3 )    //has been deprieved 3 times
	{
		pptr = &proctab[max];
		pptr->dep=0;	
		pptr->wait_lock=-1;
		pptr->wait_type=-1;
		ready(dequeue(max),RESCHNO);
		//ready(dequeue(max),RESCHYES);
		return OK;	
		//continue;
	}
	

	if (q[max].qnext ==tail && max !=head)      //writer is the last element
	{
		pid=getlast(tail);
		pptr = &proctab[pid];
		pptr->dep=0;	
		pptr->wait_lock=-1;
		pptr->wait_type=-1;

		ready(pid,RESCHNO);
		return OK; 
		//continue;
	}		

	// do this only if the last writer priority = priority of last reader
	next=q[tail].qprev;	
	

	if (max ==head)
	{
		while (next !=head)
		{
			pptr= &proctab[next];
			pptr->wait_lock=-1;
			pptr->wait_type=-1;
			ready(dequeue(next),RESCHNO);
			next=q[tail].qprev;
		}
	}
	else
	{
		while(next!=max)
		{
			pptr= &proctab[next];
			pptr->wait_lock=-1;
			pptr->wait_type=-1;
			ready(dequeue(next),RESCHNO); 
			/*dfsdfsfsdfsdfsdfSD*/
			next=q[tail].qprev;
		}
		
		(&proctab[max])->dep++;
	}
	return OK;
}
int releaseall ( numlocks, ldes1)					//handling of variable number of arguments
{
int i=1;



unsigned long   *a;             /* points to list of args       */

a = (unsigned long *)(&ldes1) + (numlocks-1); /* last argument      */
for ( ; numlocks > 0 ; numlocks--)    /* machine dependent; copy args */
        {       

		//kprintf("\n HEre 2");
		release(1,*a);
		a--; 
	}
//resched();

return OK;
}

int release(int numlocks,int ldes1)
{
	struct lentry *lptr;
	struct pentry *pptr;
	int i;	

	pptr=&proctab[currpid];
	

	
	if (pptr->deleted[ldes1]==1)			//check if the locks has been deleted
	{

		pptr->deleted[ldes1]==0;
		pptr->pwaitret=OK;
		//continue;
		return (SYSERR);
	}
							//if bad lock
	if(isbadlock(ldes1) || (lptr=&lockarr[ldes1])->lstate==LFREE ||pptr->pwaitret==DELETED)
	{	pptr->pwaitret=OK;
		return (SYSERR);
	}
	if (pptr->pinh != 0)
	{
		pptr->pinh=0;
		//chprio(currpid,pptr->prioh);
	}
	


	if (lptr->lstate==READ)
	{	
		lptr->holder[currpid]=-1;
		pptr->lockdesc[ldes1]=-1;
		if (lptr->r_count>0)				//if more readers present in the waiting queue
		{
			lptr->r_count--;
			if (lptr->r_count ==0 && !isEmpty(lptr->lhead))
			{

			}
			else
			return OK;				
			
		}
				
	}	
	else							//change the holder of the process and of the locks
	{
		lptr->holder[currpid]=-1;
		pptr->lockdesc[ldes1]=-1;

	}
	

	pptr->pinh=0;						//check for the inverted priority
	for(i=0;i<50;i++)
	{
		if (pptr->lockdesc[i]==1)
		{
			changePriority(i,getlast((&lockarr[i])->ltail));
		}
	}
	pop_3(lptr->lhead,lptr->ltail,ldes1);
	//continue;	
	return OK;
}

