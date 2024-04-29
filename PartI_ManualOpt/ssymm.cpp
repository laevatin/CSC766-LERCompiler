#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define NMAX  100

double a[NMAX][NMAX], b[NMAX][NMAX], c[NMAX][NMAX], temp[NMAX][NMAX];

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

  for (i = 0; i < NMAX; i++) {
    for (j = 0; j < NMAX; j++) {
      c[i][j] = i+j;
      temp[i][j] = i/(j+1);
      a[i][j] =  i*j*0.5 / NMAX;
      b[i][j] =  i*j*j/ NMAX;
    }
  }

  int internumber = 10000/NMAX;
  if(internumber < 1)
    internumber = 1;
//orignial
IF_TIME(t_start = rtclock());

#pragma scop
for(int iter = 0; iter < internumber; iter++){
  for (i=0; i<NMAX; i++) {
    for (j=0; j<NMAX; j++) {
      for (k=0; k<j-1; k++) {
        c[i][k] += a[j][k] * b[i][j];
        c[i][j] += a[j][j] * b[i][j];
      }
      c[i][j] += a[j][j] * b[i][j];
    }
  }
}
#pragma endscop

  IF_TIME(t_end = rtclock());
  IF_TIME(fprintf(stdout, "%0.6lfs\n", t_end - t_start));

  double rst_org = c[NMAX-1][NMAX-1];
  
  printf("result %f\n", rst_org);
return 0;
}
