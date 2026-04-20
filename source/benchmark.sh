#!/bin/bash

# 1. Компилируем версию без оптимизаций
echo "Compiling version without command line optimizations..."
g++ ./source/euler.cpp -o ./bin/euler_no_optimizations

# 2. Компилируем версию с оптимизациями
echo "Compiling version with command line optimizations..."
g++ -O3 -march=native -ffast-math ./source/euler.cpp -o ./bin/euler

# 3. Запуск и замер времени для первой версии
echo "---------------------------------------"
echo "Running euler_no_optimizations:"
./bin/euler_no_optimizations

# 4. Запуск и замер времени для второй версии
echo "---------------------------------------"
echo "Running euler:"
./bin/euler