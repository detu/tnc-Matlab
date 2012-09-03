#include "matlabprogram.h"
#include "array.h"

void* pt2Object;

// Function definitions.
// -----------------------------------------------------------------
// Copy the matrix elements to the destination array. It is assumed
// that sufficient memory has been allocated for the destination
// array. The copying preserves the column-major ordering of the
// matrix elements.
void copyElemsFromMatrix (const Matrix& source, double* dest) {
  Matrix destmatrix(dest,source.height(),source.width());
  destmatrix = source;
}

// Copy the elements from the source array into the matrix.
void copyElemsToMatrix (const double* source, Matrix& dest) {
  dest.inject(source);
}

// Copy the elements contained in all the matrices to the destination
// array. It is assumed that sufficient memory has been allocated for
// the destination array. The copying preserves the column-major
// ordering of the matrix elements.
void copyElemsFromArrayOfMatrices (const ArrayOfMatrices& source, 
				   double* dest) {

  // Repeat for each matrix.
  for (int i = 0; i < source.length(); i++) {
    copyElemsFromMatrix(*source[i],dest);
    dest += source[i]->length();
  }
}

// Copy the elements from the source array into the array of matrices.
void copyElemsToArrayOfMatrices (const double* source, 
				 ArrayOfMatrices& dest) {

  // Repeat for each matrix.
  for (int i = 0; i < dest.length(); i++) {
    copyElemsToMatrix(source,*dest[i]);
    source += dest[i]->length();
  }
}


// Function definitions for class MatlabProgram.
// -----------------------------------------------------------------
MatlabProgram::MatlabProgram (ArrayOfMatrices& variables, 
			      const ArrayOfMatrices& lowerbounds, 
			      const ArrayOfMatrices& upperbounds, 
			      const MatlabString* objFunc, 
			      const MatlabString* gradFunc,  
			      mxArray* auxData, int maxCGit, int maxnfeval, 
			      double ftol, double xtol) 
  : Program(variables.numelems(),TNC_MSG_ALL,maxCGit,maxnfeval),
    variables(variables) {
  x     = new double[n];
  lb    = new double[n];
  ub    = new double[n];

  this->objFunc  = objFunc;
  this->gradFunc = gradFunc;

  // Copy some pieces of information from the input arguments to the
  // respective program structures.
  copyElemsFromArrayOfMatrices(variables,x);  
  copyElemsFromArrayOfMatrices(lowerbounds,lb);
  copyElemsFromArrayOfMatrices(upperbounds,ub);

  // Create a Matlab array for the variables.
  int nv    = variables.length();
  varInputs = new mxArray*[nv];
  varMatlab = new ArrayOfMatrices(varInputs,variables);

  // Set up the inputs to the objective callback function.
  numInputsObjFunc = nv + (bool) auxData;
  inputsObjFunc    = new mxArray*[numInputsObjFunc];
  copymemory(varInputs,inputsObjFunc,nv);
  if (auxData)
    inputsObjFunc[nv] = auxData;

  // Set up the inputs to the gradient callback function.
  numInputsGradFunc = numInputsObjFunc;
  inputsGradFunc    = inputsObjFunc;

}

MatlabProgram::~MatlabProgram() {

  // Deallocate the Matlab arrays.
  int nv = variables.length();
  for (int i = 0; i < nv; i++)
    mxDestroyArray(varInputs[i]);
  delete   varMatlab;
  delete[] varInputs;

}

double MatlabProgram::computeObjective (double* x) {
  int      nlhs = 1;    // The number of outputs from Matlab.
  mxArray* plhs[nlhs];  // The outputs from the Matlab routine.

  // Copy the current value of the optimization variables.
  copyElemsToArrayOfMatrices(x,*varMatlab); 

  // Call the designated Matlab routine for evaluating the objective
  // function. It takes as input the values of the variables, and
  // returns a single output, the value of the objective function.
  if (mexCallMATLAB(nlhs,plhs,numInputsObjFunc,inputsObjFunc,*objFunc))
    throw MatlabException("Evaluation of objective function failed in \
call to MATLAB routine");

  // Get the result passed back from the Matlab routine.
  MatlabScalar matlabOutput(plhs[0]);
  double       objective = matlabOutput;
  
  // Free the dynamically allocated memory. 
  mxDestroyArray(plhs[0]);

  return objective;
}

void MatlabProgram::computeGradient (double* x, double* g) {
  int      nlhs = variables.length(); // The number of outputs from Matlab.
  mxArray* plhs[nlhs];                // The outputs from the Matlab routine.

  // Copy the current value of the optimization variables.
  copyElemsToArrayOfMatrices(x,*varMatlab); 

  // Call the designated Matlab routine for computing the gradient
  // of the objective function. It takes as input the values of the
  // variables, and returns as many outputs corresponding to the
  // partial derivatives of the objective function with respect to
  // the variables at their curent values.
  if (mexCallMATLAB(nlhs,plhs,numInputsGradFunc,inputsGradFunc,*gradFunc))
    throw MatlabException("Evaluation of objective gradient failed in \
call to MATLAB routine");

  // Get the result passed back from the Matlab routine.
  ArrayOfMatrices matlabOutput((const mxArray**) plhs,nlhs);
  for (int i = 0; i < variables.length(); i++)
    if (matlabOutput[i]->length() != variables[i]->length())
      throw MatlabException("Invalid gradient passed back from MATLAB \
routine");	
  copyElemsFromArrayOfMatrices(matlabOutput,g);
  
  // Free the dynamically allocated Matlab outputs.
  for (int i = 0; i < variables.length(); i++)
    mxDestroyArray(plhs[i]);
}

int MatlabProgram::function(double x[], double *f, double g[], void *state)
{
  *f = computeObjective(x);
  computeGradient(x,g);
 return 0;
}

int MatlabProgram::Wrapper_To_Call_function(double x[], double *f, double g[], void *state)
{
  //pt2Object = (void*) this;
  MatlabProgram* mySelf = (MatlabProgram*) pt2Object;
  mySelf->function(x, f, g, state);
  return 0;
}


int MatlabProgram::runSolver() {
  int status;  // The return value.
  
  pt2Object = (void*) this;
  
  // Initialize the objective function and gradient to zero.
  f = 0;
  for (int i = 0; i < n; i++)
      g[i] = 0;

  status = tnc(n, x, &f, g,MatlabProgram::Wrapper_To_Call_function , &state, lb, ub, scale, offset, message,maxCGit, maxnfeval, eta, stepmx, accuracy, fmin, ftol, xtol, pgtol,rescale, nfeval);  

  // Copy the solution to the class member "variables".
  copyElemsToArrayOfMatrices(x,variables);

  return status;
}

