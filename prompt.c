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

#include <editline/readline.h>

int main(int argc, char **argv) {
   
  /* Print Version and Exit Information */
  puts("Rah Version 0.0.0.0.1");
  puts("Press Ctrl+c to Exit\n");
   
  /* In a never ending loop */
  while (1) {
    
    /* Output our prompt and get input */
    char *input = readline("rah> ");
    
    /* Add input to history */
    add_history(input);
    
    /* Echo input back to user */    
    printf("No you're a %s\n", input);

    /* Free retrieved input */
    free(input);
    
  }
  
  return 0;
}

