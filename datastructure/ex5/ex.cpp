#include<bits/stdc++.h>
#include<cstdlib>
#include<cstdio>
#include<iomanip>
#include<cmath>
#include<cassert>
#include<omp.h>
#include "gettime.h"

using namespace std;


static const double eps = 1.0;
static const double sigma = 0.025;
static const double sigma2 = sigma * sigma;
static const double rcut = 4.0 * sigma;
static const double rcut2 = rcut * rcut;


class Subbox{

 public: 

  int nsubbox;
  int np;
  int *np_sub;
  int *p;
  int *next_p;
  
  Subbox( const int _nsubbox, const int _np) : nsubbox(_nsubbox), np(_np){
    int nsubbox_total = nsubbox * nsubbox * nsubbox;
    p = new int[nsubbox_total];
    np_sub = new int[nsubbox_total];
    next_p = new int[np];
    for( int i=0; i<nsubbox_total; i++){
      p[i] = -1;
      np_sub[i] = 0;
    }
  }

  ~Subbox(){
    delete [] p;
    delete [] np_sub;
    delete [] next_p;
  }

  int getSubboxIndex( const int ix, const int iy, const int iz){
    return iz + nsubbox*( iy + nsubbox*ix);
  }
  
  void setup( const double (*x)[3]){
    for( int i=0; i<np; i++ ){
      next_p[i] = getSubboxIndex((int)(x[i][0] * nsubbox),
          (int)(x[i][1] * nsubbox),
          (int)(x[i][2] * nsubbox));
    }
  }

  int getParticle( const double (*xin)[3], int *pid,
       const int ix, const int iy, const int iz){
    int count = 0;
    for( int i=0; i<np; i++){
      if(next_p[i] == getSubboxIndex(ix,iy,iz)){
        pid[count] = i;
        count++;
      }
    }
    return count;
  }
  
  int getParticle2( const double (*xin)[3], int *pid,
        const int ix, const int iy, const int iz){
    int count = 0;
    int dx[] = {-1,0,1};
    int dy[] = {-1,0,1};
    int dz[] = {-1,0,1};
    for( int i=0; i<np; i++){
      for( int j=0; j<27; j++){
        if(next_p[i] == getSubboxIndex(ix+dx[j/9],iy+dy[(j/3)%3],iz+dz[j%3])){
          pid[count] = i;
          count++;
          break;
        }
      }
    }
    return count;
  }

};





void init1( double (*x)[3], const int n){
  for( int i=0; i<n; i++){
    x[i][0] = drand48();
    x[i][1] = drand48();
    x[i][2] = drand48();
  }
}

void init2( double (*x)[3], const int n){
  for( int i=0; i<n; i++){
    if(i<n/4){
      x[i][0] = 0.4 + 0.2*drand48();
      x[i][1] = 0.4 + 0.2*drand48();
      x[i][2] = 0.4 + 0.2*drand48();
    }else{
      x[i][0] = drand48();
      x[i][1] = drand48();
      x[i][2] = drand48();
    }
  }
}

void init3( double (*x)[3], const int n){
  for( int i=0; i<n; i++){
    if(i<n/2){
      x[i][0] = 0.4 + 0.2*drand48();
      x[i][1] = 0.4 + 0.2*drand48();
      x[i][2] = 0.4 + 0.2*drand48();
    }else{
      x[i][0] = drand48();
      x[i][1] = drand48();
      x[i][2] = drand48();
    }
  }
}

void lj( double (*x)[3], double *p, 
   const int *pid_i, const int ni,
   const int *pid_j, const int nj){

  for(int i=0; i<ni; i++){
    for(int j=0; j<nj; j++){
      if(pid_i[i] == pid_j[j])continue;
      double r2 = 0;
      for(int k=0; k<3; k++) r2 += pow(x[pid_i[i]][k] - x[pid_j[j]][k],2.0);
      if(r2>=rcut2)continue;
      double rinv2 = sigma2/r2;
      double rinv6 = rinv2*rinv2*rinv2;
      double rinv12 = rinv6*rinv6;
      double u = 4.0*eps*(rinv12 - rinv6);
      p[pid_i[i]] += u;
    }
  }
}

int main( int argc, char **argv){

  int n;
  int nsubbox;
  int nthreads;
  int initmode;
  int schmode;
  int randseed;
  int chunksize;
  //cerr << "[n nsubbox nthreads initmode[0,1] schmode[0,1] chunksize randseed[1-10]]" << endl;
  cin >> n >> nsubbox >> nthreads >> initmode >> schmode >> chunksize >> randseed;
// cerr << n << "\t" << nsubbox << "\t"
//   << nthreads << "\t" << initmode << "\t"
//   << schmode << "\t" << chunksize << "\t"
//   << randseed << "\t"  << endl;

  double (*x)[3] = new double[n][3];  // 粒子の位置
  double *p = new double[n];  // 粒子のポテンシャル
  int pid_i[nthreads][n]; //格子内のindex
  int pid_j[nthreads][n]; //周辺格子内のindex

  omp_set_num_threads(nthreads);

  srand48(randseed);
  if(initmode==0) init1( x, n);
  else if(initmode==1)init2( x, n);
  else init3( x, n);

  Subbox *subbox = new Subbox( nsubbox, n);
  subbox->setup(x);

  double nowtime = 0.0;
  getTime( &nowtime);
  int nsubbox2 = nsubbox*nsubbox;
  int nsubbox3 = nsubbox2*nsubbox;
  if(schmode==0){
    #pragma omp parallel for schedule( static, chunksize )
    for( int id=0; id<nsubbox3; id++){
      int i = id/nsubbox2, j = (id/nsubbox)%nsubbox, k = id%nsubbox;
      int t_num = omp_get_thread_num();
      int ni = subbox->getParticle( x, pid_i[t_num], i, j, k);
      int nj = subbox->getParticle2( x, pid_j[t_num], i, j, k);
      //cerr << ni << "\t" << nj << endl;
      lj( x, p, pid_i[t_num], ni, pid_j[t_num], nj);
    }
  }else{
    #pragma omp parallel for schedule( dynamic, chunksize)
    for( int id=0; id<nsubbox3; id++){
      int i = id/nsubbox2, j = (id/nsubbox)%nsubbox, k = id%nsubbox;
      int t_num = omp_get_thread_num();
      int ni = subbox->getParticle( x, pid_i[t_num], i, j, k);
      int nj = subbox->getParticle2( x, pid_j[t_num], i, j, k);
      //cerr << ni << "\t" << nj << endl;
      lj( x, p, pid_i[t_num], ni, pid_j[t_num], nj);
    }
  }
  double t = getTime( &nowtime);
  cout << t << endl;
  cerr << setprecision(10);
  cerr << setiosflags(ios::scientific);
  for( int i=0; i<n; i++){
    cerr << x[i][0] << "\t" 
   << x[i][1] << "\t" 
   << x[i][2] << "\t" 
   << p[i] << endl;
  }

  delete [] x;
  delete [] p;
  delete subbox;

}
