#!/bin/bash

gcc cnfConverter.c -o cnfConverter

cat $1 | ./cnfConverter | tee solution

echo ""
echo "result is saved in solution"
