#ifndef BOARD_H
#define BOARD_H

class Board {
   public:
    enum CHESS_COLOR {
        WHITE = 0,
        BLACK = 1,
    };

    enum BOARD_STATE {
        INVALID = -1,
        WHITE_CHESS = CHESS_COLOR::WHITE,
        BLACK_CHESS = CHESS_COLOR::BLACK,
        UNPLACE = 2
    };

    Board();

    /** Display Board */
    void display() const;

    /** Update state at (x, y) */
    void placeAt(int x, int y, CHESS_COLOR color);

    void unplaceAt(int x, int y);

    /** Get the state of (x, y) */
    BOARD_STATE getState(int x, int y) const;

    int cntNeighbour(int x, int y) const { return m_cntNeighbour[x][y]; }

    const static int BOARD_SIZE = 15;
    const static int dr[4];
    const static int dc[4];

   private:
    BOARD_STATE m_boardState[BOARD_SIZE][BOARD_SIZE];
    int m_cntNeighbour[BOARD_SIZE][BOARD_SIZE];
};

#endif