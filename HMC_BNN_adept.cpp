#include <iostream>

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
  int nRep = 5;
  int nBurn = 30;
  int H = 4; /// number of hidden nodes
  int in = 2; /// number of inputs

  int np =  1+H*(2+in);
  std::vector<HMC_type> data(N,1.0);
  std::vector<HMC_type> weights(np,1.0);
  std::vector<HMC_type> targets(N,1.0);
  BNN HMC_BNN(L, nOut, nRep, nBurn, H, in, data, weights, targets, "test.out");

  HMC_BNN.run();
  return 0;

  //using adept::adouble;

  //adept::Stack s;
  //adept::Stack s_net;
  //adouble x[2]; // Our independent variables
  //adouble y;    // Our dependent variable

  // Set the values of x
  //x[0] = 2.0;
  //x[1] = 3.0;

  /*
  const int np = 17;
  
  adouble q[np];
  for(int i=0;i<np;i++){
    q[i] = 0.2;
  } 

  s.new_recording();
  //s_net.new_recording();
  // Run the algorithm again
  //y = algorithm(x);
  adouble f = neural_net(q); 
  std::cout << "f = neural_net(q) = " << f.value() << std::endl;
  std::cout << "Stack status after algorithm run but adjoint not yet computed:\n"
	    << s;
  // Print memory information
  std::cout << "Memory usage: " << s.memory() << " bytes\n\n";

  */
}
