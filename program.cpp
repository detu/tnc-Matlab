#include "program.h"
#include <string.h>

// Function definitions.
// -----------------------------------------------------------------
// Copy a C-style string (a null-terminated character array) to a
// non-C-style string (a simple character array). The length of the
// destination character array is given by "ndest". If the source is
// shorter than the destination, the destination is padded with blank
// characters.
void copyCStrToCharArray (const char* source, char* dest, int ndest) {
  
  // Get the length of the source C string.
  int nsource = strlen(source);
  
  // Only perform the copy if the source can fit into the destination.
  if (nsource < ndest) {

    // Copy the string.
    strcpy(dest,source);

    // Fill in the rest of the string with blanks.
    for (int i = nsource; i < ndest; i++)
      dest[i] = ' ';
  }
}

// Return true if the two strings are the same. The second input
// argument must be a C-style string (a null-terminated character
// array), but the first input argument need not be one. We only
// compare the two strings up to the length of "cstr".
bool strIsEqualToCStr (const char* str, const char* cstr) {

  // Get the length of the C string.
  int n = strlen(cstr);

  return !strncmp(str,cstr,n);
}

// Function definitions for class Program.
// -----------------------------------------------------------------
Program::Program (int n, double* x, double* lb, double* ub, double* scale,
	          double* offset, int* nfeval, int message, int maxCGit, int maxnfeval, 
                  int eta, double stepmx, double accuracy, double fmin,
	          double ftol, double xtol,double pgtol, double rescale) {

  this->n         = n;
  this->x         = x;
  this->lb        = lb;
  this->ub        = ub;
  this->scale     = scale;
  this->offset    = offset;
  this->nfeval    = nfeval;
  this->message   = TNC_MSG_ALL;
  this->maxCGit   = maxCGit;
  this->maxnfeval = maxnfeval;
  this->eta       = eta;
  this->stepmx    = stepmx;
  this->accuracy  = accuracy;
  this->fmin      = fmin;
  this->ftol      = ftol;
  this->xtol      = xtol;
  this->pgtol     = pgtol;
  this->rescale   = rescale;
  this->owner     = false;

  initStructures();
}

Program::Program (int n, int message, int maxCGit, int maxnfeval, 
                  int eta, double stepmx, double accuracy, double fmin,
	          double ftol, double xtol, double pgtol, double rescale) {

  this->n         = n;
  this->x         = new double[n];
  this->lb        = new double[n];
  this->ub        = new double[n];
  this->scale     = NULL;
  this->offset    = NULL;
  this->message   = TNC_MSG_ALL;
  this->maxCGit   = maxCGit;
  this->maxnfeval = maxnfeval;
  this->eta       = eta;
  this->stepmx    = stepmx;
  this->accuracy  = accuracy;
  this->fmin      = fmin;
  this->ftol      = ftol;
  this->xtol      = xtol;
  this->pgtol     = pgtol;
  this->rescale   = rescale;
  this->nfeval    = new int;
  this->owner     = true;

  initStructures();
}

Program::~Program() { 
  if (owner) {
    delete[] x;
    delete[] lb;
    delete[] ub;
    delete[] scale;
    delete[] offset;
    delete[] nfeval;
  }
}

void Program::initStructures() {
  f   = 0;
  g   = new double[n];
}
