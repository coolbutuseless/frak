

#include <R.h>
#include <Rinternals.h>

extern SEXP julia_();

static const R_CallMethodDef CEntries[] = {
  // name    pointer            Num args
  {"julia_" , (DL_FUNC) &julia_ , 7},

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



