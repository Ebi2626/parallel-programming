#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mpi.h"

#define PI 3.14159265358979

int main(int argc, char *argv[])
{

  struct timeval begin, end;

  int n = 1000000;
  int i = 0, count = 0;
  double x, y, pi, elapsed;

  if (argc == 2)
  {
    n = atoi(argv[1]);
  }

  srand((unsigned int)time(NULL));

  gettimeofday(&begin, NULL);

  for (i = 0; i < n; i++)
  {
    if (pow((double)rand() / RAND_MAX, 2) + pow((double)rand() / RAND_MAX, 2) <= 1)
      count++;
  }

  gettimeofday(&end, NULL);

  long seconds = end.tv_sec - begin.tv_sec;
  long microseconds = end.tv_usec - begin.tv_usec;
  elapsed = seconds + microseconds * 1e-6;

  pi = (double)count / n * 4;
  printf("\nPI Estimate:\t\t%f", pi);
  printf("\nEstimate Error:\t\t%f", fabs(PI - pi));

  printf("\nTime to Estimate PI:\t%fs\n\n", elapsed);

  return 0;
}