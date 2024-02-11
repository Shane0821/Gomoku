#ifndef SCORER_H
#define SCORER_H

#include "board.h"

class Scorer {
   public:
    enum Type {
        FIVE = 0,
        LIVE_FOUR = 1,
        KILL = 2,
        SLEEP_FOUR = 3,
        LIVE_THREE = 4,
        SLEEP_THREE = 5,
        LIVE_TWO = 6,
        SLEEP_TWO = 7,
        BASE = 8
    };

    Scorer();
    ~Scorer() { delete[] m_pTypeTable; }

    int getScoreByLineState(int state) const { return TYPE_SCORES[m_pTypeTable[state]]; }

    const static int CNT_STATES = 19683;
    const static int CNT_TYPES = 9;
    const static int TYPE_SCORES[CNT_TYPES];
    const static int BASE_SCORES[Board::BOARD_SIZE][Board::BOARD_SIZE];

   private:
    Type *m_pTypeTable;
};

#endif