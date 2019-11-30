/**
 * Calculate the max arguments amount of print.
 * Print the symbol table.
 */

#include "prog1.h"
#include "slp.h"
#include "util.h"
#include <stdio.h>

typedef struct M_table_ *M_table;
typedef struct M_intAndTable_ *M_intAndTable;

struct M_table_ {
  string id;
  int value;
  M_table tail;
};
struct M_intAndTable_ {
  int i;
  M_table t;
};

M_table M_Table(string id, int value, M_table tail) {
  M_table t = checked_malloc(sizeof(*t));
  t->id = id;
  t->value = value;
  t->tail = tail;
  return t;
}
M_intAndTable M_IntAndTable(int i, M_table t) {
  M_intAndTable it = checked_malloc(sizeof(*it));
  it->i = i;
  it->t = t;
  return it;
}

int argsMax = 0;

// Function declarations
int maxargs(A_stm s);
void stmCount(A_stm s);
int expsCount(A_expList el);
void expCount(A_exp e);
void interp(A_stm s);
M_table interpStm(A_stm s, M_table t);
M_intAndTable interpExp(A_exp e, M_table t);
M_table interpExpList(A_expList l, M_table t);
int lookup(M_table t, string key);

// Return the max arguments number of print statement.
int maxargs(A_stm s) {
  stmCount(s);
  return argsMax;
}

void stmCount(A_stm s) {
  int count = 0;
  switch (s->kind) {
    case 0:
      stmCount(s->u.compound.stm1);
      stmCount(s->u.compound.stm2);
      break;
    case 1:
      expCount(s->u.assign.exp);
      break;
    case 2:
      count = expsCount(s->u.print.exps);
      argsMax = argsMax > count ? argsMax : count;
      break;
  }
}

int expsCount(A_expList el) {
  if (el->kind == 0) {
    expCount(el->u.pair.head);
    return 1 + expsCount(el->u.pair.tail);
  }
  if (el->kind == 1) {
    expCount(el->u.last);
    return 1;
  }
}

void expCount(A_exp e) {
  if (e->kind == 3) {
    stmCount(e->u.eseq.stm);
    expCount(e->u.eseq.exp);
  }
}

// Interpreter
void interp(A_stm s) {
  // Symbol table
  M_table t = checked_malloc(sizeof(*t));
  t->tail = NULL;
  // Scan the grammar tree.
  t = interpStm(s, t);
  // Print the symbol table.
  while (t->tail != NULL) {
    printf("%s\n", t->id);
    printf("%d\n", t->value);
    t = t->tail;
  }
}

// Handle the interpStm statement.
M_table interpStm(A_stm s, M_table t) {
  M_table tTemp;
  M_intAndTable itTemp;
  // Choose the next step.
  switch (s->kind) {
    // compound
    case 0:
      tTemp = interpStm(s->u.compound.stm1, t);
      return interpStm(s->u.compound.stm2, tTemp);
      break;
    // assign
    case 1:
      itTemp = interpExp(s->u.assign.exp, t);
      return M_Table(s->u.assign.id, itTemp->i, itTemp->t);
      break;
    // print
    case 2:
      tTemp = interpExpList(s->u.print.exps, t);
      return tTemp;
      break;
  }
}

// Handle the exp statement.
M_intAndTable interpExp(A_exp e, M_table t) {
  M_intAndTable itTemp1;
  M_intAndTable itTemp2;
  M_table tTemp;
  // Choose the next step.
  switch (e->kind) {
    // id
    case 0:
      return M_IntAndTable(lookup(t, e->u.id), t);
      break;
    // num
    case 1:
      return M_IntAndTable(e->u.num, t);
      break;
    // op
    case 2:
      itTemp1 = interpExp(e->u.op.left, t);
      itTemp2 = interpExp(e->u.op.right, t);
      switch (e->u.op.oper) {
        case 0:
          return M_IntAndTable((itTemp1->i + itTemp2->i), t);
          break;
        case 1:
          return M_IntAndTable((itTemp1->i - itTemp2->i), t);
          break;
        case 2:
          return M_IntAndTable((itTemp1->i * itTemp2->i), t);
          break;
        case 3:
          return M_IntAndTable((itTemp1->i / itTemp2->i), t);
          break;
      }
      break;
    // eseq
    case 3:
      tTemp = interpStm(e->u.eseq.stm, t);
      itTemp1 = interpExp(e->u.eseq.exp, t);
      return itTemp1;
      break;
  }
}

// Handle the interpExpList statement.
M_table interpExpList(A_expList l, M_table t) {
  // Count the argument.
  M_intAndTable itTemp1;
  // Choose the next step.
  switch (l->kind) {
    // pair
    case 0:
      itTemp1 = interpExp(l->u.pair.head, t);
      interpExpList(l->u.pair.tail, t);
      return t;
      break;
    // last
    case 1:
      itTemp1 = interpExp(l->u.last, t);
      return t;
      break;
  }
}

// Return the value of the symbol node.
int lookup(M_table t, string key) {
  while (t->tail != NULL) {
    if (t->id == key) {
      return t->value;
    }
    t = t->tail;
  }
  return 0;
}

int main(void) {
  // Instance of grammar tree
  A_stm s = prog();
  // First scanning: calculate the arguments number.
  printf("The maximum amount of arguments is %d.\n", maxargs(s));
  // Second scanning: print the symbol table.
  printf("Symbol table:\n");
  interp(s);

  return 0;
}
