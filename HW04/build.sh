#!/bin/bash
#include <string.h>

gcc main.c weather.c jsonParser.c -o main -l:libcurl.so -l:libjson-c.so -Wall -Wextra -Wpedantic -std=c11
