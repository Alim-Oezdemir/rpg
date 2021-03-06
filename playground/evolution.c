#include "eval_vectorized.h"
#include "mutate_function.h"
#include "selection.h"
#include "population.h"
#include "create_expr_tree.h"
#include <time.h> 

void summary(SEXP population,SEXP actualParameters, SEXP targetValues, double * bestRMSE) {

int popSize= LENGTH(population);
SEXP rmseVectorSum;
PROTECT(rmseVectorSum= allocVector(VECSXP, popSize));

for(int i= 0; i < popSize; i++) {
SET_VECTOR_ELT(rmseVectorSum, i, evalVectorizedRmse(VECTOR_ELT(population, i), actualParameters, targetValues, bestRMSE)); 
}
int *numbersRMSE= Calloc(popSize * sizeof(int), int);
int *sortedNumbersRMSE= Calloc(popSize * sizeof(int), int);
double *sortedRMSE= Calloc(popSize * sizeof(double), double);
for(int i=0; i < popSize; i++) {
numbersRMSE[i]=i + 1;
}

sortByRmse(numbersRMSE, sortedNumbersRMSE, popSize, rmseVectorSum, sortedRMSE);

PROTECT(rmseVectorSum= coerceVector(rmseVectorSum, REALSXP));
double rmseArray[popSize];
for(int i= 0; i < popSize; i++) {
  rmseArray[i]= REAL(rmseVectorSum)[i];
}
for(int i= 0; i < popSize; i++) {
  int temp=sortedNumbersRMSE[i] - 1;
  printf("\n Number: %d, RMSE: %f ",sortedNumbersRMSE[i], rmseArray[temp]);
}
Free(numbersRMSE);
Free(sortedNumbersRMSE);
Free(sortedRMSE);
UNPROTECT(2);
}

SEXP restartPopulation(SEXP population,SEXP actualParameters, SEXP targetValues,SEXP funcSet, SEXP inSet, SEXP maxDepth_ext, SEXP constProb_ext, SEXP subtreeProb_ext, SEXP constScaling_ext, double * bestRMSE) {

int popSize= LENGTH(population);
SEXP rmseVectorSum;
PROTECT(rmseVectorSum= allocVector(VECSXP, popSize));

for(int i= 0; i < popSize; i++) {
SET_VECTOR_ELT(rmseVectorSum, i, evalVectorizedRmse(VECTOR_ELT(population, i), actualParameters, targetValues, bestRMSE)); 
}
int *numbersRMSE= Calloc(popSize * sizeof(int), int);
int *sortedNumbersRMSE= Calloc(popSize * sizeof(int), int);
double *sortedRMSE= Calloc(popSize * sizeof(double), double);
for(int i=0; i < popSize; i++) {
numbersRMSE[i]=i + 1;
}
sortByRmse(numbersRMSE, sortedNumbersRMSE, popSize, rmseVectorSum, sortedRMSE);

double restartCrit = sortedRMSE[0];

if (restartCrit == *bestRMSE)
  for(int i=1; i < popSize; i++) {
    if(sortedRMSE[i] == restartCrit) {
       //Rprintf("restart%f", sortedRMSE[i]);
       SET_VECTOR_ELT(population, sortedNumbersRMSE[i] - 1, randFuncGrow(funcSet, inSet, maxDepth_ext, constProb_ext, subtreeProb_ext, constScaling_ext));
    } }
UNPROTECT(1);
Free(numbersRMSE);
Free(sortedNumbersRMSE);
Free(sortedRMSE);
return population;
}

SEXP evolutionRun(SEXP numberOfRuns_ext, SEXP popSize_ext, SEXP sampleSize_ext, SEXP actualParameters, SEXP targetValues, SEXP funcSet, SEXP inSet, SEXP maxDepth_ext, SEXP maxLeafs_ext, SEXP maxNodes_ext, SEXP constProb_ext, SEXP constScaling_ext,  SEXP subtreeProb_ext, SEXP RMSElimit_ext, SEXP returnRMSE_ext, SEXP silent_ext, SEXP timeLimit_ext) {

  SEXP population;

    PROTECT(population= createPopulation(popSize_ext, funcSet, inSet, maxDepth_ext, constProb_ext, subtreeProb_ext, constScaling_ext));
    PROTECT(numberOfRuns_ext= coerceVector(numberOfRuns_ext, INTSXP));
    int numberOfRuns= INTEGER(numberOfRuns_ext)[0];

    PROTECT(RMSElimit_ext= coerceVector(RMSElimit_ext, REALSXP));
    double RMSElimit= REAL(RMSElimit_ext)[0];

    PROTECT(silent_ext= coerceVector(silent_ext, INTSXP));
    int silent= INTEGER(silent_ext)[0]; 

    PROTECT(timeLimit_ext= coerceVector(timeLimit_ext, INTSXP));
    int timeLimit= INTEGER(timeLimit_ext)[0]; 

    PROTECT(returnRMSE_ext= coerceVector(returnRMSE_ext, INTSXP));
    int rRMSE= INTEGER(returnRMSE_ext)[0]; 
  
    clock_t prgstart, prgende;
    prgstart=clock();

    double bestRMSE= 1000000;
  for(int i=0; i < numberOfRuns; i++) {
    
      PROTECT(population= selection(population, sampleSize_ext, actualParameters, targetValues, funcSet, inSet, maxDepth_ext, constProb_ext, subtreeProb_ext, maxLeafs_ext, maxNodes_ext, constScaling_ext, &bestRMSE));
      if(silent == 0) {
        Rprintf(" \n StepNumber: %d", i+1);
        Rprintf(" best RMSE: %f", bestRMSE); 
        }
      if(RMSElimit > 0) {
        if (RMSElimit >= bestRMSE) {
          UNPROTECT(7);
          if(rRMSE == 0) {
            if(silent != 1) {
              summary(population, actualParameters, targetValues, &bestRMSE);
            }
            return population; 
          } else {
          SEXP returnRMSE;
          PROTECT(returnRMSE = allocVector(REALSXP, 1));
          REAL(returnRMSE)[0] = bestRMSE;
          UNPROTECT(1);
            return returnRMSE; } }
        } 
     
     prgende=clock();
     float runtime= (float)(prgende-prgstart) / CLOCKS_PER_SEC;
     if(silent != 1) {
       Rprintf(" runtime: %.2f sec", runtime);
     }

      if(timeLimit > 0) {
        if ((int)runtime >= timeLimit) {
          UNPROTECT(7);
          if(rRMSE == 0) {
            if(silent != 1) {
              summary(population, actualParameters, targetValues, &bestRMSE);
            }
            return population; 
          } else {
          SEXP returnRMSE;
          PROTECT(returnRMSE = allocVector(REALSXP, 1));
          REAL(returnRMSE)[0] = bestRMSE;
          UNPROTECT(1);
          return returnRMSE; } }
        } 
     UNPROTECT(1); 
}


  if(silent != 1) {
    summary(population, actualParameters, targetValues, &bestRMSE);
  }
  UNPROTECT(6);
  if(rRMSE == 0) {
   return population; 
  } else {
   SEXP returnRMSE;
   PROTECT(returnRMSE = allocVector(REALSXP, 1));
   REAL(returnRMSE)[0] = bestRMSE;
   UNPROTECT(1);
   return returnRMSE; }
}





//   pop1 <- .Call("evolutionRun", Runs = 1000, popsize= 40, samples= 20, actualParameters= c(1,2,3,4,5,6), targetValues= c(1.1,4.1,9.1,16.1,25.1,36.1), funcset= c("+","-","*","/"),inset=c("x"),maxdepth= 7,constprob= 0.2,subtreeprob= 0.5) 
