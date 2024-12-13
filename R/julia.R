

#' @useDynLib frak, .registration=TRUE
NULL



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Generate a julia set
#'
#' This function generates Julia fractals as an array of raw values.  The
#' values represent the number of iterations performed before the values at
#' the pixel location iterated towards infinity.  It is not a raw count of iterations,
#' but rather the count of iterations scaled to be a maximum of 255 - and thus
#' the data fits in a \code{raw} vector.
#'
#' @param c_re,c_im real and imaginary components of 'c'. Default [-0.7, 0.27015]
#' @param width,height size of the image
#' @param zoom zoom level. default 1.
#' @param x,y offsets in centre position Default [0, 0]
#' @param max_iter maximum number of iterations. Default: 256
#' @param result 'int', 'dbl', 'nativeraster', 'raster' OR an array of the
#'        right type which will be filled with the result.  If an array 
#'        is provided its dimensions will override the \code{size}
#'        argument.
#' @param colors if type is 'raster' or 'nativeraster', then this is the
#'        colour palette to use. If not provided, a default
#'        palette will be used.
#'
#' @return raw vector with dimensions (size, size)
#' @examples
#' im <- julia(result = 'int')
#' plot(as.raster(im/max(im)))
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
julia <- function(c_re = -0.7, c_im = 0.27015, 
                  x = 0, y = 0, zoom = 1,
                  width = 400, height = 400, 
                  max_iter = 255, result = 'int', colors = NULL) {
  
  # SEXP c_re_, SEXP c_im_, 
  # SEXP x_, SEXP y_, SEXP zoom_,
  # SEXP width_, SEXP height_, 
  # SEXP max_iter_, 
  # SEXP result_, SEXP colors_
  
  .Call(
    julia_, 
    c_re, c_im,
    x, y, zoom,
    width, height,
    max_iter, result, colors
  )
}

