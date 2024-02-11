#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>

#include "board.h"

class MoveGenerator {
   public:
    struct Move {
        int x = -1;
        int y = -1;

        bool operator==(const Move &other) const { return x == other.x && y == other.y; }
    };

    MoveGenerator();

    void sortMoves();
    void updateMoveScoreByDir(const Move &move, int dir, int w, Board::CHESS_COLOR);
    void addMove(const Move &move);
    void eraseMove(const Move &move);
    bool existsMove(const Move &move);
    std::vector<Move> generateMovesList(int cnt);
    int playerMoveScore(const Move &move, Board::CHESS_COLOR color) const;
    int maxMoveScore(const Move &move) const;
    int sumPlayerScore(Board::CHESS_COLOR color) const;

    const static int INVALID_MOVE_WEIGHT = -__INT32_MAX__;

   public:
    std::vector<Move> m_moves;
    int m_recorded[Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_dirScore[2][4][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_playerMoveScore[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_maxScore[Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_cntKill[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_sumPlayerScore[2] = {0, 0};
};

#endif