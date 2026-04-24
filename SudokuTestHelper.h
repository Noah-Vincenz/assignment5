#ifndef SUDOKU_TEST_HELPER_H
#define SUDOKU_TEST_HELPER_H

#include "Sudoku.h"

#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

bool fillBoard(Sudoku * board, const vector<string> & fillWith) {
    for (size_t row = 0; row < fillWith.size(); ++row) {
        for (size_t col = 0; col < fillWith.size(); ++col) {
            if (fillWith[row][col] != ' ') {
                const int setTo = fillWith[row][col] - '0';
                if (!board->setSquare(row, col, setTo)) {
                    cout << "Error: setSquare returned false after setting square["
                         << row << "][" << col << "] to " << setTo
                         << " - this should not happen as the example boards are solvable\n";
                    return false;
                }
            }
        }
    }
    return true;
}

bool checkAnswer(Sudoku * board, const vector<string> & answer) {
    for (size_t row = 0; row < answer.size(); ++row) {
        for (size_t col = 0; col < answer.size(); ++col) {
            const int setTo = answer[row][col] - '0';
            if (board->getSquare(row, col) != setTo) {
                cout << "Error: wrong answer in square[" << row << "][" << col
                     << "]: should be " << setTo << " but is "
                     << board->getSquare(row, col) << endl;
                return false;
            }
        }
    }
    return true;
}

#endif
