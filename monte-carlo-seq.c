#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

#define PI 3.14159265358979

int main()
{

  struct timeval begin, end;

  int n = 0, i = 0, count = 0;
  double x, y, pi, elapsed;

  printf("Input the total number of points: ");
  scanf("%d", &n);

  gettimeofday(&begin, NULL);

  for (i = 0; i < n; i++)
  {
    srand((unsigned int)time(NULL));
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