#ifndef TT_H
#define TT_H

#include "board.h"

/** Transposition Table */
class TT {
   public:
    enum Flag { EMPTY, EXACT, LOWER, UPPER };

    struct Item {
        int depth;
        unsigned long long hash;
        int value;
        Flag flag = EMPTY;
    };

    TT();
    ~TT();

    int find(unsigned long long hash, int depth, int alpha, int beta,
             Board::CHESS_COLOR) const;
    void insert(unsigned long long hash, int depth, int value, Flag,
                Board::CHESS_COLOR);

    const static int LENGTH = 1 << 20;
    const static int TT_NOT_HIT = __INT32_MAX__ - 1;

   private:
    int getHashIndex(unsigned long long hash) const { return hash & 0xFFFFF; }

    Item *m_pTable[2] = {nullptr, nullptr};
};

#endif