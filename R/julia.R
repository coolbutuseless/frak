

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
#' @examples
#' \dontrun{
#' js <- julia()
#' mode(js) <- 'integer'
#' plot(as.raster(js/255))
#' }
#'
#' @param cx,cy real and imaginary components of 'c'
#' @param size width and height of resulting array
#' @param zoom zoom level. default 1.
#' @param movex,movey offsets in centre position
#' @param max_iter maximum number of iterations
#'
#' @return raw vector with dimensions (size, size)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
julia <- function(cx = -0.7, cy = 0.27015, movex = 0, movey = 0, zoom = 1,
                  size = 400, max_iter = 255) {
  res <- .Call(julia_, cx, cy, movex, movey, zoom, size, max_iter)

  dim(res) <- c(size, size)
  res
}

