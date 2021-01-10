#!/bin/sh

#cc -Os -Wall -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC -o sss sgp30_example_usage.c sgp30.c sensirion_common.c sgp_git_version.c sensirion_hw_i2c_implementation.c cm1109.c
gcc -Wall -O2 -o particle main.c sgp30.c sensirion_common.c sensirion_hw_i2c_implementation.c cm1109.c pm5000.c htu31d.c -lzmq
