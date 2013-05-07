#!/bin/bash

find $1 -name "*.h" -o -name "*.c" -o -name "*.cpp" -o -name "*.java" > files 
cscope -bkq -i files 
ctags -R 
