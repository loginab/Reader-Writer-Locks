#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
//#include <sem.h>
#include <stdio.h>
#include <lock.h>
int newlock();

/*------------------------------------------------------------------------
 * lcreate  --  create and initialize a lock, returning its id
 *------------------------------------------------------------------------
 */
void lcreate()
{

        int     lk;


        if ( (lk=newlock())==SYSERR ) {

                return(SYSERR);
        }
        //lock[sem].semcnt = count;
        /* sqhead and sqtail were initialized at system startup */

        return(lk);
}

/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
 int newlock()
{
        int     lk;
        int     i;

        for (i=0 ; i<NLOCKS ; i++) {
                lk=nextlock--;
                if (nextlock < 0)
                        nextlock = NLOCKS-1;
                if (lockarr[lk].lstate==LFREE || lockarr[lk].lstate==DELETED) {
                        lockarr[lk].lstate = LUSED;
                        return(lk);
                }
        }
        return(SYSERR);
}             
