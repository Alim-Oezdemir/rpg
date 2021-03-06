#include "create_expr_tree.h"
#include "population.h"

SEXP createPopulation(SEXP popSize_ext, SEXP funcSet, SEXP inSet, SEXP maxDepth_ext, SEXP constProb_ext, SEXP subtreeProb_ext, SEXP constScaling_ext) {
  SEXP pop;
  PROTECT(popSize_ext = coerceVector(popSize_ext, INTSXP));
  int popSize= INTEGER(popSize_ext)[0];

  PROTECT(pop= allocVector(VECSXP, popSize));
  for(int i=0; i < popSize; i++)
  {
    SET_VECTOR_ELT(pop, i, randFuncGrow(funcSet, inSet, maxDepth_ext, constProb_ext, subtreeProb_ext, constScaling_ext));
  }
  UNPROTECT(2);
  return pop;
}


// R CMD SHLIB population.c create_expr_tree.c
//Test: makePop <- function(popsize,funcset,inset,maxdepth,constprob,subtreeprob) { .Call("createPopulation", popsize= 10, funcset= c("+","-","*","/"),inset=("x"),maxdepth= 8,constprob= 0.2,subtreeprob= 0.5) }
