

#' @useDynLib frak, .registration=TRUE
NULL



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Generate a Julia Set
#'
#' This function generates a fractal image - the value at each location 
#' represents the number of iterations performed before the function
#' evaluation tends towards infinity.  
#'
#' @param c_re,c_im real and imaginary components of 'c'. Default [-0.7, 0.27015]
#' @param width,height size of the output image default: 400x400
#' @param zoom zoom level. default 1.
#' @param x,y offsets in centre position Default [0, 0]
#' @param max_iter maximum number of iterations. Default: 255
#' @param result 'int', 'dbl', 'nativeraster' (or 'nara').
#' \describe{
#'   \item{\code{'int'}}{The integer matrix returned is a record of the raw counts}
#'   \item{\code{'dbl'}}{The iteraction counts are normalised to be in the range [0, 1]}
#'   \item{\code{'nativeraster'}}{The matrix is returned as a native raster with the 
#'   colors specified in the \code{colors} argument (not yet implemented)}
#' }
#'
#' @return raw vector with dimensions (size, size)
#' @examples
#' im <- julia(result = 'int', height = 200, zoom = 4, x = 0.2)
#' plot(as.raster(im/max(im)))
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
julia <- function(c_re = -0.7, c_im = 0.27015, 
                  x = 0, y = 0, zoom = 1,
                  width = 400, height = 400, 
                  max_iter = 255, result = 'int') {
  
  .Call(
    julia_, 
    c_re, c_im,
    x, y, zoom,
    width, height,
    max_iter, result, NULL
  )
}


if (FALSE) {
  
  im <- julia(result = 'int', width = 400, height = 200, zoom = 1)
  plot(as.raster(im/max(im)))

  julia(width = 400, height = 200, zoom = 1, result = 'nara') |> grid::grid.raster()
    
}

