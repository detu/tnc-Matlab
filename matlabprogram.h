#ifndef INCLUDE_MATLABPROGRAM
#define INCLUDE_MATLABPROGRAM

#include "mex.h"
#include "program.h"
#include "matlabscalar.h"
#include "matlabstring.h"
#include "arrayofmatrices.h"


// Class MatlabProgram.
// -----------------------------------------------------------------
// This is an implementation of the abstract class Program.
class MatlabProgram: public Program {
public:

  // On input, "variables" should contain the initial point for the
  // optimization routine. If no auxiliary data
  // is needed, it may also be set to 0.
  MatlabProgram (ArrayOfMatrices& variables, 
		 const ArrayOfMatrices& lowerbounds, 
		 const ArrayOfMatrices& upperbounds, 
		 const MatlabString* objFunc, const MatlabString* gradFunc, 
		 mxArray* auxData, int maxCGit, int maxnfeval, 
		 double ftol = defaultftol, double xtol = defaultxtol);

  // The destructor.
  virtual ~MatlabProgram();

  // These provide definitions for the pure virtual functions of the
  // abstract parent class.
  virtual double computeObjective (double* x);
  virtual void   computeGradient  (double* x, double* g);  
  
  // Run the solver. Upon completion, the solution is stored in
  // "variables".
  virtual int runSolver();

  int function(double x[], double *f, double g[], void *state);

  static int Wrapper_To_Call_function(double x[], double *f, double g[], void *state);

protected:
  ArrayOfMatrices&    variables;  // Storage for the initial value and 
                                  // solution.
  const MatlabString* objFunc;    // The objective callback function.
  const MatlabString* gradFunc;   // The gradient callback function.
  
  ArrayOfMatrices*    varMatlab;  // Inputs to the Matlab callback
  mxArray**           varInputs;  // functions representing the
				  // current values of the variables.

  int       numInputsObjFunc;  // The number of inputs passed to the
			       // objective callback function.
  int       numInputsGradFunc; // The number of inputs passed to the
			       // gradient callback function.
  mxArray** inputsObjFunc;     // The inputs passed to the objective 
                               // callback function.
  mxArray** inputsGradFunc;    // The inputs passed to the gradient
			       // callback function.
 
};

#endif
