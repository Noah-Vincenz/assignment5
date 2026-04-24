#include "SudokuTestHelper.h"
#include "BestFirstSearch.h"

int main() {
    {
        vector<string> hardBoard{"8        ",
                                 "  36     ",
                                 " 7  9 2  ",

                                 " 5   7   ",
                                 "    457  ",
                                 "   1   3 ",

                                 "  1    68",
                                 "  85   1 ",
                                 " 9    4  "};

        vector<string> answer{"812753649",
                              "943682175",
                              "675491283",
                              "154237896",
                              "369845721",
                              "287169534",
                              "521974368",
                              "438526917",
                              "796318452"};

        auto board = std::make_unique<Sudoku>(9);

        if (!fillBoard(board.get(), hardBoard)) {
            cout << "Failed: When filling the board, setSquare() returned false\n";
            return 1;
        }
        cout << "Trying to solve hard board:\n";
        board->write(cout);

        BestFirstSearch search(std::move(board));

        Searchable * solution = search.solve();

        if (solution == nullptr) {
            cout << "\nFailed: Couldn't be solved\n";
            return 1;
        }

        Sudoku * solvedBoard = static_cast<Sudoku *>(solution);

        if (checkAnswer(solvedBoard, answer)) {
            cout << "\nPassed: Solution was:\n";
            solvedBoard->write(cout);
            cout << "\nNodes expanded: " << search.getNodesExpanded() << std::endl;
            return 0;
        } else {
            cout << "Failed: Solution was returned, but it was the wrong answer\n";
            return 1;
        }
    }
}
