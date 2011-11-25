//
// Copyright 2011 Marco Ronchese | http://ronche.se
// License MIT: http://www.opensource.org/licenses/MIT
//

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "common.h"

#include "SudokuModel.h"
#include "SudokuSolver.h"

#define MAX 100

int main(int argc, char **argv)
{
    FILE *fp;
    Sudoku sudoku, solution;
    int forced = 0;

    int i;
    bool fileExists = false;

    if (argc != 2)
    {
        printf("Usage: sulver <sudoku file>");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    if (!(fileExists = (fp = fopen(filename, "r"))))
    {
        printf("\nThe file '%s'  doesn't exists!\n", filename);
        return EXIT_FAILURE;
    }

    int loaded = load(fp, &sudoku);
    if (!loaded)
    {
        fprintf(stdout, "Unable to load sudoku!");
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);
    fprintf(stdout, "\nSudoku loaded from \'%s\':\n\n", filename);
    save(stdout, &sudoku, 0);

    fprintf(stdout, "\nChecking coherence... ");
    int valid = isValid(&sudoku);
    if (!valid)
    {
        fprintf(stdout, "\nNOT VALID");
        return EXIT_SUCCESS;
    }
    fprintf(stdout, "VALID\n");

    solution = sudoku;

    int solved = solve(&solution, true);

    if (!solved)
    {
        printf("\n\nThis sudoku is hard...\n\nUsing brute force...\n");
        forced = true;
        solved = forceSolve(&solution, 10);

        if (!solved)
        {
            fprintf(stderr, "\nUnable to solve it!\n");
            return EXIT_SUCCESS;
        }
    }

    fprintf(stdout, "\n\n*****SOLVED!*****\n\n");

    print(&solution, 0);
}
