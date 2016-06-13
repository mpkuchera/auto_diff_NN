#include <cmath>
#include <cassert>
#include "HMC_base.h"
#include "define_type.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <random>

//#include "adept.h"
#include "algorithm.h"


//NOTE: can not inline this function
inline std::vector<HMC_type> HMC_base::it(std::vector<HMC_type> &q0) {
  //std::cout << "begin it()\n";
  std::vector<HMC_type> eps(NP);
  /*  std::vector<adept::adouble> qq(NP); 
  for(int i=0;i<NP;i++){
    qq[i]= q0[i];
    } */
  //eps = calc_step(q0); /*!< Uq0 calculated in this function */
  Uq0 = U(q0);
  //std::cout << "U called.\n";
   //std::cout << "eps = " <<  eps[0] << std::endl;
  
//!!!!!!!!!!!!!!!
   for(int i=0;i<NP;i++){ 
     eps[i] = 0.00002;
   }
   // std::default_random_engine generator;
   //std::normal_distribution<double> distribution;
  //!!!!!!!!!
  std::vector<HMC_type> dU(NP); 
  //std::vector<HMC_type> FDdU((int)q0.size()); 
  //std::cout << "q0 in HMC_base = " << q0[0] << std::endl;
  q = q0;
  //std::vector<HMC_type> p(q.size());
  //std::cout << "NP = " << NP << std::endl;

  
  assert((int)p.size() == NP);
  std::default_random_engine generator;
  std::normal_distribution<float> dist;
  for(int i=0;i<NP;i++) {
    p[i] = dist(generator);
  }

  std::vector<HMC_type> p0 = p;
  //std::cout << "before dU call.\n" ;
  // 1/2 step in momentum
  dU = delU(q);  //std::cout << "after dU call.\n" ;
 


  //std::cout << "in it() dU = ";
  /*
  for(int j=0;j<(int)dU.size();j++){
    std::cout << dU[j] << "\t";
  }
  std::cout << std::endl;*/
  // std::cout << "np = " << NP << std::endl;

  for(int i=0; i<NP;i++){
    p[i] = p[i] - 0.5*eps[i]*dU[i];
    // std::cout << "dU[" << i << "] = " << dU[i] << std::endl;
    if(p[i] != p[i]){
      std::cout << "p[" << i << "]" << "  diverging!" << std::endl;
      p[i] = 100000000;
      // assert(0);
    }
    //std::cout << "dU[i] = " << dU[i] << std::endl;
  }
  //  std::cout << "Begin leapfrog\n";
  for(int i=0;i<L;i++){
    //std::cout << "i = " << i << std::endl;
    // 1 step in position
    //std::cout << "p[0] = " << p[0] << std::endl; 
    for(int j=0;j<NP;j++) {
      q[j] = q[j] + eps[j]*p[j];
      // std::cout << "i = " << i << "  q[" << j << "] = " << q[j] << std::endl;
    }
    dU = delU(q); 
     for(int j=0;j<NP;j++) {
	if(dU[j] != dU[j]){
	  dU[j] = 1000000;
	  std::cout << "diverging dU[" << j << "] = " << dU[j] << std::endl;
	  //assert(0);
	}
     }
    // std::cout << "dU[0] = " << dU[0] << std::endl;
    // 1 step in momentum, except at the end
    if(i != (L-1)){
      for(int j=0;j<NP;j++) {
	p[j] = p[j] - eps[j]*dU[j];
      }
    }
  }
  // 1/2 step in momentum
  dU = delU(q);
  for(int i=0; i<NP;i++){	
    if(dU[i] != dU[i]){
      dU[i] = 1000000;
      std::cout << "dU[" << i << "] = " << dU[i] << std::endl;
      //assert(0);
    }
    p[i] = p[i] - 0.5*eps[i]*dU[i];
    if(p[i] != p[i]){
      assert(0);
    }
    //std::cout << "p[i] = " << p[i] << std::endl;
  }
 // to symmetrize momentum ?????
  for(int i=0; i<NP;i++){
    p[i] = -p[i];
    if(p[i] != p[i]){
      assert(0);
    }
  }

  // Calculate original and new U & K
  HMC_type Uc = U(q0);
  //std::cout << "Uc = " << Uc << std::endl;
  Uc = std::min(Uc,(HMC_type)3000000);
  HMC_type Un = U(q);

  HMC_type Kc = 0;
  for(int i=0;i<NP;i++){
    Kc += 0.5*p0[i]*p0[i];
  } 

  HMC_type Kn = 0;
  for(int i=0;i<NP;i++){
    Kn += 0.5*p[i]*p[i]; 
  } 
  // accept or reject?
  HMC_type f = exp(Uc-Un+Kc-Kn);
 
  //std::cout << "exp(Uc-Un+Kc-Kn) = " << f << std::endl;
   HMC_type rndm =  (HMC_type)rand()/(HMC_type)RAND_MAX;
 
   //std::cout << "rndm = " << rndm << std::endl;
   /*if(Uc + Kc > Un + Kn) {
    //std::cout << "exp(Uc-Un+Kc-Kn) = " << f << std::endl << "in it() q = ";
    //for(int j=0;j<(int)q0.size();j++){
    //std::cout << q[j] << "\t";
    //}
    //std::cout << std::endl;
    std::cout << "condition 1" << std::endl;
    return q; ///reject
    
  } else
   */ 
   if(rndm < f) {
     
     return q;   //accept
   } else {
     // std::cout << "condition 3" << std::endl;
     return q0; //reject
   }
};
int HMC_base::getNP() {
  return NP;
};
void HMC_base::run(){

  std::default_random_engine generator;
  std::normal_distribution<HMC_type> distribution;

  std::cout << "nBurn = " << nBurn << " nRep = " << nRep << " nOut = " << nOut << std::endl;
  int nIter = nBurn + nRep*nOut;

  for(int i=0;i<NP; i++){  
    q[i] = distribution(generator);
  }
  //std::cout << "q filled.\n";
  int accept = 0;
  float acc_ratio = 0.0;
  for(int i=0; i<nIter; i++){
    q0 = q;
    //std::cout << "q0 set.\n";
    q = it(q0);
    if(q != q0) {
      accept++; 
    }
    if(i%10 == 0){
      std::cout << "i = " << i << std::endl;
    }
    if(i%100==0){
      //      std::cout << "i = " << i << std::endl;
      acc_ratio = (float)accept/(float)(i+1);
      std::cout << "i = " << i << "\tacceptance ratio = " << acc_ratio << std::endl;
    }
    if(i>=nBurn && i%nRep==0){
      for(int j=0;j<NP;j++){
	of << q[j] << "\t";
      }
      of << std::endl;
    }
  }
};

