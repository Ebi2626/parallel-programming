#include "mpi.h"   /* wlacza plik naglowkowy potrzebny dla biblioteki MPI */
#include <stdio.h> /* potrzebny dla funkcji 'fprintf()' */
#include <time.h>

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;

	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds)
		;
}

int main(int argc, char **argv)
{
	int myid, namelen;
	char computer_name[512]; /* nazwa komputera na ktorym
																 wykonuje sie proces */

	MPI_Init(&argc, &argv); /* inicjalizuje srodowisko MPI */

	/* pobiera identyfikator biezacego procesu */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	/* Pobiera nazwe komputera na ktorym proces sie wykonuje */
	MPI_Get_processor_name(computer_name, &namelen);
	printf("Start of script");

	for (int i = 0; i < 100; i++)
	{
		printf("Hello for %d from process %d on %s\n", i, myid, computer_name);
		delay(1 + i);
	}

	printf("End of script");

	/* konczy dzialanie srodowiska MPI */
	MPI_Finalize();

	return 0;
}