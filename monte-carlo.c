#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"

#define PI 3.14159265358979

MPI_Status status;

int s, rank, num_processes;

// Inicjalizacja liczby pętli i ziaren (100 000 iteracji i 100 ziaren)
int n = 100000;
int S = 100;

// Zmienne związane z pomiarem czasu
struct timeval begin, end;

void slave()
{

  char slave_name[256];
  int slave_name_len;
  MPI_Get_processor_name(slave_name, &slave_name_len);

  printf("Slave\tProcess #%d\t[%s]\n", rank, slave_name);

  int computations = 0;

  while (1)
  { // Nieskończona pętla

    int seed;

    // Pobranie seeda od mastera
    MPI_Recv(&seed, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    if (status.MPI_TAG == 0)
    { // Zabicie procesu podrzędnego gdy TAG == 0
      break;
    }
    else if (status.MPI_TAG == 200)
    { // Obliczenie liczby punktów w kole gdy TAG == 200

      // Obliczenie liczby punktów otaczających
      srand(seed);
      int count = 0, i = 0;

      for (i = 0; i < n; i++)
      {
        int x = rand() % (RAND_MAX / 100);
        int y = rand() % (RAND_MAX / 100);
        if (pow((double)x / (RAND_MAX / 100), 2) + pow((double)y / (RAND_MAX / 100), 2) <= 1)
          count++;
      }

      // Wysłanie wyników obliczeń do mastera
      // printf("N: %d, COUNT: %d\n", n, count);
      MPI_Send(&count, 1, MPI_INT, 0, 300, MPI_COMM_WORLD);

      computations++;
    }

  } // Koniec pętli while

  // Wysłanie sumy obliczeń do mastera
  MPI_Send(&computations, 1, MPI_INT, 0, 400, MPI_COMM_WORLD);

} // Koniec procesu podrzędnego

void master()
{ // Proces nadrzędny
  char master_name[256];
  int master_name_len;
  MPI_Get_processor_name(master_name, &master_name_len);

  printf("Master\tProcess #%d\t[%s]\n", rank, master_name);

  int count = 0, i = 0;
  int computations[num_processes];

  int min = num_processes - 1;
  if (s < num_processes)
    min = s;

  // Uruchomienie procesu podrzędnego
  for (i = 1; i <= min; i++)
  {
    int seed = rand();
    MPI_Send(&seed, 1, MPI_INT, i, 200, MPI_COMM_WORLD);
  }

  s -= min - 1;

  while (s > 0)
  {

    s--; // Obniżenie liczby pozostałych ziaren

    // Zebranie wyników ze wszystki procesów
    int value;
    MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, 300, MPI_COMM_WORLD, &status);

    count += value;

    int seed = rand() % (RAND_MAX / 10) + 100000000;

    // Przekazanie ziarna do procesów podrzędnych
    if (s > 0)
      MPI_Send(&seed, 1, MPI_INT, status.MPI_SOURCE, 200, MPI_COMM_WORLD);

  } // Koniec procesu nadrzędnego

  // Wysłanie do procesów podrzędnych sygnału do zamknięcia, gdy praca jest wykonana
  for (i = 1; i < num_processes; i++)
    MPI_Send(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

  // Pobranie wyników ze wszystkich procesów
  for (i = 1; i < num_processes; i++)
  {
    int value;
    MPI_Recv(&value, 1, MPI_INT, i, 400, MPI_COMM_WORLD, &status);
    computations[status.MPI_SOURCE] = value;
  }

  // Obliczenie PI i wyświetlenie wyników
  double pi = count / ((double)(S * n)) * 4;

  printf("N x S = %d\n", n * S);
  printf("PI Estimation: %f\n", pi);
  printf("Estimation Error: %f\n", fabs(PI - pi));

  // Wyświetlenie ziaren jakie procesy wykorzystały
  printf("Seeds Computed: ");

  for (i = 1; i < num_processes; i++)
    printf("[P%d] -> %d ", i, computations[i]);

  printf("\n");

} // Zakończenie procesu nadrzędnego

int main(int argc, char *argv[])
{

  setbuf(stdout, NULL);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

  // Zamknięcie programu gdy nie dysponujemy przynajmniej 2 procesorami
  if (num_processes < 2)
  {
    printf("You must utilize at least 2 processes for the workpool!\n");
    MPI_Finalize();
    return 0;
  }

  if (rank == 0)
  { // Proces nadrzędny

    // Odczytujemy argument programu - liczbę iteracji (i ziaren)
    if (argc == 2)
    {
      n = sqrt((double)atoi(argv[1]));
      S = n;
    }

    // Pobranie czasu po wprowadzniu danych o ilości procesów i ziaren
    // przez użytkownika
    gettimeofday(&begin, 0);
    s = S;
  }

  // Rozgłaszanie wartości n to wszystkich procesów
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0)
    slave();
  else
    master();

  MPI_Finalize();

  // Wyświetlenie informacji o czasie trwania programu (procesu nadrzędnego) przed zakończeniem
  if (rank == 0)
  {

    // Pobranie czasu zakończenia przebiegu programu
    gettimeofday(&end, 0);

    // Obliczenie czasu
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    // Wyświetlenie
    printf("Time measured: %.3f seconds.\n\n\n", elapsed);
  }
  return 0;

} // Koniec