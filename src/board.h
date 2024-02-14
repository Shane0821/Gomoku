#ifndef BOARD_H
#define BOARD_H

class Zobrist;

class Board {
   public:
    enum PIECE_COLOR {
        WHITE = 0,
        BLACK = 1,
    };

    enum BOARD_STATE {
        INVALID = -1,
        WHITE_PIECE = PIECE_COLOR::WHITE,
        BLACK_PIECE = PIECE_COLOR::BLACK,
        UNPLACE = 2
    };

    Board();
    ~Board();

    /** Display Board */
    void display() const;

    /** Update state at (x, y) */
    void placeAt(int x, int y, PIECE_COLOR color);

    void unplaceAt(int x, int y);

    /** Get the state of (x, y) */
    BOARD_STATE getState(int x, int y) const;

    int cntNeighbour(int x, int y) const { return m_cntNeighbour[x][y]; }

    unsigned long long getBoardHash() const;

    const static int BOARD_SIZE = 15;
    const static int dr[4];
    const static int dc[4];

   private:
    Zobrist *m_pZobristHash = nullptr;
    BOARD_STATE m_boardState[BOARD_SIZE][BOARD_SIZE];
    int m_cntNeighbour[BOARD_SIZE][BOARD_SIZE];
};

#endif