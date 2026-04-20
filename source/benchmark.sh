#!/bin/bash

echo "---------------------------------------"
echo "Running euler_no_optimizations:"
./bin/euler_no_optimization

echo "---------------------------------------"
echo "Running euler:"
./bin/euler

echo "---------------------------------------"
echo "Running euler with OpenMP:"
./bin/euler_OpenMP