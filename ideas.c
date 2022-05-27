#include <stdio.h>
#include <Stdlib.h>

void initBoard(int** board);
void freeBoard(int** board);

void main()
{
    int** board = NULL;
    int i = 0, j = 0, k = 0, flag = 0;
    initBoard(board);


    for (i = 0; i < 6; i++) 
    {
        for (j = 0; j < 6; j++)
        {
            if ((i == 0 || i == 5) && (j == 0 || j == 5)) // dont need to check corners
                continue;

            if ((i - 1 >= 0) || (i + 1 < 6 ))
                if ((board[i - 1][j] == board[i][j] || board[i - 1][j] == 0) && (board[i + 1][j] == board[i][j] || board[i - 1][j] == 0)) // check up and down
                    return 0;

            if ((j - 1 >= 0) || (j + 1 < 6))
                if ((board[i][j - 1] == board[i][j] || board[i - 1][j] == 0) && (board[i + 1][j] == board[i][j] || board[i - 1][j] == 0)) // check right and left
                    return 0;

            if (((i - 1 >= 0) && (j - 1 >= 0)) && ((i + 1 < 6) && (j + 1 < 6)))
                if ((board[i - 1][j - 1] == board[i][j] || board[i - 1][j - 1] == 0) && (board[i + 1][j + 1] == board[i][j] || board[i + 1][j + 1] == 0)) // check diagnol left to right (\) 
                    return 0;

            if (((i + 1 < 6) && (j - 1 >= 0)) && ((i - 1 >= 0) && (j + 1 < 6)))
                if ((board[i + 1][j - 1] == board[i][j] || board[i + 1][j - 1] == 0) && (board[i - 1][j + 1] == board[i][j] || board[i - 1][j + 1] == 0)) // check diagnol right to left (/) 
                    return 0;
        } 
    }
    return 1;

    
    freeBoard(board);
}

void initBoard(int** board)
{
    int i = 0, j = 0;

    board = (int**)malloc(sizeof(int*) * 6);
    for(i=0;i<6;i++)
    {
        board[i] = (int*)malloc(sizeof(int) * 6);
    }

    for(i=0;i<6;i++)
        for(j=0;j<6;j++)
        {
            board[i][j] = 0;
        }

    
}

void freeBoard(int** board)
{
    int i;
    for(i=0;i<6;i++)
        free(board[i]);
    free(board[i]);
}