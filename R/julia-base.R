

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate Julia set using base R matrix operations
#' 
#' Users are encouraged to copy the body of this function and hack on it in 
#' order to achieve novel renderings.
#'
#' @inheritParams julia
#' @return matrix of iteration count
#' @examples
#' # Calculate number of iterations for each location with default parameters
#' iters <- julia_r()
#' 
#' # Specify the colour scheme
#' cols <- terrain.colors(256) 
#' 
#' # Convert the integer iteration count to the range [1, 256]
#' # Do a little bit of gamma correction while we're at it
#' z <- round( (iters / max(iters)) ^ 0.25 * 255) + 1
#' 
#' # Convert from integer to colour
#' z[] <- cols[z]
#' plot(as.raster(z), interpolate = FALSE)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
julia_r <- function(c_re = -0.7, c_im = 0.27015, width = 400, height = 200, max_iter = 255) {

  # Define the (x, y) grid of coordiantes
  x <- matrix(seq(-2, 2, length.out =  width), height, width, byrow = TRUE)
  y <- matrix(seq(-2, 2, length.out = height), height, width, byrow = FALSE)
  
  # Where is the iteration still valid (i.e. hasn't diverged to infinity)
  valid <- rep(T, height * width)
  idx   <- which(valid)
  
  # A matrix to hold the iteration count
  iter  <- matrix(0L, height, width)
  
  for (i in seq(max_iter)) {
    # Just calculate on the (x,y) which are still valid
    xt <- x[idx]
    yt <- y[idx]

    # where is the (squared) magnitude still < 4    
    valid[idx] <- (xt*xt + yt*yt) < 4
    
    # terminate early if all values have escaped
    idx <- which(valid)
    if (length(idx) == 0) break;
    
    # Bump the iteraton count
    iter[idx] <- iter[idx] + 1L
    
    # Update which x,y we'll be calculating on (using latest idx)
    xt <- x[idx]
    yt <- y[idx]
    
    # Iterative calculation
    tmp    <- xt * xt - yt * yt + c_re 
    y[idx] <- 2 * xt * yt + c_im       
    x[idx] <- tmp
  }
  
  # return the number of iterations
  iter
}


if (FALSE) {
  
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # Simple plot
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  iter <- julia_r()
  
  cols <- viridisLite::viridis(256, begin = 0.1, end = 0.9)
  cols <- terrain.colors(256) 
  
  z <- round( (iter / max(iter)) ^ 0.25 * 255) + 1
  z[] <- cols[z]
  plot(as.raster(z), interpolate = FALSE)
}







