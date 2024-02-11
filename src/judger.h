#ifndef JUDGER_H
#define JUDGER_H

#include "core.h"

class Judger {
   public:
    enum MODE { ONLINE_JUDGE = 0, COMMAND_LINE = 1 };

    Judger() {}
    ~Judger();

    static void setMode(MODE mode) { JUDGER_MODE = mode; }

    void printCoreMoveByJSON();
    void initGameByJSON();
    void startGame();
    bool checkFiveAt(int x, int y, Board::CHESS_COLOR);

    static MODE JUDGER_MODE;

   private:
    Core *m_pCore = nullptr;
    Board *m_pBoard = nullptr;
};

#endif