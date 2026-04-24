#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <cmath>
#include <stdexcept>

class Sudoku : public Searchable {
private:
    int boardSize;
    int boxSize;
    vector<vector<std::set<int>>> board;

public:
    Sudoku(int sizeIn)
        : boardSize(sizeIn), boxSize(static_cast<int>(std::sqrt(sizeIn))) {
        if (boxSize * boxSize != boardSize) {
            throw std::invalid_argument("boardSize must be a perfect square");
        }
        board.resize(sizeIn, std::vector<std::set<int>>(sizeIn));
        for (int row = 0; row < sizeIn; ++row) {
            for (int col = 0; col < sizeIn; ++col) {
                for (int i = 1; i <= sizeIn; ++i) {
                    board[row][col].insert(i);
                }
            }
        }
    }

    int getSquare(int row, int col) const {
        if (board[row][col].size() == 1) {
            return *board[row][col].begin();
        }
        return -1;
    }

    bool setSquare(int row, int col, int value) {
        board[row][col].clear();
        board[row][col].insert(value);

        bool isStillChanging = true;
        while (isStillChanging) {
            isStillChanging = false;
            for (int row1 = 0; row1 < boardSize; ++row1) {
                for (int col1 = 0; col1 < boardSize; ++col1) {
                    int rowMod = row1 - row1 % boxSize;
                    int colMod = col1 - col1 % boxSize;

                    if (board[row1][col1].size() == 1) {
                        int determinedValue = *board[row1][col1].begin();

                        // eliminate determined value from same column and same row
                        for (int i = 0; i < boardSize; ++i) {
                            if (i != row1 && board[i][col1].count(determinedValue)) {
                                board[i][col1].erase(determinedValue);
                                isStillChanging = true;
                                if (board[i][col1].empty()) return false;
                            }
                            if (i != col1 && board[row1][i].count(determinedValue)) {
                                board[row1][i].erase(determinedValue);
                                isStillChanging = true;
                                if (board[row1][i].empty()) return false;
                            }
                        }

                        // eliminate from same box
                        for (int row2 = rowMod; row2 < rowMod + boxSize; ++row2) {
                            for (int col2 = colMod; col2 < colMod + boxSize; ++col2) {
                                if (!(row2 == row1 && col2 == col1)
                                    && board[row2][col2].count(determinedValue)) {
                                    board[row2][col2].erase(determinedValue);
                                    isStillChanging = true;
                                    if (board[row2][col2].empty()) return false;
                                }
                            }
                        }
                    }
                    // naked pairs elimination
                    else if (board[row1][col1].size() == 2) {
                        // check row for naked pair
                        for (int i = 0; i < boardSize; ++i) {
                            if (i != col1 && board[row1][col1] == board[row1][i]) {
                                for (int j = 0; j < boardSize; ++j) {
                                    if (j == col1 || j == i) continue;
                                    for (int k : board[row1][col1]) {
                                        if (board[row1][j].count(k)) {
                                            board[row1][j].erase(k);
                                            isStillChanging = true;
                                            if (board[row1][j].empty()) return false;
                                        }
                                    }
                                }
                            }
                        }

                        // check column for naked pair
                        for (int i = 0; i < boardSize; ++i) {
                            if (i != row1 && board[row1][col1] == board[i][col1]) {
                                for (int j = 0; j < boardSize; ++j) {
                                    if (j == row1 || j == i) continue;
                                    for (int k : board[row1][col1]) {
                                        if (board[j][col1].count(k)) {
                                            board[j][col1].erase(k);
                                            isStillChanging = true;
                                            if (board[j][col1].empty()) return false;
                                        }
                                    }
                                }
                            }
                        }

                        // check box for naked pair
                        for (int r = rowMod; r < rowMod + boxSize; ++r) {
                            for (int c = colMod; c < colMod + boxSize; ++c) {
                                if (!(r == row1 && c == col1)
                                    && board[row1][col1] == board[r][c]) {
                                    for (int r2 = rowMod; r2 < rowMod + boxSize; ++r2) {
                                        for (int c2 = colMod; c2 < colMod + boxSize; ++c2) {
                                            if ((r2 == row1 && c2 == col1) || (r2 == r && c2 == c)) continue;
                                            for (int k : board[row1][col1]) {
                                                if (board[r2][c2].count(k)) {
                                                    board[r2][c2].erase(k);
                                                    isStillChanging = true;
                                                    if (board[r2][c2].empty()) return false;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    virtual bool isSolution() const override {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (getSquare(i, j) == -1) {
                    return false;
                }
            }
        }
        return true;
    }

    virtual void write(ostream & o) const override {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (getSquare(i, j) == -1) {
                    o << " ";
                } else {
                    o << getSquare(i, j);
                }
            }
            o << std::endl;
        }
    }

    virtual vector<unique_ptr<Searchable>> successors() const override {
        vector<unique_ptr<Searchable>> result;

        // mrv heuristic: pick the undetermined cell with fewest candidates
        int bestRow = -1, bestCol = -1;
        int bestSize = boardSize + 1;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                int sz = board[i][j].size();
                if (sz > 1 && sz < bestSize) {
                    bestSize = sz;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }

        if (bestRow == -1) {
            return result;
        }

        for (int a : board[bestRow][bestCol]) {
            auto copy = std::make_unique<Sudoku>(*this);
            if (copy->setSquare(bestRow, bestCol, a)) {
                result.push_back(std::move(copy));
            }
        }

        // single-successor optimization: if only one valid successor and it's not
        // solved, skip the queue and expand it directly via iteration
        while (result.size() == 1 && !result[0]->isSolution()) {
            result = result[0]->successors();
        }

        return result;
    }

    virtual int heuristicValue() const override {
        int count = 0;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j].size() > 1) {
                    ++count;
                }
            }
        }
        return count;
    }
};

#endif
