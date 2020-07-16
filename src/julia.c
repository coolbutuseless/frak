

#include <R.h>
#include <Rinternals.h>

SEXP julia_(SEXP cx_, SEXP cy_, SEXP movex_, SEXP movey_, SEXP zoom_, SEXP size_, SEXP max_iter_) {

  int size  = asInteger(size_);

  double zoom = asReal(zoom_);

  double cx = asReal(cx_); // -0.7;
  double cy = asReal(cy_); // 0.27015;

  double movex = asReal(movex_);
  double movey = asReal(movey_);

  int max_iter = asInteger(max_iter_);

  SEXP rarray;
  PROTECT(rarray = allocVector(RAWSXP, size * size));
  uint8_t *raw_ptr = RAW(rarray);

  double idenom = 1.5/(0.5 * zoom * size);

  int max = 1;

  for (int x=0; x < size; x++) {
    for (int y = 0; y < size; y++) {
      double zx = (x - size/2) * idenom + movex;
      double zy = (y - size/2) * idenom + movey;

      int iter = 0;
      while(zx * zx + zy * zy < 4 & iter < max_iter) {
        double tmp = zx * zx - zy * zy + cx;
        zy = 2 * zx * zy + cy;
        zx = tmp;
        iter++;
      }

      // Keep track of the actual maximum iteration seen during frame
      if (iter > max) {
        max = iter;
      }

      *raw_ptr++ = iter;
    }
  }

  // Normalise
  raw_ptr = RAW(rarray);
  for (int i = 0; i < size * size; i++) {
    *raw_ptr = (uint8_t)(*raw_ptr * 255/max);
    raw_ptr++;
  }



  UNPROTECT(1);
  return rarray;
}
