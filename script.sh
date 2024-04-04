#!/bin/bash
#This is for Q4

echo "Arguments received by script.sh:"
for arg in "$@"; do
    echo "$arg"
done

echo "Environment variables in script.sh:"
env

