

#include <R.h>
#include <Rinternals.h>

extern SEXP julia_(SEXP cx_, SEXP cy_, 
                   SEXP movex_, SEXP movey_, 
                   SEXP zoom_, SEXP size_, 
                   SEXP max_iter_, SEXP result_,
                   SEXP colors_);

static const R_CallMethodDef CEntries[] = {
  // name    pointer            Num args
  {"julia_" , (DL_FUNC) &julia_ , 9},

  {NULL , NULL, 0}
};


void R_init_frak(DllInfo *info) {
  R_registerRoutines(
    info,      // DllInfo
    NULL,      // .C
    CEntries,  // .Call
    NULL,      // Fortran
    NULL       // External
  );
  R_useDynamicSymbols(info, FALSE);
}



