/*
 * =====================================================================================
 *
 *       Filename:  example5.cpp
 *
 *    Description:  the fifth examples from Xipeng Shen
 *
 *        Version:  1.0
 *        Created:  04/21/2015 09:46:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yufei Ding, yding8@ncsu.edu
 *   Organization:  North Carolina State University
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>

typedef long LARGE_INTEGER;

#define D 1000
#define M 1000
#define r0 1
#define NOiteration 1000
double x[M][D];

#define TIME

#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

//array initialization
void init_array()
{
  int i, j;

  for (i=0; i<M; i++) {
    for (j=0; j<D; j++) {
      x[i][j] = i + j;
    }
  }
}


//define timer for performance measurement
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



//example5
double example5(){

  double w[D]; 
  double d[D]; 

  double r = r0;
  for(int j = 0; j < D; j++){
    w[j] = 0;
  }

  for(int k = 0; k < NOiteration; k++){
    //prepare for reduction on array d[j]
    for(int j = 0; j < D; j++){
      d[j] = 0;
    }
    for(int i = 0; i < M; i++){
      double s = 0;
      for(int j = 0; j < D; j++){
        s = s + x[i][j]*w[j];
      }
      for(int j = 0; j < D; j++){
        d[j] = d[j] +s*x[i][j]; 
      }
    }
    for(int j = 0; j < D; j++){
      w[j] = w[j] + r*d[j];
    }
  }
  return w[D-1];
}


int main()
{
  int i, j, k;

  init_array();

  IF_TIME(t_start = rtclock());
  double rst_org = example5();
  IF_TIME(t_end = rtclock());
  IF_TIME(printf("time %0.6lfs\n", t_end - t_start));

  printf("result: %f\n", rst_org);
  return 0;
}

