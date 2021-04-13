#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using SudokuBoard = int*;
constexpr int MIN_CLUES = 17;

void printBoard(SudokuBoard board, int boardLength) {
    for (int row = 0; row < boardLength; ++row) {
        std::cout << "-------------------------------------" << '\n';
        std::cout << "| ";
        for (int col = 0; col < boardLength; ++col) {
            std::cout << board[row*boardLength+col] << " | ";
        }
        std::cout << '\n';
    }
    std::cout << "-------------------------------------" << '\n';
}

bool isValidRowInsert(SudokuBoard board, int boardLength, int x, int y, int assignedValue) {
    for (int col = 0; col < boardLength; ++col) {
        if (col != y && board[x*boardLength+col] == assignedValue) {
            return false;
        }
    }
    return true;
}

bool isValidColInsert(SudokuBoard board, int boardLength, int x, int y, int assignedValue) {
    for (int row = 0; row < boardLength; ++row) {
        if (row != x && board[row*boardLength+y] == assignedValue) {
            return false;
        }
    }
    return true;
}

bool isValidBoxInsert(SudokuBoard board, int boxLength, int x, int y, int assignedValue) {
    int boardLength = boxLength * boxLength;
    // division operation needs to happen first to round down
    int topLeftX = (x / boxLength) * boxLength;
    int topLeftY = (y / boxLength) * boxLength;

    for (int row = 0; row < boxLength; ++row) {
        for (int col = 0; col < boxLength; ++col) {
            if (topLeftX + row != x && topLeftY + col != y && board[(topLeftX + row)*boardLength+topLeftY+col] == assignedValue) {
                return false;
            }
        }
    }
    return true;
}

bool isValidInsert(SudokuBoard board, int boxLength, int boardLength, int x, int y, int assignedValue) {

    if (assignedValue < 1 || assignedValue > boardLength) {
        return false;
    }

    return isValidRowInsert(board, boardLength, x, y, assignedValue) &&
        isValidColInsert(board, boardLength, x, y, assignedValue) &&
        isValidBoxInsert(board, boxLength, x, y, assignedValue);
}

bool solveBoard(SudokuBoard board, int boxLength, int boardPosition, int& solutions, bool uniqueFlag) {

    if (boxLength <= 0) {
        return false;
    }

    int boardLength = boxLength * boxLength;
    if (boardPosition < 0 || boardPosition > boardLength * boardLength) {
        return false;
    }

    if (boardPosition == boardLength * boardLength) {
        ++solutions;
        return !uniqueFlag;
    }
    
    int boardX = boardPosition / boardLength;
    int boardY = boardPosition % boardLength;

    std::vector<int> numList;
    for (int i = 1; i <= boardLength; ++i) {
        numList.push_back(i);
    }

    if (board[boardPosition] == 0) {
        std::random_shuffle(numList.begin(), numList.end());
        for (unsigned int index = 0; index < numList.size(); ++index) {
            board[boardPosition] = numList[index];
            if (isValidInsert(board, boxLength, boardLength, boardX, boardY, numList[index])) {
                if (solveBoard(board, boxLength, boardPosition + 1, solutions, uniqueFlag)) {
                    return true;
                }
            }
            board[boardPosition] = 0;
        }
        return false;
    }
    else {
        if (isValidInsert(board, boxLength, boardLength, boardX, boardY, board[boardPosition])) {
            return solveBoard(board, boxLength, boardPosition + 1, solutions, uniqueFlag);
        }
        return false;
    }
}

bool populateBoard(SudokuBoard board, int boxLength) {

    if (boxLength <= 0) {
        return false;
    }

    std::srand(static_cast<unsigned int>(std::time(0)));

    int boardLength = boxLength * boxLength;
    int solutions = 0;
    for (int row = 0; row < boardLength; ++row) {
        for (int col = 0; col < boardLength; ++col) {
            board[row * boardLength + col] = 0;
        }
    }
    return solveBoard(board, boxLength, 0, solutions, false);
}

bool createBoard(SudokuBoard board, int boxLength, int pairCluesRemoved) {

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

    while (pairCluesRemoved > 0) {
        boardPos = rand() % (boardLength * boardLength);

        if (board[boardPos] != 0) {
            oppositeBoardPos = boardLength * boardLength - boardPos - 1;
            board[boardPos] = 0;
            board[oppositeBoardPos] = 0;
            --pairCluesRemoved;
        }
    }
    return true;
}

/*int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    int badBoard [81] = {   1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9,
                            1,2,3,4,5,6,7,8,9 };

    int solutions = 0;
    int * unsolvedBoard  = new int [81]{    0,0,0,0,0,9,0,0,2,
                                            8,3,0,0,1,0,0,0,0,
                                            0,9,0,0,0,7,5,0,4,
                                            0,2,6,0,0,0,0,0,0,
                                            1,0,3,0,2,0,8,0,7,
                                            0,0,0,0,0,0,4,2,0,
                                            6,0,2,1,0,0,0,5,0,
                                            0,0,0,0,5,0,0,7,9,
                                            9,0,0,7,0,0,0,0,0 };

    solveBoard(unsolvedBoard, 3, 0, solutions, false);
    printBoard(unsolvedBoard, 9);
    delete[] unsolvedBoard;

    return 0;
}*/