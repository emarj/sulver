//
//Copyright 2011 Marco Ronchese | http://ronche.se
//License MIT: http://www.opensource.org/licenses/MIT
//

#ifndef CSudokuSolver_SudokuSolver_h
#define CSudokuSolver_SudokuSolver_h

#include "common.h"

#include "SudokuModel.h"

void checkRow(int row,int column,Sudoku *s,int *result);
void checkCols(int row,int column,Sudoku *s,int *result);
void checkSquare(int row,int column,Sudoku *s,int *result);

bool checkCell(int row,int column, Sudoku *s,bool readonly);


int checkRowUnicity(int row,int column,Sudoku *s);
int checkColsUnicity(int row,int column,Sudoku *s);
int checkSquareUnicity(int row,int column,Sudoku *s);

void checkCellUnicity(int row,int column, Sudoku *s);

int isValid(Sudoku *s);
int forceSolve(Sudoku *s,int level);

int solve(Sudoku *s,bool echo);

#endif
