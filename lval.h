#ifndef LVAL_H_
#define LVAL_H_

/*
 * lval.h
 * 
 * Copyright (c) 2016 Rahul Syamlal
 */


/* lval types */
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };

/*
 * lval: Lisp value 
 */
typedef struct lval {
  int type;
  long num;
  /* Error and Symbol types have some string data */
  char* err;
  char* sym;
  /* Count and Pointer to a list of "lval*"; */
  int count;
  struct lval** cell;
} lval;

/* 
 * creating lval types 
 */
lval* lval_num(long x);
lval* lval_err(char* m);
lval* lval_sym(char* s);
lval* lval_sexpr(void);

/*
 * manipulating lval cells 
 */
void lval_del(lval* v);
lval* lval_add(lval* v, lval* x);
lval* lval_pop(lval* v, int i);
lval* lval_take(lval* v, int i);

/*
 * printing lvals
 */
void lval_print(lval* v);
void lval_expr_print(lval* v, char open, char close);
void lval_print(lval* v);
void lval_println(lval* v);

/* 
 * evaluating lvals
 */
lval* builtin_op(lval* a, char* op);
lval* lval_eval(lval* v);
lval* lval_eval_sexpr(lval* v);
lval* lval_eval(lval* v);

/*
 * reading lvals
 */
lval* lval_read_num(mpc_ast_t* t);
lval* lval_read(mpc_ast_t* t);

#endif
