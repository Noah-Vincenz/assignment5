#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <cmath>


// TODO: Your Sudoku class goes here:
class Sudoku { // public Searchable
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
        //when it finds 1 at (0,4) it keeps erasing 1 only for every single one but it shouldnt be
        //it does erasing fine until having finihsed with 1 at (0,4) then it gets stuck at 11111111111111111
        bool result = true;
        field[row][col].clear();
        field[row][col].insert(value);
        std::cout << "Setting a sqr: Row: " << row << "  Col: " << col << "  Data: " << getSquare(row, col) << std::endl;
        bool isStillChanging = true;
        while (isStillChanging == true) {
            std::cout << "now in while loop" << std::endl;
            isStillChanging = false;
            for (int row1 = 0; row1 < size; ++row1) {
                for (int col1 = 0; col1 < size; ++col1) {
                    if (field[row1][col1].size() == 1) {
                        std::cout << "Square full at (" << row1 << ", " << col1 << ")." << std::endl;
                        int value = field[row1][col1].begin().operator*();
                        std::cout << "This is the value that needs to be checked for: " << value << std::endl;
                        for (int i = 0; i < size; ++i) {
                            //taking care of squares on same row
                            std::cout << " this is the value of find(value): " << field[i][col1].find(value).operator*()  <<std::endl;
                            if (field[i][col1].find(value).operator*() == value && field[i][col1].find(value) != field[i][col1].end() && i != row1) { //if value is inside the set && value is not only value
                                field[i][col1].erase(value);
                                isStillChanging = true;
                                std::cout << "staying while loop now" << std::endl;
                                if (field[i][col1].size() == 0) {
                                    result = false;
                                    break;
                                }
                                std::cout << "111111111111111111111" << std::endl;
                                std::cout << "erasing " << value << " from (" << i << ", " << col1 << ")." << std::endl;
                                std::cout << "size: " << field[i][col1].size() << std::endl;
                            }
                            //taking care of squares on same column
                            if (field[row1][i].find(value).operator*() == value && field[row1][i].find(value) != field[row1][i].end() && i != col1) { //if value is inside the set && value is not only value
                                field[row1][i].erase(value); //does erase erase at index or erase that elem
                                isStillChanging = true;
                                std::cout << "staying while loop now" << std::endl;
                                if (field[row1][i].size() == 0) {
                                    result = false;
                                    std::cout << "should leave while loop now" << std::endl;
                                    break;
                                }
                                std::cout << "22222222222222222222222" << std::endl;     //these three are repeated 3 times thenback to "sqr full at.."
                                std::cout << "erasing " << value << " from (" << row1 << ", " << i << ")." << std::endl;
                                std::cout << "size: " << field[row1][i].size() << std::endl;
                            }
                        }
                        int sqrt = std::sqrt(size);
                        int rowMod = row1 - row1 % sqrt;
                        int colMod = col1 - col1 % sqrt;
                        for (int row2 = rowMod; row2 < (rowMod + sqrt); ++row2) {
                            for (int col2 = colMod; col2 < (colMod + sqrt); ++col2) {
                                if (field[row2][col2].find(value).operator*() == value && field[row2][col2].find(value) != field[row2][col2].end() && row2 != row1 && col2 != col1) { //if value is inside the set && value is not only value
                                    field[row2][col2].erase(value);
                                    isStillChanging = true;
                                    std::cout << "staying while loop now" << std::endl;
                                    if (field[row2][col2].size() == 0) {
                                        result = false;
                                        break;
                                    }
                                    std::cout << "333333333333333333333333" << std::endl;
                                    std::cout << "erasing " << value << " from (" << row2 << ", " << col2 << ")." << std::endl;
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
    bool isSolution() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (field[i][j].size() != 1) {
                    return false;
                }
            }
        }
        return true;
    }
    void write(ostream & o) { //std::ostream??
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (getSquare(i,j) == -1) {
                    o << " ";
                }
                else {
                    o << getSquare(i,j);
                }
            }
        }
    }
    /*
    Sudoku successors() {
        vector<unique_ptr<Searchable> > successors;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (getSquare(i,j) == -1) {
                    int row = i;
                    int leftMostSquare = getSquare(row,j);
                }
            }
        }
    }
    */
};
#endif
