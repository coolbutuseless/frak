

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate Julia set using base R matrix operations
#'
#' @inheritParams julia
#' @return matrix of iteration count
#' @examples
#' iters <- julia_r()
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
julia_r <- function(cx = -0.7, cy = 0.27015, size = 400, max_iter = 255, 
                    gamma = 1) {
  
  N <- size

  # Define the (x, y) grid of coordiantes
  x <- matrix(seq(-2, 2, length.out = N), N, N, byrow = TRUE)
  y <- matrix(seq(-2, 2, length.out = N), N, N, byrow = FALSE)
  
  # Where is the iteration still valid (i.e. hasn't diverged to infinity)
  valid <- rep(T, N * N)
  idx   <- which(valid)
  
  # A matrix to hold the iteration count
  iter  <- matrix(0L, N, N)
  
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
    tmp    <- xt * xt - yt * yt + cx 
    y[idx] <- 2 * xt * yt + cy       
    x[idx] <- tmp
  }
  
  # return the number of iterations
  iter
}


if (FALSE) {
  print(Sys.time() - start)
  cat("Complete at iteration: ", i, "\n")
  table(valid)
  
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # Simple plot
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cols <- viridisLite::viridis(256, begin = 0.1, end = 0.9)
  cols <- terrain.colors(256) 
  
  z <- round( (iter / max(iter)) ^ 0.25 * 255) + 1
  z[] <- cols[z]
  plot(as.raster(z), interpolate = FALSE)
  
  z <- sqrt(x * x + y * y)
  z <- round((z / max(z)) ^ gamma * 255) + 1
  z[] <- cols[z]
  plot(as.raster(z), interpolate = FALSE)
}