#ifndef INCLUDE_PROGRAM
#define INCLUDE_PROGRAM

#include "tnc.h"

// Constants.
// -----------------------------------------------------------------
const int defaulteta           = -1;
const double defaultstepmx     = -1;
const double defaultaccuracy   = -1;
const double defaultfmin       = -1;
const double defaultftol       = -1;
const double defaultxtol       = -1;
const double defaultpgtol      = -1;
const double defaultrescale    = -1;

// Function declarations.
// -----------------------------------------------------------------
// Truncated Newton (TN) implementation in C
extern "C" int tnc(int n, double x[], double *f, double g[],
                  tnc_function *function, void *state,
                  double low[], double up[], double scale[], double offset[],
                  int messages, int maxCGit, int maxnfeval, double eta, double stepmx,
                  double accuracy, double fmin, double ftol, double xtol, double pgtol,
                  double rescale, int *nfeval);

// Class Program.
// -----------------------------------------------------------------
// This class encapsulates execution of the TN routine for
// solving a nonlinear optimization problem with bound constraints.
//
// This is an abstract class since some of the class methods have not
// been implemented (they are "pure virtual" functions). In order to
// use this class, one needs to define a child class and provide
// definitions for the pure virtual methods.
class Program {
public:

  // The first input argument "n" is the number of variables, and "x"
  // must be set to the suggested starting point for the optimization
  // algorithm. See the TN documentation for more information on
  // the inputs to the constructor.
  Program (int n, double* x, double* lb, double* ub, double* scale,
	   double* offset, int* nfeval, int message, int maxCGit, int maxnfeval, 
           int eta = defaulteta, double stepmx = defaultstepmx, 
           double accuracy = defaultaccuracy, double fmin = defaultfmin,
	   double ftol = defaultftol, double xtol = defaultxtol,
           double pgtol = defaultpgtol, double rescale = defaultrescale );

  // This is the same constructor as above, except that the
  // appropriate amount of memory is dynamically allocated for the
  // variables, the bounds, and the bound types. The destructor also
  // makes sure that the memory is properly deallocated, but not so
  // for the other constructor.
  Program (int n, int message, int maxCGit, int maxnfeval, 
           int eta = defaulteta, double stepmx = defaultstepmx, 
           double accuracy = defaultaccuracy, double fmin = defaultfmin,
	   double ftol = defaultftol, double xtol = defaultxtol,
           double pgtol = defaultpgtol, double rescale = defaultrescale);

  // The destructor.
  virtual ~Program();

  // An implementation of this method should return the value of the
  // objective at the current value of the variables "x", where "n" is
  // the number of variables.
  virtual double computeObjective (double* x) = 0;

  // An implementation of this method should fill in the values of the
  // gradient "g" and the current point "x".
  virtual void computeGradient (double* x, double* g) = 0;

  // Run the solver. Upon completion, the solution is stored in "x".
  virtual int runSolver() = 0;

protected:

  // The copy constructor and copy assignment operator are kept
  // private so that they are not used.
  Program            (const Program& source) { };
  Program& operator= (const Program& source) { return *this; };

  int     n;       // The number of variables.
  double* x;       // The current point.
  double* lb;      // The lower bounds.
  double* ub;      // The upper bounds.
  double* scale;
  double* offset;
  int* nfeval;
  double  f;       // The value of the objective.
  double* g;       // The value of the gradient.
  void* state;
  int message;
  int maxCGit;
  int maxnfeval;
  int eta;
  double stepmx; 
  double accuracy;
  double fmin;
  double ftol; 
  double xtol;
  double pgtol;
  double rescale;

private:

  // Initialize some of the structures used by the TN routine.
  void initStructures();

  bool    owner;   // If true, then the blocks of memory pointed to by
		   // x, lb, ub and btype must be deallocated in the
		   // destructor.
  
};

#endif
