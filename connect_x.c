#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOP 1
#define BOTTOM 0

#define HORIZONTOl 1
#define VERTICAL 0

#define LeftToRight 1
#define RightToLeft 0

typedef struct board {
	int rows;
	int cols;
	int** board;
	int x;

	int mallc, free;
}board;


int whichBigger(int, int);
void set_red();
void set_yellow();
void set_reset();
void printColor(int);
void delay(int);
void printBoard(board);
void initBoard(board*);
int dropPiece(board*, int, int);
int checkWin(board*);


void main()
{
	board board = { 0, 0, NULL, 0, 0 };
	int i = 0, j = 0;
	int col = 0, row = 0;
	int player = 1;
	int winner = 0;
	int gameOver = 0;

	int run = 1;

	do {
		do {
			printf("Please choose the height (6-9): ");
			scanf_s("%d", &(board.rows));
		} while (board.rows < 6 || board.rows > 9);

		do {
			printf("Please choose the colms (6-9): ");
			scanf_s("%d", &(board.cols));
		} while (board.cols < 6 || board.cols > 9);

		printf("Choose how much in a streak needed to win (1-%d): ", whichBigger(board.rows, board.cols));
		scanf_s("%d", &(board.x));

		board.board = (int**)malloc(sizeof(int*) * board.rows);
		board.mallc++;
		for (i = 0; i < board.rows; i++)
		{
			board.board[i] = (int*)malloc(sizeof(int) * board.cols);
			board.mallc++;
		}
		initBoard(&board);
		printBoard(board);
		gameOver = 0;

		while (!gameOver)
		{
			printf("Player %d, enter a column number (1-%d) to drop your piece: ", player, board.cols);
			scanf_s("%d", &col);

			row = dropPiece(&board, col, player);

			if (row != -1)
			{
				printBoard(board);
				winner = gameOver = checkWin(&board); // we can check also gameOver beacuse if there is no win or tie the func will return 0

				switch (winner)
				{
				case (1 || 2):
					printf("Player %d wins!\n\n", winner);
					break;
				case -1:
					printf("There is a tie\n\n");
				}

				player = (++player % 2) ? 1 : 2;
			}
		}
		for (i = 0; i < board.rows; i++)
		{
			free(board.board[i]);
			board.free++;
		}
		free(board.board);
		board.free++;
		printf("your unrealesd mallocs: %d\n\n\n", board.free - board.mallc);
		printf("would you like to play again? no(0) : yes(1);\n");
		scanf_s("%d", &run);
	} while (run);
}

int whichBigger(int height, int colms)
{
	return height > colms ? height : colms; // what is the maxiumum amount of streak possible
}

void set_red()
{
	printf("\033[0;31m");
}

void set_yellow()
{
	printf("\033[0;33m");
}

void set_reset()
{
	printf("\033[0m");
}

void printColor(int player)
{
	if (player == 1)
	{
		set_red();
	}
	else if (player == 2)
	{
		set_yellow();
	}
	printf("o");
	set_reset();
}

void delay(int milli_seconds)
{
	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds);
}

void printLine(board board, int line_postion)
{
	int i;
	//print a line, depending on the parmter line_postion ( 0 / 1 ) it will know if the line is a bottom one or a bottom
	if (line_postion == TOP) { //if line_postion true it will print the line number on top 
		for (i = 0; i < board.cols; i++)
			printf("  %d ", i + 1);
		printf("\n ");
	}

	// just print a line
	for (i = 0; i < board.cols; i++)
		line_postion ? printf("___ ") : printf("----"); // depnding on the position of the line print a diffrent kind of line

	if (line_postion == BOTTOM) // if it is a bottom line print another new line at the end
		printf("-\n");
	printf("\n");

}

void printBoard(board board)
{
	int i, j;
	printf("\n");
	printLine(board, TOP);
	for (i = 0; i < board.rows; i++)
	{
		printf("| ");
		for (j = 0; j < board.cols; j++)
		{
			printColor(board.board[i][j]);
			printf(" | ");
		}
		printf("\n");
	}
	printLine(board, BOTTOM);
}

void initBoard(board* board)
{
	// reset the board
	int i, j;

	for (i = 0; i < board->rows; i++)
	{
		for (j = 0; j < board->cols; j++)
		{
			board->board[i][j] = 0;
		}
	}
}

int dropPiece(board* board, int col, int player)
{
	int i;

	if (col <= 0 || col > board->cols) // check for invaild input
	{
		printf("Invalid column!\n");
		return -1;
	}

	for (i = board->rows - 1; i >= 0; i--) // check what is the lowest avilable place in the row
	{
		if (board->board[i][col - 1] == 0)
		{
			board->board[i][col - 1] = (player == 1) ? 1 : 2;
			return i;
		}
	}
	// if there is no place in the row tell the user the row is full
	printf("Column is full!\n");
	return -1;
}

int countCheck(board* board, int count, int* player, int i, int j)
{
	if (board->board[i][j] == 0) // if the sqaure is empty we reset every thing
		*player = count = 0;

	else if (board->board[i][j] == *player) // the streak is still going
		count++;

	else if (*player == 0) // if the square is not empty and player is then set player to the current sqare
	{
		*player = board->board[i][j];
		count = 1;
	}
	else // reset the count if the streak was broken
	{
		*player = board->board[i][j];
		count = 1;
	}
	return count;

}

int checkStrightWin(board* board, int line)
{
	int i, j, count, player;
	for (i = 0, player = 0, count = 0; i < board->rows; i++)
	{
		for (j = 0; j < board->cols; j++)
		{
			if (line == HORIZONTOl) // check if right now were searching for a horizontol win or a vertical
				count = countCheck(board, count, &player, i, j);
			else if (line == VERTICAL)
				count = countCheck(board, count, &player, j, i);
			if (count == board->x)
				return player;
		}
	}
	return 0;
}

int checkDiagonalWin(board* board, int diagonal)
{
	//This function checks for a diagonal win. It loops through the board and checks if there are 'x' number of the same values in a row. If there are, it returns the value. If not, it returns 0.
	int i, j, k, count;
	// left to right

	for (i = board->x - 1; i < board->rows; i++) // the starting row should be the x, less then that can not be a possible win
		for (j = 0; j < board->cols; j++)
			for (count = 1, k = 1; k <= board->x; k++) {

				if (diagonal ? i - k >= 0 && j + k < board->cols : i + k < board->rows && j + k < board->rows) // to long to read on the for condition
					break; // for example: diagonl = LeftToRight(1), then check the conditon that fits onto that parmter

				if (board->board[i][j] == board->board[diagonal ? i - k: i + k][j + k] && board->board[i][j] != 0)
					count++;
				else
					break;

				if (count == board->x)
					return board->board[i][j];
			}
	return 0;
}

int checkTie(board* board)
{
	int i, j;
	// run trought all the board, if one of the values is not taken there is no tie
	for (i = 0; i < board->rows; i++)
	{
		for (j = 0; j < board->cols; j++)
		{
			if (!board->board[i][j])
			{
				return 0;
			}
		}
	}
	return 1;
}

int checkWin(board* board)
{
	int player = 0;
	// beacuse the check win functions return the winning player, firstly we need to check that someone actully won and then return what player it was.
	// we can't just return the player with || or that kind of varisoin beacuse then it will return only true or false
	if (player = (checkStrightWin(board, HORIZONTOl)))
		return player; // Check for horizontol wins
	else if (player = (checkStrightWin(board, VERTICAL)))
		return player; // Check for vertical wins
	else if (player = (checkDiagonalWin(board, LeftToRight)))
		return player; // Check for diagnol left to right wins
	else if (player = (checkDiagonalWin(board, RightToLeft)))
		return player; // Check for diagnol right to left wins
	else if (checkTie(board))
		return -1;
	/*
	chart for returned values:
		-1 - a tie
		0 - no win and not tie
		1 - win
	*/
	return 0;
}