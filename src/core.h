#ifndef CORE_H
#define CORE_H

#include "board.h"
#include "generator.h"
#include "hash.h"
#include "scorer.h"
#include "timer.h"
#include "tt.h"

class Core {
   public:
    Core(Board *pBoard);
    ~Core() {}

    void setColor(Board::PIECE_COLOR color) { m_color = color; }
    MoveGenerator::Move bestMove() const { return m_bestMove; }
    int bestScore() const { return m_bestScore; }
    int run();
    void makeMove(int x, int y, Board::PIECE_COLOR);
    void cancelMove(int x, int y);

    static int MIN_SEARCH_DEPTH;
    static int MAX_SEARCH_DEPTH;
    static int KILL_DEPTH;
    static bool ITERATIVE_DEEPENING;
    static int BRANCH_FACTOR;
    const static int INF = __INT32_MAX__ - 100;
    const static int TIME_LIMIT = 3980;

   private:
    int negMiniMaxSearch(int depth, Board::PIECE_COLOR player, int alpha, int beta);
    int maxKillSearch(int depth);
    int minKillSearch(int depth);
    void updateMoveAt(int x, int y, Board::PIECE_COLOR);
    void updateMoveAt(int x, int y, int dir, Board::PIECE_COLOR);
    void updateMoveAround(int x, int y, Board::PIECE_COLOR);

    int evaluate() const;

    Board *m_pBoard = nullptr;

    Timer m_timer;
    TT m_TT;
    MoveGenerator m_moveGenerator;
    Scorer m_scorer;

    Board::PIECE_COLOR m_color = Board::PIECE_COLOR::WHITE;

    MoveGenerator::Move m_bestMove;
    int m_bestScore = -__INT32_MAX__;
    int iterativeDepth = 4;
};

#endif