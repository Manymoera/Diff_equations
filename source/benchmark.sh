#!/bin/bash

echo "---------------------------------------"
echo "Running euler_no_optimizations:"
./bin/euler_no_optimization

echo "---------------------------------------"
echo "Running euler_O3:"
./bin/euler_O3

echo "---------------------------------------"
echo "Running euler_O3_native_march:"
./bin/euler_O3_native_march

echo "---------------------------------------"
echo "Running euler_O3_native_march_ffast:"
./bin/euler_O3_native_march_ffast

echo "---------------------------------------"
echo "Running euler with OpenMP:"
./bin/euler_OpenMP