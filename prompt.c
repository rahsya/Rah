/**
 * Rah: A simple LISP in C
 *
 * Based on buildyourownlisp.com by Daniel Holden
 *
 * *nix implementation
 *
 * Created: 17-Jul-2016
 * By: Rahul Syamlal
 *
 * Copyright 2016
 */

#include <stdio.h>
#include <stdlib.h>

#include "mpc/mpc.h"

#include <editline/readline.h>

/* Use operator string to see which operation to perform */
long eval_op(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  return 0;
}

long eval(mpc_ast_t* t) {

  /* If tagged as number return it directly. */ 
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }

  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` */
  long x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;  
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
    /* Create Some Parsers */
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Rah    = mpc_new("rah");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                     \
            number   : /-?[0-9]+/ ;                           \
            operator : '+' | '-' | '*' | '/' ;                \
            expr     : <number> | '(' <operator> <expr>+ ')' ;\
            rah      : /^/ <operator> <expr>+ /$/ ;           \
        ",
        Number, Operator, Expr, Rah);

    /* Print Version and Exit Information */
    puts("Rah Version 0.0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    /* In a never ending loop */
    while (1) {

        /* Output our prompt and get input */
        char *input = readline("rah> ");

        /* Add input to history */
        add_history(input);

        /* Attempt to Parse the user Input */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Rah, &r)) {
            long result = eval(r.output);
            printf("%li\n", result);
            print_debug_info(r);
            mpc_ast_delete(r.output);
        } else {
          /* Otherwise Print the Error */
          mpc_err_print(r.error);
          mpc_err_delete(r.error);
        }

        /* Free retrieved input */
        free(input);
    }

    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Number, Operator, Expr, Rah);

    return 0;
}

