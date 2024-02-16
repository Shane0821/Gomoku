#ifndef SCORER_H
#define SCORER_H

#include "board.h"

class Scorer {
   public:
    enum Type {
        FIVE = 0,
        LIVE_FOUR = 1,
        KILL_1 = 2,
        KILL_2 = 3,
        SLEEP_FOUR = 4,
        LIVE_THREE = 5,
        SLEEP_THREE = 6,
        LIVE_TWO = 7,
        SLEEP_TWO = 8,
        BASE = 9
    };

    Scorer();
    ~Scorer() { delete[] m_pTypeTable; }

    Type getTypeByLineState(int state) const { return m_pTypeTable[state]; }

    const static int CNT_STATES = 19683;
    const static int CNT_TYPES = 10;
    const static int TYPE_SCORES[CNT_TYPES];
    const static int BASE_SCORES[Board::BOARD_SIZE][Board::BOARD_SIZE];

   private:
    Type *m_pTypeTable;
};

#endif