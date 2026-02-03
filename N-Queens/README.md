# N-Queens Problem

The N-Queens problem consists of placing Nqueens on an N×Nchessboard such that no two queens attack
each other. This includes conflicts along rows, columns, and diagonals. For N= 8, the problem has exactly
92 valid solutions

## Multithreading Solutions:
### In this repo, we implemented a solution of the problem taking advantage of parallel computing, using p_threads or OpenMP

### gcc flag to find header file: -I.. queens_omp.c ../safe.c ../board.c