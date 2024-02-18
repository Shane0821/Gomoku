#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>

#include "board.h"
#include "scorer.h"

class MoveGenerator {
   public:
    struct Move {
        int x = -1;
        int y = -1;

        bool operator==(const Move &other) const { return x == other.x && y == other.y; }
    };

    MoveGenerator();

    void updateMoveScoreByDir(const Move &move, int dir, Scorer::Type,
                              Board::PIECE_COLOR);
    void addMove(const Move &move);
    void eraseMove(const Move &move);
    bool existsMove(const Move &move);
    std::vector<Move> generateMovesList(int cnt);
    std::vector<Move> generateKillMovesList();
    void setGenKillFlag(bool flag);
    int playerMoveScore(const Move &move, Board::PIECE_COLOR color) const;
    int maxMoveScore(const Move &move) const;
    int sumPlayerScore(Board::PIECE_COLOR color) const;

    const static int INVALID_MOVE_WEIGHT = -__INT32_MAX__;

   public:
    void sortMoves();
    void eraseKillMove(const Move &move);
    void addKillMove(const Move &move);

    std::vector<Move> m_moves;
    std::vector<Move> m_killMoves;
    int m_killMovePos[Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_recorded[Board::BOARD_SIZE][Board::BOARD_SIZE];
    Scorer::Type m_dirType[2][4][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_playerMoveScore[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_maxScore[Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_cntS4[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_cntL3[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_sumPlayerScore[2] = {0, 0};
    bool m_genKillFlag = false;
};

#endif