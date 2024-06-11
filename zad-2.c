#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	const int MAX_POINTS = 20;

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	// Find out rank, size
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int points = 0;
	int partner_rank = (world_rank + 1) % 2;

	// Pętla komunikacji aż do liczby określonej w treści zadania
	while (points < MAX_POINTS)
	{
		if (world_rank == points % 2)
		{
			// Inkrementacja liczby punktów przed wysłaniem
			points++;
			// Wysłanie liczby punktów
			MPI_Send(&points, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
			// Wyświetlenie komunikatu
			printf("Gracz_%d inkremetnuje liczbę punktów do %d i wysyła ją do Gracz_%d\n",
						 world_rank, points, partner_rank);
		}
		else
		{
			// Odebranie liczby puntów
			MPI_Recv(&points, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
							 MPI_STATUS_IGNORE);
			// Wyświetlenie komunikatu
			printf("Gracz_%d otrzymuje punkty %d od Gracz_%d\n",
						 world_rank, points, partner_rank);
		}
	}
	MPI_Finalize();
}