#pragma once

typedef struct BoardCell {
	int originalValue;
	int currentValue;
	int solutionValue;
} BoardCell, * PBoardCell;

using SudokuBoard = BoardCell*;
constexpr int MIN_CLUES = 17;

void printBoard(SudokuBoard board, int boardLength);
bool isValidRowInsert(SudokuBoard board, int boardLength, int x, int y, int assignedValue);
bool isValidColInsert(SudokuBoard board, int boardLength, int x, int y, int assignedValue);
bool isValidBoxInsert(SudokuBoard board, int boxLength, int x, int y, int assignedValue);
bool isValidInsert(SudokuBoard board, int boxLength, int boardLength, int x, int y, int assignedValue);

#ifdef SUDOKU_EXPORTS
#define SUDOKU_API __declspec(dllexport)
#else
#define SUDOKU_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	SUDOKU_API bool solveBoard(SudokuBoard board, int boxLength, int boardPosition, int& solutions, bool uniqueFlag);
	SUDOKU_API bool populateBoard(SudokuBoard board, int boxLength);
	SUDOKU_API bool createBoard(SudokuBoard board, int boxLength, int pairCluesRemoved);
	SUDOKU_API bool resetBoard(SudokuBoard board, int boxLength);
	SUDOKU_API bool checkBoard(SudokuBoard board, int boxLength);

#ifdef __cplusplus
}
#endif