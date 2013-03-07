nclude <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>
#include <lock.h>

#define DEFAULT_LOCK_PRIO 20

/*--------------------------------Test 1--------------------------------*/
 void reader (char *msg, int lck)
{
        int     ret;

       kprintf ("  %s: to acquire lock\n", msg);
       lock (lck, READ, DEFAULT_LOCK_PRIO);
       kprintf ("  %s: acquired lock\n", msg);
       kprintf ("  %s: to release lock\n", msg);
       releaseall (1, lck);
}

void writer (char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}
void test1 ()
{
        int     lck;
        int     rd1;
        int     wr1;
	 int     wr2;
	 int	lck1;

        kprintf("\nTest 1: test the basic priority inheritence\n");
        lck  = lcreate ();
        lck1  = lcreate ();
      
        rd1 = create(reader, 2000, 55, "reader", 2, "reader B", lck);
        wr1 = create(writer, 2000, 40, "writer", 1, "writerA", lck); 
        wr2 = create(writer, 2000, 50, "writer", 2, "writerB", lck1);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 40)\n");
        resume(wr1);
        sleep (1);
 
        kprintf("-start reader, then sleep 1s. lock granted to reader (prio 55)\n");
        resume(rd1);
        sleep (1);
	
        kprintf("-start writer, then sleep 1s. writer B(prio 55)\n");
        resume (wr2);
        sleep (1);
	
        sleep (40);
        kprintf ("Test 3 OK\n");
}

int main( )
{
	int i, s;
	int count = 0;
	char buf[8];


	kprintf("Please Input:\n");
	while ((i = read(CONSOLE, buf, sizeof(buf))) <1);
	buf[i] = 0;
	s = atoi(buf);
	switch (s)
	{
	case 1:
		test1();
		break;
	
	}
}




