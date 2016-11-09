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
    Sudoku(const int sizeIn)
            : size(sizeIn) {
        field = vector<vector<std::set<int> > >(sizeIn, std::vector<std::set<int> >(sizeIn));
        for (int row = 0; row < sizeIn; ++row) {
            for (int col = 0; col < sizeIn; ++col) {
                mySet.clear();
                for (int i = 1; i <= sizeIn; ++i) {
                    mySet.insert(i);
                    field[row][col].insert(i);
                }
                field[row][col] = mySet;
            }
        }
    }
    int getSquare(const int row, const int col) const {
        if (field[row][col].size() > 1) {
            return -1;
        } else {
            return *field[row][col].begin();
        }
    }
    bool setSquare(const int row, const int col, const int value) {
        bool result = true;
        field[row][col].clear();
        field[row][col].insert(value);
        bool isStillChanging = true;
        while (isStillChanging == true) {
            isStillChanging = false;
            for (int row1 = 0; row1 < size; ++row1) {
                for (int col1 = 0; col1 < size; ++col1) {
                    int sqrt = std::sqrt(size);
                    int rowMod = row1 - row1 % sqrt;
                    int colMod = col1 - col1 % sqrt;
                    if (field[row1][col1].size() == 1) {
                        int value = *field[row1][col1].begin();
                        for (int i = 0; i < size; ++i) {
                            //taking care of squares on same col
                            if (field[i][col1].find(value) != field[i][col1].end() && i != row1) { //if value is inside the set && value is not only value
                                field[i][col1].erase(value);
                                isStillChanging = true;
                                if (field[i][col1].size() == 0) {
                                    result = false;
                                    break;
                                }
                            }
                            //taking care of squares on same row
                            if (field[row1][i].find(value) != field[row1][i].end() && i != col1) { //if value is inside the set && value is not only value
                                field[row1][i].erase(value);
                                isStillChanging = true;
                                if (field[row1][i].size() == 0) {
                                    result = false;
                                    break;
                                }
                            }
                        }
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
                    //checking for identical pairs in sets
                    else if (field[row1][col1].size() == 2) {
                        for (int i = 0; i < size; ++i) {
                            if (field[row1][col1] == field[row1][i] && i != col1) { //if the sets are the same
                                for (int j = 0; j < size && j != col1 && j != i; ++j) {
                                    for (int k : field[row1][col1]) {
                                        if (field[row1][j].find(k) != field[row1][j].end()) {
                                            field[row1][j].erase(k);
                                            isStillChanging = true;
                                            if (field[row1][j].size() == 0) {
                                                result = false;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            if (field[row1][col1] == field[i][col1] && i != row1) { //if the sets are the same
                                for (int j = 0; j < size && j != row1 && j != i; ++j) {
                                    for (int k : field[row1][col1]) {
                                        if (field[j][col1].find(k) != field[j][col1].end()) {
                                            field[j][col1].erase(k);
                                            isStillChanging = true;
                                            if (field[j][col1].size() == 0) {
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
            }
        }
        return result;
    }
    virtual bool isSolution() const override {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (getSquare(i,j) == -1) {
                    return false;
                }
            }
        }
        return true;
    }
    virtual void write(ostream & o) const override {
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
    virtual vector<unique_ptr<Searchable> > successors() const override {
        vector<unique_ptr<Searchable> > successors;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (field[i][j].size() > 1) {
                    for(int a : field[i][j]) {
                        Sudoku * board = new Sudoku(*this);
                        if (board->setSquare(i,j,a)) {
                            successors.emplace_back(board);
                        }
                        else {
                            delete board;
                        }
                    }
                    return successors;
                }
            }
        }
        return successors;
    }
    virtual int heuristicValue() const override {
        int count = 0;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if(field[i][j].size() > 1) {
                    ++count;
                }
            }
        }
        return count;
    }
};
#endif
