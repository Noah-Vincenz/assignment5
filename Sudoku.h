#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <cmath>


// TODO: Your Sudoku class goes here:
class Sudoku {
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
        std::cout << "setting a sqr." << std::endl;
        bool result = true;
        field[row][col].clear();
        field[row][col].insert(value);
        std::cout << "Row: " << row << "  Col: " << col << "  Data: " << getSquare(row, col) << std::endl;
        bool isStillChanging = true;
        while (isStillChanging) {
            isStillChanging = false;
            for (int row1 = 0; row1 < size; ++row1) {
                for (int col1 = 0; col1 < size; ++col1) {
                    if (field[row1][col1].size() == 1) {
                        std::cout << "Square full at (" << row1 << ", " << col1 << ")." << std::endl;
                        int value = field[row1][col1].begin().operator*();
                        for (int i = 0; i < size; ++i) {
                            //taking care of squares on same row
                            //if (field[row1][i].find(value).operator*() == value && field[row1][i].size() != 1) { //if value is inside the set && value is not only value
                            if (field[row1][i].find(value).operator*() == value && i != col1) { //if value is inside the set && value is not only value
                                field[row1][i].erase(value); //does erase erase at index or erase that elem
                                int size = field[row1][i].size();
                                if (size == 0) {
                                    result = false;
                                    break;
                                }
                                if (size > 1) {
                                    isStillChanging = true;
                                }
                                std::cout << "erasing " << value << " from (" << row1 << ", " << i << ")." << std::endl;
                                std::cout << "size: " << field[row1][i].size() << std::endl;
                            }
                            //taking care of squares on same column
                            //if (field[i][col1].find(value).operator*() == value && field[i][col1].size() != 1) { //if value is inside the set && value is not only value
                            if (field[i][col1].find(value).operator*() == value && i != row1) { //if value is inside the set && value is not only value
                                field[i][col1].erase(value);
                                int size = field[i][col1].size();
                                if (size == 0) {
                                    result = false;
                                    break;
                                }
                                if (size > 1) {
                                    isStillChanging = true;
                                }
                                std::cout << "erasing " << value << " from (" << i << ", " << col1 << ")." << std::endl;
                                std::cout << "size: " << field[i][col1].size() << std::endl;
                            }
                        }
                        int sqrt = std::sqrt(size);
                        int rowMod = row1 - row1 % sqrt;
                        int colMod = col1 - col1 % sqrt;
                        for (int row2 = rowMod; row2 < (rowMod + sqrt); ++row2) {
                            for (int col2 = colMod; col2 < (colMod + sqrt); ++col2) {
                                //if (field[row2][col2].find(value).operator*() == value && field[row2][col2].size() != 1) { //if value is inside the set && value is not only value
                                if (field[row2][col2].find(value).operator*() == value && row2 != row1 && col2 != col1) { //if value is inside the set && value is not only value
                                    field[row2][col2].erase(value);
                                    int size = field[row2][col2].size();
                                    if (size == 0) {
                                        result = false;
                                        break;
                                    }
                                    if (size > 1) {
                                        isStillChanging = true;
                                    }
                                    std::cout << "erasing " << value << " from (" << row2 << ", " << col2 << ")."
                                              << std::endl;
                                    std::cout << "size: " << field[row2][col2].size() << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        return result;
    }
};
#endif
