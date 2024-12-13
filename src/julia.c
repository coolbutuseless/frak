
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>


int default_packed_cols[256] = {
  -16777216, -16711423, -16645630, -16579837, -16514044, -16448251, 
  -16382458, -16316665, -16250872, -16185079, -16119286, -16053493, 
  -15987700, -15921907, -15856114, -15790321, -15724528, -15658735, 
  -15592942, -15527149, -15461356, -15395563, -15329770, -15263977, 
  -15198184, -15132391, -15066598, -15000805, -14935012, -14869219, 
  -14803426, -14737633, -14671840, -14606047, -14540254, -14474461, 
  -14408668, -14342875, -14277082, -14211289, -14145496, -14079703, 
  -14013910, -13948117, -13882324, -13816531, -13750738, -13684945, 
  -13619152, -13553359, -13487566, -13421773, -13355980, -13290187, 
  -13224394, -13158601, -13092808, -13027015, -12961222, -12895429, 
  -12829636, -12763843, -12698050, -12632257, -12566464, -12500671, 
  -12434878, -12369085, -12303292, -12237499, -12171706, -12105913, 
  -12040120, -11974327, -11908534, -11842741, -11776948, -11711155, 
  -11645362, -11579569, -11513776, -11447983, -11382190, -11316397, 
  -11250604, -11184811, -11119018, -11053225, -10987432, -10921639, 
  -10855846, -10790053, -10724260, -10658467, -10592674, -10526881, 
  -10461088, -10395295, -10329502, -10263709, -10197916, -10132123, 
  -10066330, -10000537, -9934744, -9868951, -9803158, -9737365, 
  -9671572, -9605779, -9539986, -9474193, -9408400, -9342607, -9276814, 
  -9211021, -9145228, -9079435, -9013642, -8947849, -8882056, -8816263, 
  -8750470, -8684677, -8618884, -8553091, -8487298, -8421505, -8355712, 
  -8289919, -8224126, -8158333, -8092540, -8026747, -7960954, -7895161, 
  -7829368, -7763575, -7697782, -7631989, -7566196, -7500403, -7434610, 
  -7368817, -7303024, -7237231, -7171438, -7105645, -7039852, -6974059, 
  -6908266, -6842473, -6776680, -6710887, -6645094, -6579301, -6513508, 
  -6447715, -6381922, -6316129, -6250336, -6184543, -6118750, -6052957, 
  -5987164, -5921371, -5855578, -5789785, -5723992, -5658199, -5592406, 
  -5526613, -5460820, -5395027, -5329234, -5263441, -5197648, -5131855, 
  -5066062, -5000269, -4934476, -4868683, -4802890, -4737097, -4671304, 
  -4605511, -4539718, -4473925, -4408132, -4342339, -4276546, -4210753, 
  -4144960, -4079167, -4013374, -3947581, -3881788, -3815995, -3750202, 
  -3684409, -3618616, -3552823, -3487030, -3421237, -3355444, -3289651, 
  -3223858, -3158065, -3092272, -3026479, -2960686, -2894893, -2829100, 
  -2763307, -2697514, -2631721, -2565928, -2500135, -2434342, -2368549, 
  -2302756, -2236963, -2171170, -2105377, -2039584, -1973791, -1907998, 
  -1842205, -1776412, -1710619, -1644826, -1579033, -1513240, -1447447, 
  -1381654, -1315861, -1250068, -1184275, -1118482, -1052689, -986896, 
  -921103, -855310, -789517, -723724, -657931, -592138, -526345, 
  -460552, -394759, -328966, -263173, -197380, -131587, -65794, 
  -1
};

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP julia_(SEXP c_re_, SEXP c_im_, 
            SEXP x_, SEXP y_, SEXP zoom_,
            SEXP width_, SEXP height_, 
            SEXP max_iter_, 
            SEXP result_, SEXP colors_) {

  int nprotect = 0;
  
  int width      = Rf_asInteger(width_);
  int height     = Rf_asInteger(height_);
  int size       = MAX(width, height);
  double zoom    = Rf_asReal(zoom_);
  double c_re    = Rf_asReal(c_re_); // -0.7;
  double c_im    = Rf_asReal(c_im_); // 0.27015;
  double x       = Rf_asReal(x_);
  double y       = Rf_asReal(y_);
  int max_iter   = Rf_asInteger(max_iter_);

  double idenom = 2/(0.5 * zoom * size);

  int max_iter_count = 1;

  int *iter_count = calloc((size_t)(width * height), sizeof(int));
  if (iter_count == NULL) {
    Rf_error("julia_(): Could not allocate memory for 'iters'");
  }
  int *iter_ptr = iter_count;

  for (int col = 0; col < width; ++col) {
    for (int row = 0; row < height; ++row) {
      double re = (col -  width/2) * idenom + x;
      double im = (row - height/2) * idenom + y;

      int niter = 0;
      while( (re * re + im * im < 4) & (niter < max_iter)) {
        double tmp = re * re - im * im + c_re;
        im = 2 * re * im + c_im;
        re = tmp;
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
      PROTECT(rarray = Rf_allocMatrix(RAWSXP, height, width)); nprotect++;
      uint8_t *raw_ptr = RAW(rarray);
      
      iter_ptr = iter_count;
      
      for (int i = 0; i < height * width; i++) {
        *raw_ptr++ = (uint8_t)round(*iter_ptr++/(double)max_iter_count * 255);
      }
    } else if (strcmp(result, "int") == 0) {
      PROTECT(rarray = Rf_allocMatrix(INTSXP, height, width)); nprotect++;
      memcpy(INTEGER(rarray), iter_count, height * width * sizeof(int));
    } else if (strcmp(result, "dbl") == 0) {
      PROTECT(rarray = Rf_allocMatrix(REALSXP, height, width)); nprotect++;
      double *ptr = REAL(rarray);
      
      iter_ptr = iter_count;
      
      for (int i = 0; i < height * width; i++) {
        *ptr++ = (double)(*iter_ptr++/(double)max_iter_count);
      }
    } else if (strcmp(result, "nativeraster") == 0 || strcmp(result, "nara") == 0) {
      PROTECT(rarray = Rf_allocMatrix(INTSXP, width, height)); nprotect++;
      int *ptr = INTEGER(rarray);
      iter_ptr = iter_count;
      for (int i = 0; i < height * width; i++) {
        int idx = round(*iter_ptr++/(double)max_iter_count * 255);
        *ptr++ = default_packed_cols[idx];
      }
      Rf_setAttrib(rarray, R_ClassSymbol, Rf_mkString("nativeRaster"));
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



