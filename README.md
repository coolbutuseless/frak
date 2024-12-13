
<!-- README.md is generated from README.Rmd. Please edit that file -->

# frak <img src="man/figures/logo-trans.png" align="right" height=230/>

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
![](https://img.shields.io/badge/API-unstable-orange.svg)
[![R-CMD-check](https://github.com/coolbutuseless/frak/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coolbutuseless/frak/actions/workflows/R-CMD-check.yaml)
<!-- badges: end -->

`frak` is a small R package to generate fractals - currently just [Julia
Sets](https://en.wikipedia.org/wiki/Julia_set).

I needed a fast way to generate test images for testing other software,
with images that

- are fast to produce
- are interesting, not just white noise
- 8-bits-per-pixel, so the value can fit in an R `raw` vector or a C
  `uint8_t` vector.
- can be be panned, zoomed and changed with simple changes to the input
  variables

## Installation

You can install from [GitHub](https://github.com/coolbutuseless/frak)
with:

``` r
# install.packages('remotes')
remotes::install_github('coolbutuseless/frak')
```

## What’s in the box?

- `julia(cx, cy, movex, movey, zoom, size, max_iter)` generate a julia
  set, where `cx` and `cy` are the components of imaginary iteration
  variable *c*.

Returned array of raw values represents the iteration count at which a
particular location veered towards infinity. These values are scaled to
span the full range from allowable within a raw vector (i.e. the whole
numbers from 0 to 255)

## Example: Generate a julia set

``` r
library(frak)

fractal <- julia()
plot(as.raster(fractal/255))
```

<img src="man/figures/README-example-1.png" width="100%" />

## Example: colour the image with viridis

``` r
library(viridisLite)

# generate a fractal
fractal <- julia(zoom = 3)

# Grab a palette from viridis
pal <- rev(viridisLite::viridis(256))

# Recode the values with the palette. Lowest raw value is 0, so offset by 1.
fractal_raster   <- fractal
fractal_raster[] <- pal[fractal_raster + 1]

plot(as.raster(fractal_raster))
```

<img src="man/figures/README-unnamed-chunk-2-1.png" width="100%" />

## Example: GIF anim rendered from multiple frames

<details>

<summary>

Click to show/hide code to generate gif
</summary>

``` r
# remotes::install_github('coolbutuseless/displease')
library(fastpng)
library(displease)
library(frak)

dir.create('man/figures/anim', showWarnings = FALSE)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Use {displease} to get a pleasing motion as the zoom is performed
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N <- 100
zoom <- displease::seq_ease(1, 1000, n = N, type = 'exp', direction = 'in')

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Loop: create an image dump it to file.
# Using {foist} here for fast image output
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
for (i in seq(N)) {
  frac <- frak::julia(c_re = -0.71, zoom = zoom[i], x = 0.1)
  mode(frac) <- 'integer'
  frac <- frac/255
  
  filename <- sprintf("man/figures/anim/%03i.png", i)
  
  fastpng::write_png(
    data                 = frac, 
    filename             = filename, 
    pal                  = foist::vir$magma, 
    convert_to_row_major = FALSE
  )
}

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Create the animation using imagemagick and delete the source frames
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
system("convert man/figures/anim/*.png man/figures/anim.gif")
unlink("man/figures/anim/", recursive = TRUE)
```

</details>

<img src="man/figures/anim.gif" width="50%" />

## Benchmark

Creating a julia set in C is about 100x faster than using plain R (as in
the [Julia](https://cran.r-project.org/package=Julia) package on CRAN)

<details>

<summary>

Click to show/hide bench::mark() code
</summary>

``` r
suppressPackageStartupMessages({
  library(Julia)
  library(frak)
  library(ggplot2)
  library(dplyr)
  library(tidyr)
})

size <- 400

# Julia::JuliaImage does at most 50 iterations
res <- bench::mark(
  `Julia::JuliaImage()` = Julia::JuliaImage(imageN = size, centre = 0.0, L = 4, C = 1-1.6180339887),
  `frak::julia()`       = julia(max_iter = 50),
  `frack::julia_r()`    = julia_r(max_iter = 50),
  check = FALSE
)
#> Warning: Some expressions had a GC in every iteration; so filtering is
#> disabled.
```

</details>

| expression          |      min |   median |    itr/sec |
|:--------------------|---------:|---------:|-----------:|
| Julia::JuliaImage() | 288.88ms | 290.35ms |   3.444074 |
| frak::julia()       |   2.26ms |   2.35ms | 420.138483 |
| frack::julia_r()    |  45.08ms |  48.06ms |  19.690096 |

<img src="man/figures/README-unnamed-chunk-6-1.png" width="100%" />

## Related Software

- [mandelbrot](https://cran.r-project.org/package=mandelbrot) - Generate
  view on the Mandelbrot Set.
- [Julia](https://cran.r-project.org/package=Julia) - a Pure R Julia Set
  generator
