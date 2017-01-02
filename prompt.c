/*
 * Rah: A simple LISP in C
 *
 * Based on buildyourownlisp.com by Daniel Holden
 * *nix implementation
 *
 * Created: 17-Jul-2016
 *
 * Copyright (c) 2016 Rahul Syamlal
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mpc/mpc.h"
#include "lval.h"

#include <editline/readline.h>

/* Use operator string to see which operation to perform */
long eval_op(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  if (strcmp(op, "%") == 0) { return x % y; }
  if (strcmp(op, "^") == 0) { return pow((double) x, (double) y); }
  return 0;
}

int numOfLeaves(mpc_ast_t* t){
  if(t->children_num == 0){
      return 1;
  } 

  int total = 0;

  for(int i = 0; i < t->children_num; i++){
    total += numOfLeaves(t->children[i]);
  }

  return total;
}

/* Helper method to display debug info */
void print_debug_info(mpc_result_t r){
    mpc_ast_print(r.output);
    int num = numOfLeaves(r.output);
    printf("Number of leaves: %i\n", num);
}

int main(int argc, char **argv) {
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Symbol = mpc_new("symbol");
	mpc_parser_t* Sexpr  = mpc_new("sexpr");
	mpc_parser_t* Expr   = mpc_new("expr");
	mpc_parser_t* Rah  = mpc_new("rah");

	mpca_lang(MPCA_LANG_DEFAULT,
		"                                        \
		number : /-?[0-9]+/ ;                    \
		symbol : '+' | '-' | '*' | '/' | '%' | '^' ;   \
		sexpr  : '(' <expr>* ')' ;               \
		expr   : <number> | <symbol> | <sexpr> ; \
		rah  : /^/ <expr>* /$/ ;                 \
		",
		Number, Symbol, Sexpr, Expr, Rah);

    /* Print Version and Exit Information */
    puts("Rah Version 0.0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

	while (1) {
		char* input = readline("rah> ");
		add_history(input);

		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Rah, &r)) {
		  lval* x = lval_eval(lval_read(r.output));
		  lval_println(x);
		  lval_del(x);
		  mpc_ast_delete(r.output);
		} else {    
		  mpc_err_print(r.error);
		  mpc_err_delete(r.error);
		}

		free(input);
	}
  
    mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Rah);

    return 0;
}

