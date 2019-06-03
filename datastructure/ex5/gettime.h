#include<time.h>

inline double getTime( double *now_time ){
  struct timespec tv;
  clock_gettime( CLOCK_REALTIME, &tv );

  double told = *now_time;
  *now_time = (tv.tv_sec + (double)tv.tv_nsec * 1e-9);
  return *now_time - told;
}
