/*
 * =====================================================================================
 *
 *       Filename:  example2.cpp
 *
 *    Description:  the second examples from Xipeng Shen
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

#define m 10000
#define NOiteration 10000
#define w0 1
double r = 1/m; 
double a[m];
double b[m];
double tmp1;
double tmp2;

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
    a[i] = 1/(i+1);
  }
  for (i=0; i<m; i++) {
    b[i] = 1/(i+1);
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


//example2
double example2(){
  double w = w0;
  int iteration = 0;
  double d;
  for (int i = 0; i < m; i++){
    tmp1 += a[i];
    tmp2 += b[i];
  }

  while(true){
    d = tmp1 + tmp2 * w;
    iteration++;
    if(iteration >= NOiteration) break;
    //if(d < 0.0001) break;
    w = w + r*d;
  }
  return w;
}

int main()
{
  int i, j, k;
  int runiter  =  10000/NOiteration;
  if(runiter < 1)
    runiter  = 1;
  init_array();
  
  IF_TIME(t_start = rtclock());

  double rst_org, rst_opt;
  
  for(int inter = 0; inter < runiter; inter++){
    rst_org = example2();
  }
  IF_TIME(t_end = rtclock());
  IF_TIME(printf("time %0.6lfs\n", t_end - t_start));

  printf("result: %f\n", rst_org);
  return 0;
}

