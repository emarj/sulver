//
// Copyright 2011 Marco Ronchese | http://ronche.se
// License MIT: http://www.opensource.org/licenses/MIT
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "SudokuModel.h"

int ctoi(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    else
        return -1;
}

/*
 * Used with qsort, sort remaining cell for ascending numbers of admitted values
 */

int compareCell(const void *a, const void *b)
{
    SCell *da = (SCell *)a;
    SCell *db = (SCell *)b;

    return ((da)->nPossValues > (db)->nPossValues) - ((da)->nPossValues < (db)->nPossValues);
}

/*
 * Write in the possValues address the list of all possible values, terminated with a -1 (EOF). Return the numbers of value;
 */

int getPossValues(SCell *cell, int *possValues)
{
    int i, j = 0;
    for (i = 0; i < DEPTH; ++i)
    {
        if (cell->possValues[i])
        {
            possValues[j++] = i + 1; // write possValues in position j the numbers i+1, then increase the variable j
        }
    }

    possValues[j] = EOF; // set the delimiter that sign the end of string

    return j;
}

int getEmptyCells(Sudoku *s, SCell *result)
{
    int i, j, k, t = 0;

    for (i = 0; i < DIM; ++i)
        for (j = 0; j < DIM; ++j)
        {
            s->cells[i][j].row = i;
            s->cells[i][j].col = j;

            for (k = 0; k < DEPTH; ++k)
                s->cells[i][j].adValues[k] = 0;

            s->cells[i][j].nPossValues = 0;
            for (k = 0; k < DEPTH; ++k)
            {
                if (s->cells[i][j].possValues[k] == 1)
                {
                    s->cells[i][j].adValues[(s->cells[i][j].nPossValues)++] = k + 1;
                }
            }

            if (s->cells[i][j].value == 0)
            {
                result[t++] = s->cells[i][j];
            }
        }

    qsort(result, t, sizeof(result[0]), compareCell);

    return t;
}

/*
 * Fill possible numbers flag array for every cell
 */

void setPossValues(Sudoku *s)
{
    int i, j, k;
    SCell cell;

    for (i = 0; i < DIM; ++i)
        for (j = 0; j < DIM; ++j)
        {
            cell = s->cells[i][j]; // local variable for incresing performance (?)

            for (k = 0; k < DEPTH; ++k)
            {
                cell.possValues[k] = (cell.value == 0); // set all flags to true if is empty, otherwise false
            }
        }
}

void init(Sudoku *s)
{
    int i, j;
    for (i = 0; i < DIM; ++i)
        for (j = 0; j < DIM; ++j)
        {
            s->cells[i][j].value = 0;
        }

    setPossValues(s);
}

void cp(Sudoku *from, Sudoku *to)
{
    int i, j, k;
    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            to->cells[i][j].value = from->cells[i][j].value;
            for (k = 0; k < DEPTH; ++k)
            {
                to->cells[i][j].possValues[k] = from->cells[i][j].possValues[k];
            }
        }
    }
}

bool cmp(Sudoku *s1, Sudoku *s2)
{

    int i, j, k;
    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            if (s1->cells[i][j].value != s2->cells[i][j].value)
                return false;
            else
            {
                for (k = 0; k < DEPTH; ++k)
                {
                    if (s1->cells[i][j].possValues[k] != s2->cells[i][j].possValues[k])
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true; // s1 is equal to s2
}

bool save(FILE *stream, Sudoku *s, bool showValues)
{
    int i, j, k;

    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fprintf(stream, "%d", s->cells[i][j].value);

            if (showValues)
            {
                for (k = 0; k < DEPTH; ++k)
                {
                    if (s->cells[i][j].possValues[k])
                        fprintf(stream, "%d", k + 1);
                }
            }

            if (j == DIM - 1)
                fprintf(stream, "\n");
            else
                fprintf(stream, " ");
        }
    }

    return true;
}

void print(Sudoku *s, bool showValues)
{
    save(stdout, s, showValues);
}

int load(FILE *stream, Sudoku *s)
{
    char lineBuffer[255];

    int i, j, n;
    int row = 0;

    while (fgets(lineBuffer, 125, stream) != 0)
    {
        for (i = 0, j = 0; i <= strlen(lineBuffer); ++i)
        {
            if ((n = ctoi(lineBuffer[i])) != -1)
            {
                s->cells[row][j].value = n;
                ++j;
            }
        }

        ++row;
    }

    setPossValues(s);
    return 1;
}
