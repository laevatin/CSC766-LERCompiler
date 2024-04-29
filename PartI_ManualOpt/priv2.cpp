#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define N 1000

double a[11*N];
double b[N][N];

#define TIME 1

#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

double rtclock()
{
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}


int main()
{
  int i,j, k;
  double t_start, t_end;

  for (i = 0; i < 11 * N; i++) {
    a[i] = (double)i / 10;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      b[i][j] = i*j;
    }
  }
double dist, dist1;

//orignial
IF_TIME(t_start = rtclock());

//#pragma scop
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      dist = 0.0;
      dist1 = 0.0;
      for (k=0; k<N; k++) {
        dist += a[10*i+j]*a[10*i+j];
        dist1 += a[10*i+j]+a[10*i+j];
      }
      b[i][j] = dist;
      b[i][j] += dist1;
    }
  }
  //#pragma endscop
  
  IF_TIME(t_end = rtclock());
  IF_TIME(fprintf(stdout, "%0.6lfs\n", t_end - t_start));

  double rst_org = b[N-1][N-1];
  
  printf("result: %f\n", rst_org);

  return 0;
}
