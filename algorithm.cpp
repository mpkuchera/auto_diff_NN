/* algorithm.cpp - A simple demonstration algorithm used in Tests 1 & 2 

  Copyright (C) 2012-2014 The University of Reading

  Copying and distribution of this file, with or without modification,
  are permitted in any medium without royalty provided the copyright
  notice and this notice are preserved.  This file is offered as-is,
  without any warranty.

*/


#include <cmath>
#include <random>
#include "BNN.h"
#include "algorithm.h"

#include "define_type.h"
using adept::adouble;

// A simple demonstration algorithm used in the Adept paper. Note that
// this algorithm can be compiled with
// -DADEPT_NO_AUTOMATIC_DIFFERENTIATION to create a version that takes
// double arguments and returns a double result.
adouble algorithm(const adouble x[2]) {
  adouble y = 4.0;
  adouble s = 2.0*x[0] + 3.0*x[1]*x[1];
  double b=3.0;
  y = s + b;
  y *= sin(s);
  return y;
}
 
adouble neural_net(const adouble q[]) { 
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(1, 2);
  int N = 1;
  int I = 2;
  int H = 4;

  int np = 17;
  adouble w[N] = {1.0};

  adouble x[N*2];
  adouble t[N];
  for(int i=0;i<N;i++){
    for(int j=0;j<I;j++){
    x[i*I+j] = dis(gen);
    }
    t[i] = sin(x[i*I+0]) *cos(x[i*I+1]);
    std::cout << "t[" << i << "] = " << t[i] << "\n";
  }

  adouble d = 0.0;
  for(int th=0; th<N;th++){
    adouble f = q[0];
    for(int j=0;j<H;j++){
      adouble inSum = 0.0;
      for(int i=0;i<I;i++){
	inSum += q[2*H+1+I*j+i]*x[th*I+i];
      }
      // f+=q[j+1]*(exp(2*(q[H+1+j]+inSum))-1)/(exp(2*(q[H+1+j]+inSum))+1);
      f+=q[j+1]*tanh(q[H+1+j]+inSum);
    }
    adouble res = t[th]-f;
    std::cout << res  << "\n";
    d += w[th]*(t[th]-f)*(t[th]-f);
  }
  std::cout << d  << "\n";
  return d;
}

HMC_type BNN::U(std::vector<HMC_type> &q) {
  int np = getNP();
  std::cout << "np = " << np << std::endl;

  //std::cout << "adept_type = " << typeid(adept_type).name() << std::endl; 
  //std::cout << "float = " << typeid(float).name() << std::endl; 
  //std::cout << "double = " << typeid(double).name() << std::endl; 

  stack.new_recording();

  std::vector<adept_type> a_q(np);
  //std::cout << "a_q allocated\n";
  for(int i=0;i<np;i++) {a_q[i] = q[i];}

  std::cout << "a_q filled,\n";
  return 1.0;
}
//adouble HMC_base::U(const adouble q[]) { 


adouble BNN::U(std::vector<adouble> &q) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(1, 2);
  //int N = 1;
  // int I = 2;
  //int H = 4;

  //int np = 17;
  adouble w[N] = {1.0};

  adouble x[N*2];
  adouble t[N];
  for(int i=0;i<N;i++){
    for(int j=0;j<I;j++){
    x[i*I+j] = dis(gen);
    }
    t[i] = sin(x[i*I+0]) *cos(x[i*I+1]);
    std::cout << "t[" << i << "] = " << t[i] << "\n";
  }

  adouble d = 0.0;
  for(int th=0; th<N;th++){
    adouble f = q[0];
    for(int j=0;j<H;j++){
      adouble inSum = 0.0;
      for(int i=0;i<I;i++){
	inSum += q[2*H+1+I*j+i]*x[th*I+i];
      }
      // f+=q[j+1]*(exp(2*(q[H+1+j]+inSum))-1)/(exp(2*(q[H+1+j]+inSum))+1);
      f+=q[j+1]*tanh(q[H+1+j]+inSum);
    }
    adouble res = t[th]-f;
    std::cout << res  << "\n";
    d += w[th]*(t[th]-f)*(t[th]-f);
  }
  std::cout << d  << "\n";
  return d;
}

 std::vector<HMC_type> BNN::delU(std::vector<HMC_type> &q) {

 int np = getNP();
 std::vector<HMC_type> a_q(np,0.0);

 return a_q;
}
