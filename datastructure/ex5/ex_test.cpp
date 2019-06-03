#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
#define REP(i,N) for(i=0;i<N;i++)
static const double eps =1.0;
static const double sigma = 0.025;
static const double sigma2 = sigma*sigma;
static const double rcut = 4.0*sigma;
static const double rcut2 = rcut*rcut;

inline double getTime( double *now_time){
  struct timespec tv;
  clock_gettime( CLOCK_REALTIME, &tv);
  double told = *now_time;
  *now_time = (tv.tv_sec + (double)tv.tv_nsec*1e-9);
  return *now_time - told;
}

void init(double (*x)[3], const int n){
  int i;
  REP(i,n){
    x[i][0] = drand48();
    x[i][1] = drand48();
    x[i][2] = drand48();
  }
}

void lj( double (*x)[3], double *p, const int n){
  #pragma omp parallel for
  for(int i=0; i<n; i++){
    for(int j=0;j<n;j++){
      if(i==j)continue;
      double r2 = 0.0;
      int k;
      REP(k,3) r2 += pow((x[i][k] - x[j][k]),2.0);
      if(r2 >= rcut2) continue;
      double r2inv = sigma2/r2;
      double r6inv = r2inv*r2inv*r2inv;
      double r12inv = r6inv*r6inv;
      double u = 4*eps*(r12inv - r6inv);
      p[i] += u;
    }
  }
}

void lj2( double (*x)[3], double *p, const int n){
  #pragma omp parallel
  for(int i=0; i<n; i++){
    #pragma omp for
    for(int j=0;j<n;j++){
      if(i==j)continue;
      double r2 = 0.0;
      int k;
      REP(k,3) r2 += pow((x[i][k] - x[j][k]),2.0);
      if(r2 >= rcut2) continue;
      double r2inv = sigma2/r2;
      double r6inv = r2inv*r2inv*r2inv;
      double r12inv = r6inv*r6inv;
      double u = 4*eps*(r12inv - r6inv);
      p[j] += u;
    }
  }
}

int main( int argc, char **argv){
  int n;
  int nthreads;
  int mode;
  cin >>n >> nthreads >> mode;
  //cerr << n << "\t" <<nthreads << "\t" << mode <<  endl;
  double (*x)[3] =new double[n][3];
  double *p = new double[n];
  omp_set_num_threads(nthreads);

  srand48(1);
  init(x,n);
  int i;
  REP(i,n){
    p[i] = 0.0;
  }
  double nowtime =0.0;
  getTime( &nowtime);
  if(mode==0) lj(x,p,n);
  else lj2(x,p,n);
  double t =getTime(&nowtime);
  //cerr << t << endl;
  cout << t << endl; 

  cerr << setprecision(10);
  cerr << setiosflags(ios:: scientific);

  REP(i,n){
    cerr << x[i][0] << "\t" << x[i][1] << "\t" << x[i][2] << "\t" <<  p[i] << endl;
  }

  delete [] x;
  delete [] p;
}
