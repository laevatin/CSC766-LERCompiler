/*
 * =====================================================================================
 *
 *       Filename:  example1.cpp
 *
 *    Description:  the first example for 6 examples from Xipeng Shen
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

#define m 100
#define n 100
double x[m][n];
double y[n][m];
double s[m][n];
double r[m][n];

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

  for (i=0; i<m; i++) {
    for (j=0; j<n; j++) {
        x[i][j] = i + j;
        y[j][i] = i - j;
        s[i][j] = i * j;
        r[i][j] = 0;

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


//example1
double example1(){
  for (int i=0; i<m; i++){
    for (int j=0; j < m; j++){
      for (int k=0; k < n; k++){
        for (int l=0; l < n; l++){
          r[i][k] +=  x[i][l] * y[l][j] * s[j][k];
        }
      }
    }
  }
  return r[m-1][n-1];
}

int main()
{
  init_array();

  IF_TIME(t_start = rtclock());
  double rst_org = example1();
  IF_TIME(t_end = rtclock());
  IF_TIME(printf("time %0.6lfs\n", t_end - t_start));

  printf("result=%f\n", rst_org);
  return 0;
}

