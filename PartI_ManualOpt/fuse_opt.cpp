#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>

#define N 10000
double a[2*N+1];
double b[N];

#define TIME
#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

void init_array()
{
  int i, j;

  for (i=0; i< 2 * N + 1; i++) {
    a[i] = i ;
  }
  for (i=0; i<N; i++) {
    b[i] = i;
  }
}


void print_array()
{
  int i, j;

  for (i=0; i<2*N+1; i++) {
    fprintf(stderr, "%lf ", a[i]);
    if (j%80 == 79) fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

double rtclock()
{
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}
double t_start, t_end;
double tmp1, tmp2;

int main()
{
  int i, j, k;

  init_array();

  int iter = 10000/N;
  if(iter < 1) 
    iter = 1;
//orignial
  IF_TIME(t_start = rtclock());

  #pragma scop

  tmp2 = (double)iter;
  tmp1 = 0.0;
  for (j=0; j<N; j++) {
    tmp1 = tmp1 + b[j];
  }

  for (i=0; i<N; i++) {
    a[2*i] = tmp1 * tmp2;
    a[2*i+1] = tmp1 * tmp2;
  }

  #pragma endscop

  IF_TIME(t_end = rtclock());
  IF_TIME(printf("time %0.6lfs\n", t_end - t_start));

  double rst_org = a[2*N];
  
#ifdef TEST
  print_array();
#endif
  printf("result: %f\n", rst_org);
  return 0;
}
