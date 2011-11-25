//
// Copyright 2011 Marco Ronchese | http://ronche.se
// License MIT: http://www.opensource.org/licenses/MIT
//

#ifndef CSudokuSolver_SudokuModel_h
#define CSudokuSolver_SudokuModel_h

#include "common.h"

#define DIM 9
#define DEPTH 9

typedef struct
{
    int value;             // value
    int possValues[DEPTH]; // possible values flags
    int adValues[DEPTH];
    int nPossValues;
    int row;
    int col;
} SCell;

typedef struct
{
    SCell cells[DIM][DIM];
    int nSolved;
} Sudoku;

int compareCell(const void *a, const void *b);

int getPossValues(SCell *cell, int *possValues);

int getEmptyCells(Sudoku *s, SCell *result);

void setPossValues(Sudoku *s);

void init(Sudoku *s);

void cp(Sudoku *from, Sudoku *to);

bool cmp(Sudoku *s1, Sudoku *s2);

bool save(FILE *stream, Sudoku *s, bool showValues);

void print(Sudoku *s, bool showValues);

int load(FILE *stream, Sudoku *s);

#endif
