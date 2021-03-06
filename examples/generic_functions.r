## generic_functions.r
##   - A generic function framework with predicate dispatch for R
##
## RGP - a GP system for R
## 2010 Oliver Flasch (oliver.flasch@fh-koeln.de)
## with contributions of Thomas Bartz-Beielstein, Olaf Mersmann and Joerg Stork
## released under the GPL v2
##

##' @include function_utils.r
##' @include list_utils.r
NA

##' Generic functions with predicate dispatch
##'
##' TODO add possibility to add methods directly in the generic definition
generic <- function(defaultMethodFunction = no.default.method) {
  genericFunction <- new.function()
  formals(genericFunction) <- formals(defaultMethodFunction)
  body(genericFunction) <- body(function(...) {
    methods <- attr(sys.function(), "methods")
    for (method in methods) {
      if (eval(body(method$predicateFunction))) { # method predicate is true
        return(eval(body(method$methodFunction)))
      }
    }
    stop("generic: internal error (no matching method)") # this should never happen
  })
  class(genericFunction) <- c("generic", "function")
  defaultMethod <- list(predicateFunction = function(...) TRUE, methodFunction = defaultMethodFunction)
  attr(genericFunction, "methods") <- list(defaultMethod)
  genericFunction
}

no.default.method <- function(...)
  stop("no applicable method for arguments (", paste(..., sep = ","), ")")

add.method <- function(generic, predicate, methodFunction, m) {
  if (!inherits(generic, "generic")) stop("add.method: first argument must be a generic")
  if (!inherits(methodFunction, "function")) stop("add.method: third argument must be a function")
  if (length(formals(generic)) != length(formals(methodFunction))
      || !all(names(formals(generic)) == names(formals(methodFunction))))
    stop("add.method: methodFunction must have the same formal parameters as the generic's default function (",
         "has '", names(formals(methodFunction)),
         "' but expected '", names(formals(generic)), "')")
  predicateExpression <- substitute(predicate)
  predicateFunction <- new.function()
  formals(predicateFunction) <- formals(generic)
  body(predicateFunction) <- predicateExpression
  method <- list(predicateFunction = predicateFunction, methodFunction = methodFunction)
  methods <- c(list(method), # second "list" because c() flattens its arguments
               attr(generic, "methods"))
  sortedMethods <- tryCatch(insertionSort(methods, predicateImplication),
                            error = function(e) stop("add.method: TODO")) # TODO
  # note that this assignment leaves the attributes of the formal parameter generic unchanged,
  # because generic is a copy
  attr(generic, "methods") <- sortedMethods
  generic
}

`%implies%` <- function(predicateA, predicateB) {
  stop("`%implies%`": TODO") # TODO
}

list.methods <- NULL # TODO

dispatch.generic <- function(generic, ...) {
  NULL
} # TODO
