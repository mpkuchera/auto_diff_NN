/*!****************************************
  \file BNN.cu
 
  \author Michelle P. Kuchera
  \date created 25/2/2013 updated 7/10/2015
*******************************************/

#include "BNN.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <cassert>
//#include <thrust/host_vector.h>
//#include <thrust/device_vector.h>
#include <thrust/device_ptr.h>
#include <thrust/reduce.h> /// thrust parallel reduce
#include <thrust/device_free.h>

#include "adept.h"
using adept::adouble;
//using namespace fadbad;
/*!**
NN PARAMETERS
q[0] = b;
q[j+1] = v_j;
q[H+j+1] = a_j 
q[2H+1+i+j*I] = u_ji 
****/
//typedef adept::afloat HMC_type2;

//fadbad::B<HMC_type> U_ad(fadbad::B<std::vector<HMC_type> > &q, HMC_type* x, HMC_type* w, HMC_type* t, int H, int I, int N) {

BNN::BNN(int l, int nOut_, int nRep_, int nBurn_, int h, int inp, std::vector<HMC_type> &data, std::vector<HMC_type> &weights, std::vector<HMC_type> &targets, std::string s) 
  : HMC_base(l, 1+h*(2+inp), nOut_, nRep_, nBurn_),
    v_x(data),
    H(h),
    I(inp),
    N(targets.size()),
    sig(1),
    v_w(weights),
    v_t(targets),
    oFile(s),
    sigb(100),
    sigv(1.65),
    siga(0.86),
    sigu(0.54)
{
  std::cout << "N = " << N << std::endl;
  np = getNP();
  std::cout << "BNN_regression(L,nOut,nRep,nBurn,H,I,x,w,t,s)" << std::endl;
  std::cout << "v_x[0] = " << v_x[0] << "\t v_x[1] = " << v_x[1] << std::endl;
  std::cout << "v_t[0] = " << v_t[0] << std::endl;
  of.open(oFile.c_str(),std::ofstream::out);
  of << I << "\t" << H << std::endl;

  /** memory allocation on GPU **/
  /* need to allocate d,t,x,w,q  */
  size_np = np*sizeof(HMC_type);
  size_n = N*sizeof(HMC_type);  



  x = &v_x[0];
  t = &v_t[0];
  w = &v_w[0];
  //q = &v_q[0];
  //d = &v_d[0];
  

  
};

BNN::BNN(int l, int nOut_, int nRep_, int nBurn_, int h, int i, std::string s) 
  : HMC_base(l, 1+h*(2+i), nOut_, nRep_, nBurn_),
    v_x(std::vector<HMC_type>()),
    H(h),
    I(i),
    N(0),
    sig(1),
    v_w(std::vector<HMC_type>()),
    v_t(std::vector<HMC_type>()),
    oFile(s),
    sigb(100),
    sigv(1.65),
    siga(0.86),
    sigu(0.54)
{
  np = getNP();
  std::cout << "BNN_regression(l,h,i,s)" << std::endl;
  std::cout << "x[0] = " << x[0] << "\t x[1] = " << x[1] << std::endl;
  std::cout << "t[0] = " << t[0] << std::endl;
  of.open(oFile.c_str(),std::ofstream::out);

  of << I << "\t" << H << std::endl;

 /** memory allocation on GPU **/
  /* need to allocate d,t,x,w,q  */
  //size_t size_np = np*sizeof(HMC_type);
  //size_t size_n = N*sizeof(HMC_type);  
 
};
// void BNN::add(std::vector<HMC_type> &inputs, HMC_type target, HMC_type weight){
//   std::cout << "TO BE IMPLEMENTED" << std::endl;
// };

BNN::~BNN() {
  std::cout << "destructor of BNN_regression()" << std::endl;
  /*  
      cudaFree(dev_x);
      cudaFree(dev_t);
      cudaFree(dev_w);
      cudaFree(dev_q);
      cudaFree(dev_d);
  */
  //  thrust::device_free(d_dev_ptr);
};
/*
inline HMC_type BNN::U(std::vector<HMC_type> &in_q) {
  // if((int)q.size() != np){
  //std::cout << "ERROR: incorrect parameter numbers" << std::endl;
  //assert(0);
  // }
  /*! probability density for BNN regression is -logP */
// adept::Stack s;
// std::vector<adouble> q;
// HMC_type* qq = &in_q[0];
  // int threadsPerBlock = 256;
  // int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
  
  
  //diff<<<blocksPerGrid,threadsPerBlock>>>(dev_q, dev_x, dev_w, dev_t, dev_d, H, I, N); 
// HMC_type sum = 0.0;
  // HMC_type sum = thrust::reduce(d_dev_ptr,d_dev_ptr+N);
  //cudaCheckError("thrust::reduce");

  //return sum/(2*sig*sig) + LnPrior(in_q);

//};
//*/
/*! OPTIMIZE! CACHE!!!!! */ /*
inline std::vector<HMC_type> BNN::delU(std::vector<HMC_type> &q) {

  std::vector<HMC_type> dU(np);
  HMC_type* qq = &q[0];
  
  std::vector<HMC_type2>  qs(N);
  for(int i=0;i<N;i++){
    qs[i] = qq[i];
  }

  std::cout << qs[0] << std::endl;
  //std::cout << "IN BNNREG" << std::endl;
  //int threadsPerBlock = 256;
  //int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
  /*
  cudaMemcpy(dev_q, qq, size_np, cudaMemcpyHostToDevice);
  cudaCheckError("cudaMemcpy dev_q");
  // vector<HMC_type> sum(np); 
  for(int i=0;i<np;i++){
    del_diff<<<blocksPerGrid,threadsPerBlock>>>(dev_q, dev_x, dev_w, dev_t, dev_d, H, I, N,i);
    cudaCheckError("del_dif<<>>");
    dU[i] = thrust::reduce(d_dev_ptr,d_dev_ptr+N);
    dU[i] = dU[i] /(sig*sig);
    cudaCheckError("thrust::reduce");
  }
  */
  //HMC_type Uu = U_ad(qs, x, w, t, H, I, N); 
  //dU = 

//  return dU;
/* Finite Difference DelU. Not efficient enough */

			
  // std::vector<HMC_type> FDdU(np);// = new HMC_type[np];
  // std::vector<HMC_type> qm(np);
  // std::vector<HMC_type> qp(np);
  
  //  HMC_type h = 0.001;
  //  //std::vector<HMC_type> dU(np);
  //  for(int i=0; i<np;i++){
  //    qm = q;
  //    qp = q;
  //    qm[i] = qm[i] - 0.5*h;
  //    qp[i] = qp[i] + 0.5*h;
  //    //std::cout<< qm[i] << "\t";
  //    //std::cout<< qp[i] << "\t";
  //    FDdU[i] =  (U(qp) - U(qm))/h;
  //  }
  //  //std::cout << std::endl;
  
  //  // FDdU = FDdelU(q);
  // return FDdU;  //int np = getNP();


/*std::vector<HMC_type> dU(np);
  HMC_type sum = 0.0;
  for(int j=0;j<N; j++){
    HMC_type inSum = 0.0;
    for(int i=0;i
  }
  dU[0] = sum/(2*sig*sig);*/

//};*/



inline HMC_type BNN::LnPrior(std::vector<HMC_type> &q) {


  HMC_type prior = 0.0;
  prior += 0.5*q[0]*q[0]/(sigb*sigb);
  for (int v=1; v<H+1;v++){
    // std::cout << "v = " << v << std::endl;
    prior +=  0.5*q[v]*q[v]/(sigv*sigv);
  }
  for(int a=H+1;a<2*H+1;a++){
    prior +=  0.5*q[a]*q[a]/(siga*siga);
    //std::cout << "a = " << a << std::endl;
  }
  for(int u=2*H+1; u<H*(2+I)+1; u++){
    prior +=  0.5*q[u]*q[u]/(sigu*sigu);
    //std::cout << "u = " << u << std::endl;
  }
  return prior;
};

