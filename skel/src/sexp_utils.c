/* sexp_utils.c
 *
 */

#include "sexp_utils.h"
#include <setjmp.h>


SEXP deep_copy_closxp(SEXP closxp) {
  SEXP copied_closxp = duplicate(closxp);
  // duplicate() by default does a shallow copy of a CLOSXP body, fix this
  SET_BODY(copied_closxp, duplicate(BODY(closxp)));
  return copied_closxp;
}

SEXP map_sexp(SEXP (*const f)(SEXP), SEXP sexp) {
  switch (TYPEOF(sexp)) { // switch for speed
  case NILSXP:
    return sexp; // do nothing with nils
  case LANGSXP: // fall-through to next case
  case LISTSXP:
    return f(LCONS(map_sexp(f, CAR(sexp)),
                   map_sexp(f, CDR(sexp)))); // map inner nodes, recurse
  default: // base case
    return f(sexp); // map leafs
  }
}

SEXP map_sexp_leafs(SEXP (*const f)(SEXP), SEXP sexp) {
  switch (TYPEOF(sexp)) { // switch for speed
  case NILSXP:
    return sexp; // do nothing with nils
  case LANGSXP: // fall-through to next case
  case LISTSXP:
    return LCONS(map_sexp_leafs(f, CAR(sexp)),
                 map_sexp_leafs(f, CDR(sexp))); // do nothing with inner nodes, recurse
  default: // base case
    return f(sexp); // map leafs
  }
}

SEXP map_sexp_inner_nodes(SEXP (*const f)(SEXP), SEXP sexp) {
  switch (TYPEOF(sexp)) { // switch for speed
  case NILSXP:
    return sexp; // do nothing with nils
  case LANGSXP: // fall-through to next case
  case LISTSXP:
    return f(LCONS(map_sexp_inner_nodes(f, CAR(sexp)),
                   map_sexp_inner_nodes(f, CDR(sexp)))); // map inner nodes, recurse
  default: // base case
    return sexp; // do noting with leafs
  }
}

// TODO
SEXP map_sexp_shortcut(SEXP (*const f)(SEXP), SEXP sexp) {
  switch (TYPEOF(sexp)) { // switch for speed
  case NILSXP:
    return sexp; // do nothing with nils
  case LANGSXP: // fall-through to next case
  case LISTSXP: {
    SEXP mapped_sexp = f(sexp);
    if (sexp == mapped_sexp)
      return LCONS(map_sexp_shortcut(f, CAR(sexp)),
                   map_sexp_shortcut(f, CDR(sexp))); // recurse
    else
      return mapped_sexp; // shortcut
  }
  default: // base case
    return f(sexp); // map leafs
  }
}

// TODO
static SEXP map_sexp_shortcut_depth_recursive(SEXP (*const f)(SEXP, int), SEXP sexp, const int current_depth) {
  switch (TYPEOF(sexp)) { // switch for speed
  case NILSXP:
    return sexp; // do nothing with nils
  case LANGSXP: // fall-through to next case
  case LISTSXP: {
    SEXP mapped_sexp = f(sexp, current_depth);
    if (sexp == mapped_sexp) // recurse iff the current sexp was not replaced
      return LCONS(map_sexp_shortcut_depth_recursive(f, CAR(sexp), current_depth + 1),
                   map_sexp_shortcut_depth_recursive(f, CDR(sexp), current_depth + 1)); // recurse
    else
      return mapped_sexp; // shortcut
  }
  default: // base case
    return f(sexp, current_depth); // map leafs
  }
}

// TODO
SEXP map_sexp_shortcut_depth(SEXP (*const f)(SEXP, int), SEXP sexp) {
  return map_sexp_shortcut_depth_recursive(f, sexp, 0);
}

void modify_sexp_shortcut(void (*const f)(SEXP), SEXP sexp) {
  switch (TYPEOF(sexp)) { // switch for speed
  case NILSXP:
    return; // do nothing with nils
  case LANGSXP: // fall-through to next case
  case LISTSXP:
    f(sexp); // modify (entire) expression
    modify_sexp_shortcut(f, CAR(sexp)); // recurse on first element of expression
    modify_sexp_shortcut(f, CDR(sexp)); // recurse on rest elements of expression
  default: // base case
    f(sexp); // map leafs
  }
}

int sexp_size(SEXP sexp) {
  switch (TYPEOF(sexp)) { // switch for speed
  case NILSXP:
    return 0;
  case LANGSXP: // fall-through to next case
  case LISTSXP:
    return sexp_size(CAR(sexp)) + sexp_size(CDR(sexp));
  default: // base case
    return 1; 
  }
}

SEXP sexp_size_R(SEXP sexp) {
  SEXP s;
  PROTECT(s = allocVector(INTSXP, 1));
  INTEGER(s)[0] = sexp_size(sexp);
  UNPROTECT(1);
  return s;
}

