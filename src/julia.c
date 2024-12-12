
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP julia_(SEXP cx_, SEXP cy_, SEXP movex_, SEXP movey_, SEXP zoom_, SEXP size_, 
            SEXP max_iter_, SEXP result_, SEXP colors_) {

  int nprotect = 0;
  
  int size     = Rf_asInteger(size_);
  double zoom  = Rf_asReal(zoom_);
  double cx    = Rf_asReal(cx_); // -0.7;
  double cy    = Rf_asReal(cy_); // 0.27015;
  double movex = Rf_asReal(movex_);
  double movey = Rf_asReal(movey_);
  int max_iter = Rf_asInteger(max_iter_);

  double idenom = 1.5/(0.5 * zoom * size);

  int max_iter_count = 1;

  int *iter_count = calloc((size_t)(size * size), sizeof(int));
  if (iter_count == NULL) {
    Rf_error("julia_(): Could not allocate memory for 'iters'");
  }
  int *iter_ptr = iter_count;

  for (int x=0; x < size; x++) {
    for (int y = 0; y < size; y++) {
      double zx = (x - size/2) * idenom + movex;
      double zy = (y - size/2) * idenom + movey;

      int niter = 0;
      while( (zx * zx + zy * zy < 4) & (niter < max_iter)) {
        double tmp = zx * zx - zy * zy + cx;
        zy = 2 * zx * zy + cy;
        zx = tmp;
        niter++;
      }

      // Keep track of the actual maximum iteration seen during frame
      if (niter > max_iter_count) {
        max_iter_count = niter;
      }

      *iter_ptr++ = niter;
    }
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create return structure
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP rarray = R_NilValue;
  if (TYPEOF(result_) == STRSXP && Rf_length(result_) == 1) {
    // 'int', 'dbl', 'raw'
    const char *result = CHAR(STRING_ELT(result_, 0));
    if (strcmp(result, "raw") == 0) {
      PROTECT(rarray = Rf_allocMatrix(RAWSXP, size, size)); nprotect++;
      uint8_t *raw_ptr = RAW(rarray);
      
      iter_ptr = iter_count;
      
      for (int i = 0; i < size * size; i++) {
        *raw_ptr++ = (uint8_t)round(*iter_ptr++/(double)max_iter_count * 255);
      }
    } else if (strcmp(result, "int") == 0) {
      PROTECT(rarray = Rf_allocMatrix(INTSXP, size, size)); nprotect++;
      memcpy(INTEGER(rarray), iter_count, size * size * sizeof(int));
    } else if (strcmp(result, "dbl") == 0) {
      PROTECT(rarray = Rf_allocMatrix(REALSXP, size, size)); nprotect++;
      double *ptr = REAL(rarray);
      
      iter_ptr = iter_count;
      
      for (int i = 0; i < size * size; i++) {
        *ptr++ = (double)(*iter_ptr++/(double)max_iter_count);
      }
    } else {
      Rf_error("'result' string type not yet handled: %s", result);
    }
  } else {
    Rf_error("'result' not handled");
  }



  free(iter_count);
  UNPROTECT(nprotect);
  return rarray;
}




// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// // Histogram equalization
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// int equalize = Rf_asLogical(equalize_);
// if (equalize) {
//   uint32_t *hist;
//   hist = (uint32_t *)calloc(256, sizeof(uint32_t));
//   double cumsum[256];
//   raw_ptr = RAW(rarray);
//   
//   // Count the pixels at each level
//   for (int i = 0; i < size * size; i++) {
//     hist[raw_ptr[i]]++;
//   }
//   
//   // Calculate the cumulative sum of pixels
//   cumsum[0] = hist[0];
//   for (int i = 1; i < 256; i++) {
//     cumsum[i] = cumsum[i-1] + hist[i];
//   }
//   
//   // Rescale the cumulative sum by the total pixels and scale by the
//   // max level i.e. 255
//   uint8_t remap[256];
//   for (int i = 0; i < 256; i++) {
//     remap[i] = (uint8_t)floor(cumsum[i] / (size * size) * 255);
//   }
//   
//   // Remap pixel values to the equalized bins in 'remap'
//   raw_ptr = RAW(rarray);
//   for (int i = 0; i < size * size; i++) {
//     *raw_ptr = remap[*raw_ptr];
//     raw_ptr++;
//   }
//   free(hist);
// }
// 
// 
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// // Apply gamma correction
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// double gamma = Rf_asReal(gamma_);
// if (gamma != 1) {
//   raw_ptr = RAW(rarray);
//   for (int i = 0; i < size * size; i++) {
//     *raw_ptr = (uint8_t)round(pow(*raw_ptr/255.0, gamma) * 255);
//     raw_ptr++;
//   }
// }



