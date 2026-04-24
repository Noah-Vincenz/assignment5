#include <gtest/gtest.h>
#include "Sudoku.h"
#include "BreadthFirstSearch.h"
#include "BestFirstSearch.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace {

bool fillBoard(Sudoku * board, const vector<string> & fillWith) {
    for (size_t row = 0; row < fillWith.size(); ++row) {
        for (size_t col = 0; col < fillWith.size(); ++col) {
            if (fillWith[row][col] != ' ') {
                const int setTo = fillWith[row][col] - '0';
                if (!board->setSquare(row, col, setTo)) {
                    return false;
                }
            }
        }
    }
    return true;
}

const vector<string> hardBoard {
    "8        ",
    "  36     ",
    " 7  9 2  ",
    " 5   7   ",
    "    457  ",
    "   1   3 ",
    "  1    68",
    "  85   1 ",
    " 9    4  "
};

const vector<string> hardAnswer {
    "812753649",
    "943682175",
    "675491283",
    "154237896",
    "369845721",
    "287169534",
    "521974368",
    "438526917",
    "796318452"
};

const vector<string> easyBoard {
    "23 915   ",
    "   2  54 ",
    "6 7      ",
    "  1     9",
    "89 5 3 17",
    "5     6  ",
    "      9 5",
    " 16  7   ",
    "   329  1"
};

const vector<string> easyAnswer {
    "234915786",
    "189276543",
    "657438192",
    "461782359",
    "892563417",
    "573194628",
    "328641975",
    "916857234",
    "745329861"
};

} // anonymous namespace

TEST(SudokuConstructor, AllCellsUndetermined) {
    Sudoku board(9);
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            EXPECT_EQ(board.getSquare(i, j), -1);
        }
    }
}

TEST(SudokuConstructor, NotASolution) {
    Sudoku board(9);
    EXPECT_FALSE(board.isSolution());
}

TEST(SudokuConstructor, HeuristicValueFullBoard) {
    Sudoku board(9);
    EXPECT_EQ(board.heuristicValue(), 81);
}

TEST(SetSquare, BasicSet) {
    Sudoku board(9);
    EXPECT_TRUE(board.setSquare(0, 0, 5));
    EXPECT_EQ(board.getSquare(0, 0), 5);
}

TEST(SetSquare, ConstraintPropagation) {
    Sudoku board(9);
    board.setSquare(0, 0, 5);
    // after setting (0,0)=5, heuristic should decrease (at least one cell resolved)
    EXPECT_LT(board.heuristicValue(), 81);
}

TEST(SetSquare, ConflictReturnsFalse) {
    Sudoku board(9);
    board.setSquare(0, 0, 5);
    // setting same value on same row should eventually cause conflict
    EXPECT_FALSE(board.setSquare(0, 1, 5));
}

TEST(SetSquare, ConflictSameColumn) {
    Sudoku board(9);
    board.setSquare(0, 0, 5);
    EXPECT_FALSE(board.setSquare(1, 0, 5));
}

TEST(SetSquare, ConflictSameBox) {
    Sudoku board(9);
    board.setSquare(0, 0, 5);
    EXPECT_FALSE(board.setSquare(1, 1, 5));
}

TEST(IsSolution, EasyPuzzleSolvedByPropagation) {
    Sudoku board(9);
    ASSERT_TRUE(fillBoard(&board, easyBoard));
    EXPECT_TRUE(board.isSolution());
}

TEST(IsSolution, CorrectAnswer) {
    Sudoku board(9);
    fillBoard(&board, easyBoard);
    for (size_t row = 0; row < easyAnswer.size(); ++row) {
        for (size_t col = 0; col < easyAnswer.size(); ++col) {
            int expected = easyAnswer[row][col] - '0';
            EXPECT_EQ(board.getSquare(row, col), expected)
                << "Mismatch at [" << row << "][" << col << "]";
        }
    }
}

TEST(HeuristicValue, DecreasesAfterSetSquare) {
    Sudoku board(9);
    int before = board.heuristicValue();
    board.setSquare(0, 0, 1);
    EXPECT_LT(board.heuristicValue(), before);
}

TEST(HeuristicValue, ZeroWhenSolved) {
    Sudoku board(9);
    fillBoard(&board, easyBoard);
    EXPECT_EQ(board.heuristicValue(), 0);
}

TEST(Successors, BranchesOnHardPuzzle) {
    Sudoku board(9);
    fillBoard(&board, hardBoard);
    auto succ = board.successors();
    // hard puzzle should produce successors (not solved by propagation alone)
    EXPECT_GT(succ.size(), 0u);
}

TEST(Successors, EmptyForSolvedPuzzle) {
    Sudoku board(9);
    fillBoard(&board, easyBoard);
    auto succ = board.successors();
    EXPECT_EQ(succ.size(), 0u);
}

TEST(BreadthFirstSearchTest, SolvesHardPuzzle) {
    auto board = std::make_unique<Sudoku>(9);
    fillBoard(board.get(), hardBoard);

    BreadthFirstSearch search(std::move(board));
    Searchable * solution = search.solve();

    ASSERT_NE(solution, nullptr);
    EXPECT_TRUE(solution->isSolution());

    Sudoku * solved = static_cast<Sudoku *>(solution);
    for (size_t row = 0; row < hardAnswer.size(); ++row) {
        for (size_t col = 0; col < hardAnswer.size(); ++col) {
            int expected = hardAnswer[row][col] - '0';
            EXPECT_EQ(solved->getSquare(row, col), expected);
        }
    }
}

TEST(BestFirstSearchTest, SolvesHardPuzzle) {
    auto board = std::make_unique<Sudoku>(9);
    fillBoard(board.get(), hardBoard);

    BestFirstSearch search(std::move(board));
    Searchable * solution = search.solve();

    ASSERT_NE(solution, nullptr);
    EXPECT_TRUE(solution->isSolution());

    Sudoku * solved = static_cast<Sudoku *>(solution);
    for (size_t row = 0; row < hardAnswer.size(); ++row) {
        for (size_t col = 0; col < hardAnswer.size(); ++col) {
            int expected = hardAnswer[row][col] - '0';
            EXPECT_EQ(solved->getSquare(row, col), expected);
        }
    }
}

TEST(BestFirstSearchTest, ExpandsFewerNodesThanBFS) {
    auto bfsBoard = std::make_unique<Sudoku>(9);
    fillBoard(bfsBoard.get(), hardBoard);
    BreadthFirstSearch bfs(std::move(bfsBoard));
    bfs.solve();

    auto bestBoard = std::make_unique<Sudoku>(9);
    fillBoard(bestBoard.get(), hardBoard);
    BestFirstSearch best(std::move(bestBoard));
    best.solve();

    EXPECT_LT(best.getNodesExpanded(), bfs.getNodesExpanded());
}

TEST(FourByFour, SmallPuzzle) {
    // 4x4 sudoku with 2x2 boxes, values 1-4
    Sudoku board(4);
    // partial board:
    // 1 _ | _ _
    // _ _ | 1 _
    // ----+----
    // _ 1 | _ _
    // _ _ | _ 1
    EXPECT_TRUE(board.setSquare(0, 0, 1));
    EXPECT_TRUE(board.setSquare(1, 2, 1));
    EXPECT_TRUE(board.setSquare(2, 1, 1));
    EXPECT_TRUE(board.setSquare(3, 3, 1));

    // value 1 placed in each box, row, and column — should still be valid
    EXPECT_EQ(board.getSquare(0, 0), 1);
    EXPECT_EQ(board.getSquare(1, 2), 1);
    EXPECT_EQ(board.getSquare(2, 1), 1);
    EXPECT_EQ(board.getSquare(3, 3), 1);
}

TEST(EdgeCases, BoardSize1) {
    Sudoku board(1);
    // 1x1 board: single cell with value {1}
    EXPECT_EQ(board.getSquare(0, 0), 1);
    EXPECT_TRUE(board.isSolution());
    EXPECT_EQ(board.heuristicValue(), 0);
}

TEST(EdgeCases, NonPerfectSquareThrows) {
    EXPECT_THROW(Sudoku(5), std::invalid_argument);
    EXPECT_THROW(Sudoku(6), std::invalid_argument);
    EXPECT_THROW(Sudoku(10), std::invalid_argument);
}

TEST(EdgeCases, ImpossiblePuzzleDetected) {
    // setting two identical values in the same row must fail
    Sudoku board(9);
    board.setSquare(0, 0, 1);
    EXPECT_FALSE(board.setSquare(0, 4, 1));
}

TEST(Performance, BestFirstNodeBound) {
    // guard against performance regressions on the hard puzzle
    auto board = std::make_unique<Sudoku>(9);
    fillBoard(board.get(), hardBoard);
    BestFirstSearch search(std::move(board));
    search.solve();
    EXPECT_LT(search.getNodesExpanded(), 200)
        << "performance regression: too many nodes expanded";
}

TEST(Performance, BreadthFirstNodeBound) {
    auto board = std::make_unique<Sudoku>(9);
    fillBoard(board.get(), hardBoard);
    BreadthFirstSearch search(std::move(board));
    search.solve();
    EXPECT_LT(search.getNodesExpanded(), 300)
        << "performance regression: too many nodes expanded";
}
