## list_utils.r
##   - Utility functions for lists and vectors
##
## RGP - a GP system for R
## 2010 Oliver Flasch (oliver.flasch@fh-koeln.de)
## with contributions of Thomas Bartz-Beielstein, Olaf Mersmann and Joerg Stork
## released under the GPL v2
##

##' Functions for Lisp-like list processing
##'
##' Simple wrapper functions that allow Lisp-like list processing in R:
##' \code{first} to \code{fifth} return the first to fifth element of the list \code{x}.
##' \code{rest} returns all but the first element of the list \code{x}.
##' \code{is.empty} returns \code{TRUE} iff the list \code{x} is of length 0.
##' \code{is.atom} returns \code{TRUE} iff the list \code{x} is of length 1.
##' \code{is.composite} returns \code{TRUE} iff the list \code{x} is of length > 1.
##'
##' @param x A list or vector.
##'
##' @rdname lispLists
##' @export
first <- function(x) x[[1]]

##' @rdname lispLists
##' @export
rest <- function(x) x[-1]

##' @rdname lispLists
##' @export
second <- function(x) x[[2]]

##' @rdname lispLists
##' @export
third <- function(x) x[[3]]

##' @rdname lispLists
##' @export
fourth <- function(x) x[[4]]

##' @rdname lispLists
##' @export
fifth <- function(x) x[[5]]

##' @rdname lispLists
is.empty <- function(x) length(x) == 0

##' @rdname lispLists
##' @export
is.atom <- function(x) length(x) == 1

##' @rdname lispLists
##' @export
is.composite <- function(x) length(x) > 1

##' Sort a vector or list by the result of applying a function
##'
##' Sorts a vector or a list by the numerical result of applying the function \code{byFunc}.
##' @param xs A vector or list.
##' @param byFunc A function from elements of \code{xs} to \code{numeric}.
##' @return The result of sorting \code{xs} by \code{byfunc}.
##' @export
sortBy <- function(xs, byFunc) {
  if (missing(byFunc))
    o <- order(xs)
  else
    o <- order(sapply(xs, byFunc))
  xs[o]
}

##' Tabulate a function of one variable into a list
##'
##' Tabulates a function \code{f} by applying it to consecutive integers from 1 to \code{n}
##' and returns the results in a list.
##'
##' @param f The function to tabulate.
##' @param n The number of rows in the table to create.
##' @return The result of tabulating \code{f} into a list.
##'
##' @examples{tabulateList(sin, 2*pi)}
##' @export
tabulateList <- function(f, n) {
  if (n <= 0) {
    list()
  } else if (n == 1) {
    list(f(1))
  } else {
    x <- list(f(1))
    for (i in 2:n) x[[i]] <- f(i)
    x
  }
}

##' Concatenate a list or vector interspersing a separator
##'
##' @param x A list or vector to concatenate.
##' @param sep The object to intersperse.
##' @return The result vector or list.
##'
##' @examples{intersperse(c("bonnie", "clyde"), sep = " and ")}
##' @export
intersperse <- function(x, sep = " ") {
  if (length(x) <= 1) {
    x
  } else {
    result = x[[1]]
    for (i in 2:length(x)) result <- paste(result, x[[i]], sep = sep)
    result
  }
}

##' Generate random indices for lists or vectors
##'
##' Generates uniformly random list indices between 1 and \code{maxidx}.
##'
##' @param maxidx The largest index possible.
##' @param n The number of indices to generate.
##' @return A vector of one or more uniformly random list indices.
##' @export
randidx <- function(maxidx, n = 1) sample(maxidx, n, replace = TRUE)

##' Choose a random element from a list or vector
##'
##' Returns a unformly random chosen element of the vector or list \code{x}.
##' @param x The vector or list to chose an element from.
##' @return A uniformly random element of \code{x}.
##' @export
randelt <- function(x) {
  l <- length(x)
  if (l == 0)
    NULL
  else
    x[[randidx(length(x))]]
}