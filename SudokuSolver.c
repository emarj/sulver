//
// Copyright 2011 Marco Ronchese | http://ronche.se
// License MIT: http://www.opensource.org/licenses/MIT
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#include "SudokuModel.h"
#include "SudokuSolver.h"

#include "main.h"

#ifndef DEBUG
#define DEBUG 0
#endif

void checkRow(int row, int column, Sudoku *s, int *result)
{
    // if(DEBUG) fprintf(stderr,"\n\tcontrollo la riga...");
    int i;
    for (i = 0; i < DIM; ++i) // itero le colonne per trovare le celle della riga
    {
        // i valori trovati nella cella non sono più ammissibili
        if (i != column && s->cells[row][i].value != 0)
        {
            // if(DEBUG) fprintf(stderr,"\n\t\t!! %d",s->cells[row][i].value);
            result[s->cells[row][i].value - 1] = 0;
        }
    }
}

void checkCols(int row, int column, Sudoku *s, int *result)
{
    // if(DEBUG) fprintf(stderr,"\n\tcontrollo la colonna...");
    int i;
    for (i = 0; i < DIM; ++i) // itero le righe per trovare le celle della colonna
    {
        if (i != row && s->cells[i][column].value != 0)
        {

            // if(DEBUG) fprintf(stderr,"\n\t\t!! %d",s->cells[i][column].value);
            result[s->cells[i][column].value - 1] = 0;
        }
    }
}

void checkSquare(int row, int column, Sudoku *s, int *result)
{
    // if(DEBUG) fprintf(stderr,"\n\tcontrollo il quadrato...");

    int n = row / 3;    // numero della "riga" del quadrato
    int m = column / 3; // numero della "colonna" del quadrato

    int i, j, _i, _j;
    for (i = 0; i < DIM / 3; ++i)
    {
        for (j = 0; j < DIM / 3; ++j)
        {
            _i = i + (n * 3);
            _j = j + (m * 3);

            //(_j != column || _i != row) ||
            if ((_j != column || _i != row) && s->cells[_i][_j].value != 0)
            {

                // if(DEBUG) fprintf(stderr,"\n\t\t!! %d",s->cells[_i][_j].value);
                result[s->cells[_i][_j].value - 1] = 0;
            }
        }
    }
}

/*
 * The function can be used in 2 modes. With flag mode
 */

bool checkCell(int row, int column, Sudoku *s, bool readonly)
{
    int values[DEPTH];
    int k;
    for (k = 0; k < DEPTH; ++k)
        values[k] = 1;

    checkRow(row, column, s, values);
    checkCols(row, column, s, values);
    checkSquare(row, column, s, values);

    int i, allowed = 0, value;

    if (!readonly) // se la cella è vuota inserisce in numerini, altrimenti controlla la coerenza
    {
        if (s->cells[row][column].value == 0)
        {
            for (i = 0; i < DEPTH; ++i)
            {
                // if(values[i] == 1) if(DEBUG) fprintf(stderr,"\t %d ok",i+1);
                s->cells[row][column].possValues[i] = values[i]; // se il numero è sulla riga, sulla colonna o sul quadrato, non è ammissibile

                if (s->cells[row][column].possValues[i]) // se è un numero ammissibile
                {

                    ++allowed;
                    value = i + 1;
                }
            }

            if (allowed == 1 && s->cells[row][column].value == 0)
            {
                // if(DEBUG) fprintf(stderr,"\nInserisco %d nella cella (%d,%d)\n",value,row,column);
                s->cells[row][column].value = value;
            }
        }
        else
        {
            for (i = 0; i < DEPTH; ++i)
            {
                s->cells[row][column].possValues[i] = 0;
            }

            s->cells[row][column].possValues[s->cells[row][column].value - 1] = 1;
        }
    }
    else
    {
        if (values[s->cells[row][column].value - 1] == 0)
            return false;
        else
            return true;
    }
}

int checkRowUnicity(int row, int column, Sudoku *s)
{
    // if(DEBUG) fprintf(stderr,"\n\tcontrollo unicita\' sulla riga...");

    int unicity;

    int i, k;

    for (k = 0; k < DEPTH; ++k)
    {
        if (s->cells[row][column].possValues[k] == 1 && s->cells[row][column].value == 0)
        {
            unicity = 1;
            for (i = 0; i < DIM && unicity; ++i)
            {
                if (s->cells[row][i].possValues[k] == s->cells[row][column].possValues[k] && s->cells[row][i].value == 0)
                {
                    unicity = 0;
                    break;
                }
            }
        }
        else
        {
            unicity = 0;
            continue;
        }

        if (unicity == 1)
        {
            // if(DEBUG) fprintf(stderr,"\nIl numero %d e\' unico sulla riga\n",k+1);
            s->cells[row][column].value = (k + 1);
        }
    }
}

int checkColsUnicity(int row, int column, Sudoku *s)
{
    // if(DEBUG) fprintf(stderr,"\n\tcontrollo unicita\' sulla colonna...");

    int unicity;

    int i, k;

    for (k = 0; k < DEPTH; ++k)
    {
        if (s->cells[row][column].possValues[k] == 1 && s->cells[row][column].value == 0)
        {
            unicity = 1;
            for (i = 0; i < DIM && unicity; ++i)
            {
                if (s->cells[i][column].possValues[k] == s->cells[row][column].possValues[k] && s->cells[i][column].value == 0)
                {
                    unicity = 0;
                    break;
                }
            }
        }
        else
        {
            unicity = 0;
            continue;
        }

        if (unicity == 1)
        {
            // if(DEBUG) fprintf(stderr,"\nIl numero %d e\' unico sulla colonna\n",k+1);
            s->cells[row][column].value = (k + 1);
        }
    }
}

int checkSquareUnicity(int row, int column, Sudoku *s)
{
    // if(DEBUG) fprintf(stderr,"\n\tcontrollo unicita\' sul quadrato...");
    int n = row / 3;    // numero della "riga" del quadrato
    int m = column / 3; // numero della "colonna" del quadrato

    // if(DEBUG) fprintf(stderr,"\n\t\tla casella si trova sul quadrato riga=%d,col=%d",n,m);

    int unicity;

    int i, j, k, _i, _j;

    for (k = 0; k < DEPTH; ++k)
    {
        if (s->cells[row][column].possValues[k] == 1 && s->cells[row][column].value == 0)
        {

            unicity = 1;
            for (i = 0; i < DIM / 3 && unicity; ++i)
            {
                for (j = 0; j < DIM / 3; ++j)
                {
                    _i = i + (n * 3);
                    _j = j + (m * 3);

                    if ((_j != column || _i != row) && s->cells[_i][_j].possValues[k] == 1 && s->cells[_i][_j].value == 0)
                    {

                        unicity = 0;
                        break;
                    }
                }
            }
        }
        else
        {
            unicity = 0;
            continue;
        }

        if (unicity == 1)
        {
            // if(DEBUG) fprintf(stderr,"\nIl numero %d e\' unico sul quadrato\n, lo inserisco nella cella (%d,%d)",k+1,row+1,column+1);
            s->cells[row][column].value = (k + 1);
        }
    }
}

void checkCellUnicity(int row, int column, Sudoku *s)
{

    checkRowUnicity(row, column, s);
    checkColsUnicity(row, column, s);
    checkSquareUnicity(row, column, s);
}

int isValid(Sudoku *s)
{
    int i, j;

    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            // se la cella è piena controlla che sia coerente
            if (s->cells[i][j].value != 0)
            {
                if (!checkCell(i, j, s, true))
                    return false;
            }
        }
    }

    return true;
}

int forceSolve(Sudoku *s, int rLevel)
{
    int i, k, solved;

    SCell nullcell;
    nullcell.nPossValues = 0;
    SCell cells[81];
    for (k = 0; k < 81; ++k)
        cells[k] = nullcell;

    int nCells = getEmptyCells(s, cells);

    if (DEBUG)
        fprintf(stderr, "\n\nForce solving...\n\n");

    Sudoku clone;
    init(&clone);
    cp(s, &clone);

    for (i = 0; i < nCells; ++i)
    {
        solved = 0;

        if (DEBUG && i == 0)
            fprintf(stderr, "\n***** Vi sono %d valori possibili\n", cells[i].nPossValues);

        for (k = 0; k < cells[i].nPossValues && !solved; ++k)
        {

            if (DEBUG)
                fprintf(stderr, "\nProvo con il valore %d\n", cells[i].adValues[k]);
            clone.cells[cells[i].row][cells[i].col].value = cells[i].adValues[k];

            if (!(solved = solve(&clone, false)) && (rLevel > 0))
            {
                solved = forceSolve(&clone, rLevel - 1);
            }
        }

        if (solved)
        {
            cp(&clone, s);
            return SOLVED;
        }
        else
        {
            cp(s, &clone);
        }
    }

    return 0;
}

int solve(Sudoku *s, bool echo)
{
    int i, j, n, nSolvedCells = 0, stalled = 0;

    Sudoku clone;

    init(&clone);

    if (DEBUG)
        fprintf(stderr, "\nSolving...\n");

    for (n = 1; n <= 100; ++n)
    {

        // if(DEBUG) fprintf(stderr,"\n****ciclo %d****\n",n);

        nSolvedCells = 0;
        stalled = cmp(&clone, s);

        cp(s, &clone);

        if (stalled)
        {

            if (DEBUG)
                fprintf(stdout, "Checking unicity\n");
            for (i = 0; i < DIM; ++i)
            {
                for (j = 0; j < DIM; ++j)
                {
                    if (s->cells[i][j].value == 0)
                        checkCellUnicity(i, j, s);
                }
            }

            if (cmp(&clone, s))
            {
                if (DEBUG)
                    fprintf(stderr, "\n--stalled--\n");
                // if(echo) printf("\n\n%.3f solved\n\n", ((double) nSolvedCells)/((double) DIM*DIM));
                return NOT_SOLVED;
            }
        }

        for (i = 0; i < DIM; ++i)
        {
            for (j = 0; j < DIM; ++j)
            {
                checkCell(i, j, s, false);

                if (s->cells[i][j].value != 0)
                    ++nSolvedCells;
            }
        }

        if (nSolvedCells == (DIM * DIM))
        {
            return SOLVED; // The sudoku is solved
        }
    }

    // if(echo) printf("\n\n%.3f solved\n\n", ((double) nSolvedCells)/((double) DIM*DIM));

    return NOT_SOLVED;
}
