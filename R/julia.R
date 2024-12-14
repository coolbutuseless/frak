

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
#'   \item{\code{'int'}}{The integer matrix returned is a record of the raw iteration counts}
#'   \item{\code{'dbl'}}{The iteraction counts are normalised to the range [0, 1]}
#'   \item{\code{'nativeraster'}}{The matrix of iteraction counts is returned as a native raster with the 
#'   counts mapped to a sequence of colors specified in the \code{colors} argument}
#' }
#' @param palette integer vector of colours values for mapping to native raster output.  If
#'        no value specified, a default greyscale palette (256 colours) is used.
#'        E.g. \code{farver::encode_native(c('red', 'white', 'blue'))}
#' \itemize{
#'   \item{character vector of hex colou}
#'   \item{integer vector of packed RGBA values}
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
                  width = 400L, height = 400L, 
                  max_iter = 255L, result = 'int', palette = NULL) {
  
  .Call(
    julia_, 
    c_re, c_im,
    x, y, zoom,
    width, height,
    max_iter, result, palette
  )
}


if (FALSE) {
  
  im <- julia(result = 'int', width = 400, height = 200, zoom = 1)
  plot(as.raster(im/max(im)))

  julia(width = 400, height = 200, zoom = 1, result = 'nara') |> grid::grid.raster()
    
  
  library(displease)
  library(frak)
  
  x11(type = 'dbcairo', antialias = 'none')
  dev.control('inhibit')
 
  nr <- julia(result = 'nara')
  N  <- 30
  x  <- displease::seq_ease(0, 0.03, n = N)
  y  <- displease::seq_ease(0, 0.02, n = N, type = 'quad')
  zoom <- displease::seq_ease(1, 100, n = N, type = 'exp', direction = 'in')

  palette <- colorfast::col_to_int(rainbow(100))
  
  for (i in seq_along(x)) {
    cat(".")
    julia(result = nr, x = x[i], y = y[i], zoom = zoom[i], max_iter = 256 + i * 4, palette = palette)
    dev.hold()
    grid.raster(nr)   
    dev.flush()
    # Sys.sleep(0.04)
  }
  
  
}

