#include <iostream>
#include <math.h>
#include <random>
#include <fstream> /// for reading from file
#include <sstream>
#include <string>
#include "adept.h"
#include "BNN.h"
// Provide function prototype for "algorithm"; see algorithm.cpp for
// the contents of the function
#include "algorithm.h"

int
main(int argc, char** argv)
{

  std::ifstream infile("pMSSM_cross_sections_13TeV.txt");

  int N = 500;
  int in = 19;
  int out = 1;

  std::vector<HMC_type> data(N*in,1.0);
  //std::vector<HMC_type> weights(np,1.0);
  std::vector<HMC_type> target(N,1.0);

  std::string line;
  int n_inc = 0;
  std::getline(infile,line);
  line.clear();
  while(std::getline(infile,line))
    {
      if(n_inc >= N) {std::cout << n_inc << " input events read\n" ;break;}
      std::cout << line << std::endl;
      std::istringstream iss(line);
      iss >> target[n_inc];
      for(int i=0;i<in;i++){
	iss >> data[n_inc*in+i];
      }
      n_inc++;
    }
  std::vector<HMC_type> mean_data(in,0.0);
  HMC_type mean_target = 0.0;
  for(int j=0;j<N;j++){
    mean_target += target[j];
    //  std::cout << "target[" << j << "] = " << target[j] << std::endl;
    for(int i=0;i<in;i++){
      mean_data[i]+= data[j*in+i];
      //std::cout<< "data[" << j*in+i << "] = " << data[j*in+i] << std::endl;
    }
  }
  mean_target = mean_target/(HMC_type)N;  
  for(int i=0;i<in;i++){mean_data[i] = mean_data[i]/(HMC_type)N;}
  
  std::vector<HMC_type> dev_data(in,0.0);
  HMC_type dev_target = 0.0;

   for(int j=0;j<N;j++){
     dev_target += pow(target[j]-mean_target,2);
     //  std::cout << "target[" << j << "] = " << target[j] << std::endl;
     for(int i=0;i<in;i++){
       dev_data[i]+= pow(data[j*in+i]-dev_data[i],2);
       //std::cout<< "data[" << j*in+i << "] = " << data[j*in+i] << std::endl;
     }
   }
   dev_target = sqrt(dev_target/(HMC_type)N);
   for(int i=0;i<in;i++){dev_data[i] = sqrt(dev_data[i]/(HMC_type)N);}
 
 for(int j=0;j<N;j++){
   target[j] = (target[j]-mean_target)/dev_target;
     //  std::cout << "target[" << j << "] = " << target[j] << std::endl;
     for(int i=0;i<in;i++){
       data[j*in+i] = (data[j*in+i]-mean_data[i])/dev_data[i];
       //std::cout<< "data[" << j*in+i << "] = " << data[j*in+i] << std::endl;
     }
 }
   

   int L = 100;
  int nOut = 50;
  int nRep = 20;
  int nBurn = 100;
  int H = 22; /// number of hidden nodes
  //int in = 2; /// number of inputs 

  int np =  1+H*(2+in);
  std::vector<HMC_type> weights(N,1.0);
  BNN HMC_BNN(L, nOut, nRep, nBurn, H, in, data, weights, target, "pMSSM_test.out");

  HMC_BNN.run();
  return 0;
}
