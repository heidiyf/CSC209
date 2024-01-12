#!/usr/bin/env bash
./echo_arg csc209 > echo_out.txt
./echo_stdin < echo_stdin.c
./count 210 | tr -d "\n" | wc -c
ls -S | head -n 1 | ./echo_stdin
