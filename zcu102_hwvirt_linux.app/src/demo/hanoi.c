/*      This program provides the solution to the famous        */
/*      Hanoi problem written in ANSI C.                        */

#include <stdio.h>


/* This functions takes a tower of n disks and moves from peg   */
/* 'source' to peg 'destination'.  Peg 'temp' may be used       */
/* temporarily.                                                 */

void Hanoi(char source, char temp, char destination, int n)
{
	if (n > 0)
	{
		Hanoi (source, destination, temp, n-1);
		printf("Move disk %d from peg %c to peg %c.\n",
			   n, source, destination);
		Hanoi (temp, source, destination, n-1);
	}
}

int main(void)
{
	int n;

	printf("Please enter the number of disks: ");
	scanf("%d",&n);
	printf("\n");

	Hanoi('A','B','C',n);
	
	return 0;
}
