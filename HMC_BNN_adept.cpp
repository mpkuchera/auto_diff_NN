#include <iostream>
#include <math.h>
#include <random>
#include "adept.h"
#include "BNN.h"
// Provide function prototype for "algorithm"; see algorithm.cpp for
// the contents of the function
#include "algorithm.h"

int
main(int argc, char** argv)
{
  int N = 300;

  int L = 20;
  int nOut = 50;
  int nRep = 10;
  int nBurn = 30;
  int H = 4; /// number of hidden nodes
  int in = 2; /// number of inputs

  int np =  1+H*(2+in);
  std::vector<HMC_type> data(N*in,1.0);
  std::vector<HMC_type> weights(np,1.0);
  std::vector<HMC_type> targets(N,1.0);

std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  for(int i=0;i<N;i++){
    for(int j=0;j<in;j++){
      data[i*in+j] = distribution(generator);
    } 
    targets[i] = sin(data[i*in])*cos(data[i*in+1]); 
  }
  BNN HMC_BNN(L, nOut, nRep, nBurn, H, in, data, weights, targets, "test.out");

  HMC_BNN.run();
  return 0;
}
