#!/bin/zsh
 cc -std=c99 -g -Wall prompt.c lval.c mpc/mpc.c -ledit -lm -o rah 
