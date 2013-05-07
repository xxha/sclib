#!/bin/sh
#grep -c return the match times.
some_var=$(grep -c "hello" ./test)
echo "var value is: ${some_var}"
