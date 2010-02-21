## search_space.r
##   - Functions for defining the search space for symbolic regression
##
## RGP - a GP system for R
## 2010 Oliver Flasch (oliver.flasch@fh-koeln.de)
## with contributions of Thomas Bartz-Beielstein, Olaf Mersmann and Joerg Stork
## released under the GPL v2
##

##' Functions for defining the search space for symbolic regression
##'
##' The GP search space is defined by a set of functions, a set of input variables,
##' and a set of rules how these functions and input variables may be combined to
##' form valid symbolic expressions. Functions are simply given as strings naming
##' functions in the global environment. Input variables are also given as strings.
##' Combination rules are implemented by assigning sTypes to functions and input
##' variables.
##' TODO
##' 
##' Multiple function sets or multiple input variable sets can be combined using
##' the \code{\link{c}} function.
##' \code{functionSet} creates a function set.
##' \code{inputVariableSet} creates an input variable set.
##' \code{functionSetFromList} and \code{inputVariableSetFromList} are variants
##' that take a list as their only parameter.
##'
##' @param ... Names of functions or input variables given as strings.
##' @return A function set or input variable set.
##'
##' @examples{
##' functionSet("+", "-", "*", "/", "expt", "log", "sin", "cos", "tan")
##' inputVariableSet("x", "y")
##' }
##' @rdname searchSpaceDefinition
##' @export
functionSet <- function(...) functionSetFromList(list(...))

##' @rdname searchSpaceDefinition
##' @export
inputVariableSet <- function(...) inputVariableSetFromList(list(...))

##' @rdname searchSpaceDefinition
##' @export
functionSetFromList <- function(l) {
  funcset <- list()
  funcset$all <- Map(as.name, l)
  class(funcset) <- c("functionSet", "list")
  funcset
}

##' @rdname searchSpaceDefinition
##' @export
inputVariableSetFromList <- function(l) {
  inset <- list()
  inset$all <- Map(as.name, l)
  class(inset) <- c("inputVariableSet", "list")
  inset
}

##' @rdname searchSpaceDefinition
##' @export
c.functionSet <- function(..., recursive = FALSE) {
  fSets <- list(...)
  combinedFsets <- list()
  for (fSet in fSets) combinedFsets <- append(fSet$all, combinedFsets)
  functionSetFromList(combinedFsets)
}

##' @rdname searchSpaceDefinition
##' @export
c.inputVariableSet <- function(..., recursive = FALSE) {
  iSets <- list(...)
  combinedIsets <- list()
  for (iSet in iSets) combinedIsets <- append(iSet$all, combinedIsets)
  inputVariableSetFromList(combinedIsets)
}