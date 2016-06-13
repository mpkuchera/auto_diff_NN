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

HMC_type BNN::U(const std::vector<HMC_type> &q) {
  adept::Stack stack;
  int np = getNP();
  //std::cout << "np = " << np << std::endl;

  //std::cout << "adept_type = " << typeid(adept_type).name() << std::endl; 
  //std::cout << "float = " << typeid(float).name() << std::endl; 
  //std::cout << "double = " << typeid(double).name() << std::endl; 

  stack.new_recording();
  adept_type a_q[np];
  //std::vector<adept_type> a_q(np);
  //std::cout << "a_q allocated\n";
  for(int i=0;i<np;i++) {a_q[i] = q[i];}
  //std::cout << "a_q filled,\n";
  return value(ad_U(a_q));
  //return 1.0;
}
//adept_type HMC_base::U(const adept_type q[]) { 


adept_type BNN::ad_U(const adept_type q[]) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(1, 2);
  //int N = 1;
  // int I = 2;
  //int H = 4;

  int np = getNP();
  adept_type ww[N] = {1.0};

  adept_type xx[N*2];
  adept_type tt[N];
  for(int i=0;i<N;i++){
    for(int j=0;j<I;j++){
      //x[i*I+j] = dis(gen);
      xx[i*I+j] = v_x[i*I+j];
    }
    //tt[i] = sin(xx[i*I+0]) *cos(xx[i*I+1]);
    tt[i] = v_t[i];
    //std::cout << "t[" << i << "] = " << t[i] << "\n";
  }

  adept_type d = 0.0;
  for(int th=0; th<N;th++){
    adept_type f = q[0];
    for(int j=0;j<H;j++){
      adept_type inSum = 0.0;
      for(int i=0;i<I;i++){
	inSum += q[2*H+1+I*j+i]*xx[th*I+i];
      }
      // f+=q[j+1]*(exp(2*(q[H+1+j]+inSum))-1)/(exp(2*(q[H+1+j]+inSum))+1);
      f+=q[j+1]*tanh(q[H+1+j]+inSum);
    }
    adept_type res = tt[th]-f;
    //std::cout << res  << "\n";
    d += ww[th]*(res)*(res);
  }
  
  adept_type prior = 0.0;
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

  //std::cout << "ad_U = " << d  << "\n";
  return d+prior;
}

 std::vector<HMC_type> BNN::delU(const std::vector<HMC_type> &q) {
   adept::Stack stack;
   //std::cout << "in delU(q)\n";
   int np = getNP();    
   HMC_type del_q[np] = {0};
   adept_type a_q[np] = {1.0};
   for(int i=0;i<np;i++){a_q[i] = q[i];}
   adept_type u;
   stack.new_recording();
   //adept::set_values(a_q,np,q);
   u = ad_U(a_q);

   //std::cout << "Stack status after algorithm run but adjoint not yet computed:\n"
   //	    << stack;
   //std::cout << "u = " << u.value() << "\n";
   u.set_gradient(1.0);
   stack.compute_adjoint();  
   //std::cout << "List of derivative statements:\n";
   //stack.print_statements();
   //std::cout << "\n";

   //std::cout << "Initial list of gradients:\n";
   //stack.print_gradients();
   //std::cout << "\n";
  stack.reverse();
  // Some more diagnostic information
  //std::cout << "Final list of gradients:\n";
  // stack.print_gradients();
  //std::cout << "\n";
  stack.independent(a_q,np);
  stack.dependent(u);
  HMC_type jac[np];
  stack.jacobian(jac);
  //HMC_type test = 0;
   //adept::get_gradients(a_q,np,del_q);
  // a_q[0].get_gradient(test);
   //for(int i=0;i<np;i++){a_q[i].get_gradient();}
   //del_q[0] = a_q[0].get_gradient();
   std::vector<HMC_type> dummy(np,0);
   for(int i=0;i<np;i++){dummy[i] = jac[i];
     //  std::cout << jac[i]<< std::endl;
   };
   return dummy;
 }
