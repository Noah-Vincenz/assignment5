#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <cmath>


// TODO: Your Sudoku class goes here:
class Sudoku: public Searchable {
private:
    int size;
    vector<vector<std::set<int> > > field;
    std::set<int> mySet;
public:
    Sudoku(int sizeIn)
            : size(sizeIn) {
        field = vector<vector<std::set<int> > >(sizeIn, std::vector<std::set<int> >(sizeIn));
        for (int row = 0; row < sizeIn; ++row) {
            for (int col = 0; col < sizeIn; ++col) {
                //std::set < int> mySet;
                mySet.clear();
                for (int i = 1; i <= sizeIn; ++i) {
                    mySet.insert(i);
                    field[row][col].insert(i);
                }
                field[row][col] = mySet;
            }
        }
    }
    int getSquare(int row, int col) const {
        if (field[row][col].size() > 1) {
            return -1;
        } else {
            return field[row][col].begin().operator*();
        }
    }
    bool setSquare(int row, int col, int value) {
        bool result = true;
        field[row][col].clear();
        field[row][col].insert(value);
        bool isStillChanging = true;
        while (isStillChanging == true) {
            isStillChanging = false;
            for (int row1 = 0; row1 < size; ++row1) {
                for (int col1 = 0; col1 < size; ++col1) {
                    if (field[row1][col1].size() == 1) {
                        int value = field[row1][col1].begin().operator*();
                        for (int i = 0; i < size; ++i) {
                            //taking care of squares on same row
                            if (field[i][col1].find(value) != field[i][col1].end() && i != row1) { //if value is inside the set && value is not only value
                                field[i][col1].erase(value);
                                isStillChanging = true;
                                if (field[i][col1].size() == 0) {
                                    result = false;
                                    break;
                                }
                            }
                            //taking care of squares on same column
                            if (field[row1][i].find(value) != field[row1][i].end() && i != col1) { //if value is inside the set && value is not only value
                                field[row1][i].erase(value);
                                isStillChanging = true;
                                if (field[row1][i].size() == 0) {
                                    result = false;
                                    break;
                                }
                            }
                        }
                        int sqrt = std::sqrt(size);
                        int rowMod = row1 - row1 % sqrt;
                        int colMod = col1 - col1 % sqrt;
                        for (int row2 = rowMod; row2 < (rowMod + sqrt); ++row2) {
                            for (int col2 = colMod; col2 < (colMod + sqrt); ++col2) {
                                if (field[row2][col2].find(value) != field[row2][col2].end() && row2 != row1 && col2 != col1) { //if value is inside the set && value is not only value
                                    field[row2][col2].erase(value);
                                    isStillChanging = true;
                                    if (field[row2][col2].size() == 0) {
                                        result = false;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return result;
    }
//should be virtual??
    bool isSolution() const override {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (getSquare(i,j) == -1) {
                    return false;
                }
            }
        }
        return true;
    }
    void write(ostream & o) const override {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (getSquare(i,j) == -1) {
                    o << " ";
                }
                else {
                    o << getSquare(i,j);
                }
            }
            o << std::endl;
        }
    }
    vector<unique_ptr<Searchable> > successors() const override {
        vector<unique_ptr<Searchable> > successors;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                //if (getSquare(i,j) == -1) { //returns -1 for every single one ----------------------------------------
                if (field[i][j].size() > 1) { //doesnt make a difference
                    std:: cout << "Square empty at: (" << i << ", " << j << ")" << std::endl;
                    for(int a : field[i][j]) {
                        unique_ptr<Searchable> board = unique_ptr<Sudoku>(new Sudoku(*this));
                        if (static_cast<Sudoku*>(board.get())->setSquare(i,j,a)) {
                            successors.emplace_back(std::move(board));
                        }
                        else {
                            board.reset();
                        }
                    }
                }
            }
        }
        return successors;
    }
};
#endif
