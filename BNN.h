/*!****************************************
   \class BNN_regression
   \brief BNN functional approximation class
   \author Michelle E. Perry
   \date created 25/2/13 updated 3/6/2016
 
*******************************************/

#ifndef _BNN_H
#define _BNN_H
#include "adept.h"
#include "HMC_base.h"
#include <iostream>
#include "define_type.h"
//#include <thrust/device_ptr.h>

class BNN : public HMC_base {
 private:
  std::vector<HMC_type> v_x; //!< training data 
  int H; //!< number of hidden nodes
  int I; //!< number of data inputs 
  int N; //!< number of training events 
  HMC_type sig; //!< standard deviation for priors 
  std::vector<HMC_type> v_w; //!< training event weights 
  std::vector<HMC_type> v_t; //!< targets for training data
  //HMC_type* q;
  std::string oFile; //!< output file name 
  // __global__ void diff(HMC_type*); //!< neural network function 
  HMC_type LnPrior(std::vector<HMC_type>&); //!< LogPrior function 
  // numbers from Radford Neal code test run sinxcosy
  HMC_type sigb; //!< standard deviation for b parameters
  HMC_type sigv; //!< standard deviation for v parameters
  HMC_type siga; //!< standard deviation for a parameters
  HMC_type sigu; //!< standard deviation for u parameters


  HMC_type* x;
  HMC_type* t;
  HMC_type* w;
  HMC_type* q;
  HMC_type* d;
  
  HMC_type* dev_x;
  HMC_type* dev_t;
  HMC_type* dev_w;
  HMC_type* dev_q;
  HMC_type* dev_d;

 int np;
 
size_t size_np;
size_t size_n;
//thrust::device_ptr<HMC_type> d_dev_ptr;
// protected:
// adept::Stack stack;
 public:
  /*! constructor for storing data, targets, and (optional) weights using add() */
  BNN(int l, int nOut_, int nRep_, int nBurn_, int h, int i, std::string s);
  /*! constructor where data, targets, weights are fully stored by user prior to calling contructor*/
  BNN(int l, int nOut_, int nRep_, int nBurn_, int h, int i, std::vector<HMC_type> &data, std::vector<HMC_type> &weights, std::vector<HMC_type> &targets, std::string s); 
  ~BNN();
  /*! probability distribution for a regression neural network */
  virtual HMC_type U(const std::vector<HMC_type>&); 
  //virtual HMC_type U();
  /*! analytical derivative of the probability distribution for a regression neural network */
  virtual std::vector<HMC_type> delU(const std::vector<HMC_type>&);
  // virtual adept_type ad_U(std::vector<adept_type>&); 
  virtual adept_type ad_U(const adept_type q[]); 
 
//std::vector<HMC_typ adouble U(std::vector<adouble>&); e> FDdelU(std::vector<HMC_type>&);
  /*! adds to the input, target, and weight array while iterating over each training event  */
  // void add(std::vector<HMC_type>& input, HMC_type target, HMC_type weight=1);
};
#endif
