#ifndef HASH_H
#define HASH_H

#include "board.h"

class Zobrist {
   public:
    Zobrist();

    static unsigned long long generateRandomNumber();

    void update(int x, int y, Board::PIECE_COLOR);

    unsigned long long getBoardHash() { return m_boardHash; }

   private:
    unsigned long long m_hashTable[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    unsigned long long m_boardHash;
};

#endif