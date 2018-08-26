cmake_minimum_required(VERSION 3.10)
project(sudokuProject C)

set(CMAKE_C_STANDARD 90)
add_executable(Sudoku main.c Game.h ILPSolver.c Game.c ILPSolver.h MainAux.c MainAux.h LinkedList.h LinkedList.c gurobi_c.h Solver.c Solver.h Parser.c Parser.h Stack.c Stack.h)