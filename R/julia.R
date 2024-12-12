

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
#' @param cx,cy real and imaginary components of 'c'. Default [-0.7, 0.27015]
#' @param size width and height of resulting square array. Default: 400
#' @param zoom zoom level. default 1.
#' @param movex,movey offsets in centre position Default [0, 0]
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
#' \dontrun{
#' js <- julia()
#' mode(js) <- 'integer'
#' plot(as.raster(js/255))
#' }
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
julia <- function(cx = -0.7, cy = 0.27015, movex = 0, movey = 0, zoom = 1,
                  size = 400, max_iter = 255, result = 'int', colors = NULL) {
  .Call(julia_, cx, cy, movex, movey, zoom, size, max_iter, result, colors)
}

