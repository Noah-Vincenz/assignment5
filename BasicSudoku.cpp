#include "SudokuTestHelper.h"

int main() {
    {
        vector<string> easyBoard{"23 915   ",
                                 "   2  54 ",
                                 "6 7      ",

                                 "  1     9",
                                 "89 5 3 17",
                                 "5     6  ",

                                 "      9 5",
                                 " 16  7   ",
                                 "   329  1"};

        cout << "Making Sudoku board for an easy puzzle:\n";
        for (const auto & s : easyBoard) {
            cout << s << endl;
        }

        vector<string> easyAnswer{"234915786",
                                  "189276543",
                                  "657438192",
                                  "461782359",
                                  "892563417",
                                  "573194628",
                                  "328641975",
                                  "916857234",
                                  "745329861"};

        auto board = std::make_unique<Sudoku>(9);

        if (!fillBoard(board.get(), easyBoard)) {
            cout << "Failed: When filling the board, setSquare() returned false\n";
            return 1;
        }

        if (!checkAnswer(board.get(), easyAnswer)) {
            cout << "Failed: The board has the wrong answer in it\n";
            return 1;
        }
    }

    cout << "Passed: easy sudoku puzzle solved, every square had the right answer\n";
    return 0;
}
