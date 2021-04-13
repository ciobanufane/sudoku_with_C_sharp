#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#define SUDOKU_EXPORTS
#include "sudoku.h"

// Implementation of Sudoku board using the BoardCell struct

// Prints the board with its curretn values
void printBoardC(BoardCell * board, int boardLength) {
    BoardCell* bCur = board;
    for (int row = 0; row < boardLength; ++row) {
        std::cout << "-------------------------------------" << '\n';
        std::cout << "| ";
        for (int col = 0; col < boardLength; ++col) {
            std::cout << bCur->currentValue << " | ";
            bCur++;
        }
        std::cout << '\n';
    }
    std::cout << "-------------------------------------" << '\n';
}

// Prints the board with its original values (initial state)
void printBoardO(BoardCell* board, int boardLength) {
    BoardCell* bCur = board;
    for (int row = 0; row < boardLength; ++row) {
        std::cout << "-------------------------------------" << '\n';
        std::cout << "| ";
        for (int col = 0; col < boardLength; ++col) {
            std::cout << bCur->originalValue << " | ";
            bCur++;
        }
        std::cout << '\n';
    }
    std::cout << "-------------------------------------" << '\n';
}

// Prints the board with its solved values (solved state)
void printBoardS(BoardCell* board, int boardLength) {
    BoardCell* bCur = board;
    for (int row = 0; row < boardLength; ++row) {
        std::cout << "-------------------------------------" << '\n';
        std::cout << "| ";
        for (int col = 0; col < boardLength; ++col) {
            std::cout << bCur->solutionValue << " | ";
            bCur++;
        }
        std::cout << '\n';
    }
    std::cout << "-------------------------------------" << '\n';
}

// Checks whether the current number at a board spot satisfies the Sudoku row conditions
bool isValidRowInsert(SudokuBoard board, int boardLength, int x, int y, int assignedValue) {
    for (int col = 0; col < boardLength; ++col) {
        if (col != y && board[x * boardLength + col].currentValue == assignedValue) {
            return false;
        }
    }
    return true;
}

// Checks whether the current number at a board spot satisfies the Sudoku column conditions
bool isValidColInsert(SudokuBoard board, int boardLength, int x, int y, int assignedValue) {
    for (int row = 0; row < boardLength; ++row) {
        if (row != x && board[row * boardLength + y].currentValue == assignedValue) {
            return false;
        }
    }
    return true;
}

// Checks whether the current number at a board spot satisfies the Sudoku box conditions
bool isValidBoxInsert(SudokuBoard board, int boxLength, int x, int y, int assignedValue) {
    int boardLength = boxLength * boxLength;
    // division operation needs to happen first to round down
    int topLeftX = (x / boxLength) * boxLength;
    int topLeftY = (y / boxLength) * boxLength;

    for (int row = 0; row < boxLength; ++row) {
        for (int col = 0; col < boxLength; ++col) {
            if (topLeftX + row != x && topLeftY + col != y && board[(topLeftX + row) * boardLength + topLeftY + col].currentValue == assignedValue) {
                return false;
            }
        }
    }
    return true;
}

// Checks whether the current number at a board spot is valid
bool isValidInsert(SudokuBoard board, int boxLength, int boardLength, int x, int y, int assignedValue) {

    if (assignedValue < 1 || assignedValue > boardLength) {
        return false;
    }

    return isValidRowInsert(board, boardLength, x, y, assignedValue) &&
        isValidColInsert(board, boardLength, x, y, assignedValue) &&
        isValidBoxInsert(board, boxLength, x, y, assignedValue);
}

// Solves a given Sudoku board
// Can be used to populate a Sudoku board
// Can be used to test the uniqueness of a Sudoko if the uniqueFlag is set to true
SUDOKU_API bool solveBoard(SudokuBoard board, int boxLength, int boardPosition, int& solutions, bool uniqueFlag) {

    if (boxLength <= 0) {
        return false;
    }

    int boardLength = boxLength * boxLength;
    if (boardPosition < 0 || boardPosition > boardLength * boardLength) {
        return false;
    }

    // Function recursions ends when it reaches the end of the board+1
    // IE boardPosition = 81
    if (boardPosition == boardLength * boardLength) {
        ++solutions;

        // Setting the uniqueFlag to true will make the function explore all possibilities ofa board 
        // If the uniqueFlag is set to true, then the board will not be modified
        // The solutions parameter will return how many possible solutions to a sudoku
        return !uniqueFlag;
    }

    int boardX = boardPosition / boardLength;
    int boardY = boardPosition % boardLength;

    std::vector<int> numList;
    for (int i = 1; i <= boardLength; ++i) {
        numList.push_back(i);
    }

    if (board[boardPosition].currentValue == 0) {
        // shuffled vector of numbers ie, 1-9, so that the function can be used to populate a board
        std::random_shuffle(numList.begin(), numList.end());

        for (unsigned int index = 0; index < numList.size(); ++index) {
            board[boardPosition].currentValue = numList[index];
            if (isValidInsert(board, boxLength, boardLength, boardX, boardY, numList[index])) {
                if (solveBoard(board, boxLength, boardPosition + 1, solutions, uniqueFlag)) {
                    board[boardPosition].originalValue = board[boardPosition].currentValue;
                    board[boardPosition].solutionValue = board[boardPosition].currentValue;
                    return true;
                }
            }
            board[boardPosition].currentValue = 0;
        }
        return false;
    }
    else {
        if (isValidInsert(board, boxLength, boardLength, boardX, boardY, board[boardPosition].currentValue)) {
            board[boardPosition].originalValue = board[boardPosition].currentValue;
            board[boardPosition].solutionValue = board[boardPosition].currentValue;
            return solveBoard(board, boxLength, boardPosition + 1, solutions, uniqueFlag);
        }
        return false;
    }
}

// Populates the entire board with a fully solved Sudoku
SUDOKU_API bool populateBoard(SudokuBoard board, int boxLength) {

    if (boxLength <= 0) {
        return false;
    }

    std::srand(static_cast<unsigned int>(std::time(0)));

    int boardLength = boxLength * boxLength;
    int solutions = 0;
    for (int row = 0; row < boardLength; ++row) {
        for (int col = 0; col < boardLength; ++col) {
            board[row * boardLength + col].currentValue = 0;
        }
    }
    return solveBoard(board, boxLength, 0, solutions, false);
}

// Populates the board and removes numbers
SUDOKU_API bool createBoard(SudokuBoard board, int boxLength, int pairCluesRemoved) {

    // Probably should redefine this function to remove clues only
    // Duplicate reponsibilities with populateBoard function

    if (boxLength <= 0 || pairCluesRemoved < 0) {
        return false;
    }

    int boardLength = boxLength * boxLength;
    if (boardLength * boardLength - 2 * pairCluesRemoved < MIN_CLUES) {
        return false;
    }

    if (!populateBoard(board, boxLength)) {
        return false;
    }

    int boardPos, oppositeBoardPos;

    /*  Possible that removing clues will result in the board not having a unique solution
        Chances of that happening are low
        Checking the board for a unique solution increases the time to create a board so not worth it
    */ 
    while (pairCluesRemoved > 0) {
        boardPos = rand() % (boardLength * boardLength);

        if (board[boardPos].currentValue != 0) {
            oppositeBoardPos = boardLength * boardLength - boardPos - 1;

            board[boardPos].originalValue = 0;
            board[oppositeBoardPos].originalValue = 0;

            board[boardPos].currentValue = 0;
            board[oppositeBoardPos].currentValue = 0;
            --pairCluesRemoved;
        }
    }
    return true;
}

// Resets the board to its initial state
SUDOKU_API bool resetBoard(SudokuBoard board, int boxLength) {
    int boardLength{ boxLength * boxLength };
    int boardPosition;
    for (int row = 0; row < boardLength; ++row) {
        for (int col = 0; col < boardLength; ++col) {
            boardPosition = row * boardLength + col;
            board[boardPosition].currentValue = board[boardPosition].originalValue;
        }
    }
    return true;
}

// Checks if the board is fully solved
SUDOKU_API bool checkBoard(SudokuBoard board, int boxLength) {
    int boardLength{ boxLength * boxLength };
    int boardPosition;
    for (int row = 0; row < boardLength; ++row) {
        for (int col = 0; col < boardLength; ++col) {
            boardPosition = row * boardLength + col;

            /*
                function only checks if current value is equal to solution value
                if the board has multiple solutions as mentioned in createBoard function, then result in an error for user
                better idea to use solveBoard function, but not worry about it for now
            */
            if (board[boardPosition].currentValue != board[boardPosition].solutionValue) {
                return false;
            }
        }
    }
    return true;
}

/*
    int main() {
    SudokuBoard board = new BoardCell[81];
    createBoard(board, 3, 2);
    printBoardC(board, 9);
    delete[] board;
    return 0;
}*/
