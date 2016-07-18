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
          /* On Success Print the AST */
          mpc_ast_print(r.output);
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

