
#include <stdint.h>
#include <R.h>
#include <Rinternals.h>

SEXP julia_(SEXP cx_, SEXP cy_, SEXP movex_, SEXP movey_, SEXP zoom_, SEXP size_, SEXP max_iter_,
            SEXP gamma_, SEXP equalize_) {

  int size  = asInteger(size_);

  double zoom = asReal(zoom_);

  double cx = asReal(cx_); // -0.7;
  double cy = asReal(cy_); // 0.27015;

  double movex = asReal(movex_);
  double movey = asReal(movey_);

  int max_iter = asInteger(max_iter_);

  double idenom = 1.5/(0.5 * zoom * size);

  int max = 1;

  int *iters;
  iters = calloc(size * size, sizeof(int));
  int *iter_ptr = iters;

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

      *iter_ptr++ = iter;
    }
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Normalise into range [0-255] and cast as byte
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP rarray;
  PROTECT(rarray = allocVector(RAWSXP, size * size));
  uint8_t *raw_ptr = RAW(rarray);

  iter_ptr = iters;

  for (int i = 0; i < size * size; i++) {
    *raw_ptr++ = (uint8_t)round(*iter_ptr++/(double)max * 255);
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Histogram equalization
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int equalize = asLogical(equalize_);
  if (equalize) {
    uint32_t *hist;
    hist = (uint32_t *)calloc(256, sizeof(uint32_t));
    double cumsum[256];
    raw_ptr = RAW(rarray);

    // Count the pixels at each level
    for (int i = 0; i < size * size; i++) {
      hist[raw_ptr[i]]++;
    }

    // Calculate the cumulative sum of pixels
    cumsum[0] = hist[0];
    for (int i = 1; i < 256; i++) {
      cumsum[i] = cumsum[i-1] + hist[i];
    }

    // Rescale the cumulative sum by the total pixels and scale by the
    // max level i.e. 255
    uint8_t remap[256];
    for (int i = 0; i < 256; i++) {
      remap[i] = (uint8_t)floor(cumsum[i] / (size * size) * 255);
    }

    // Remap pixel values to the equalized bins in 'remap'
    raw_ptr = RAW(rarray);
    for (int i = 0; i < size * size; i++) {
      *raw_ptr = remap[*raw_ptr];
      raw_ptr++;
    }
    free(hist);
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Apply gamma correction
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  double gamma = asReal(gamma_);
  if (gamma != 1) {
    raw_ptr = RAW(rarray);
    for (int i = 0; i < size * size; i++) {
      *raw_ptr = (uint8_t)round(pow(*raw_ptr/255.0, gamma) * 255);
      raw_ptr++;
    }
  }


  free(iters);

  UNPROTECT(1);
  return rarray;
}
