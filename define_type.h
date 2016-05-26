#include "adept.h"

#ifdef HMC_DOUBLE
typedef double HMC_type;
typedef adept::adouble adept_type;
#else
typedef float HMC_type;
typedef adept::afloat adept_type;
#endif

