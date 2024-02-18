#ifndef BOARD_H
#define BOARD_H

class Zobrist;

/**
 * @class Board
 * @brief Represents a game board for Gomoku.
 *
 * The Board class provides functionality to manage and manipulate the game board.
 * It keeps track of the state of each cell on the board and provides methods to
 * place and remove pieces, retrieve the state of a cell, and display the board.
 */
class Board {
   public:
    /**
     * @enum PIECE_COLOR
     * @brief Enumerates the colors of the game pieces.
     */
    enum PIECE_COLOR {
        WHITE = 0, /**< White game piece */
        BLACK = 1, /**< Black game piece */
    };

    /**
     * @enum BOARD_STATE
     * @brief Enumerates the possible states of a board cell.
     */
    enum BOARD_STATE {
        INVALID = -1,    /**< Out of board */
        WHITE_PIECE = 0, /**< White go piece placed on the cell */
        BLACK_PIECE = 1, /**< Black go piece placed on the cell */
        UNPLACE = 2      /**< Cell is unoccupied */
    };

    /**
     * @brief Default constructor for the Board class.
     */
    Board();

    /**
     * @brief Destructor for the Board class.
     */
    ~Board();

    /**
     * @brief Displays the current state of the board.
     */
    void display() const;

    /**
     * @brief Places a game piece at the specified position on the board.
     * @note This function will call zobrist hash update function and update number of
     * neighboring.
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @param color The color of the game piece to be placed.
     */
    void placeAt(int x, int y, PIECE_COLOR color);

    /**
     * @brief Removes the game piece at the specified position on the board.
     * @note This function will call zobrist hash update function and update number of
     * neighboring.
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     */
    void unplaceAt(int x, int y);

    /**
     * @brief Gets the state of the cell at the specified position on the board.
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return The state of the cell.
     */
    BOARD_STATE getState(int x, int y) const;

    /**
     * @brief Gets the number of neighboring go pieces at the specified position on the
     * board.
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return The number of neighboring game pieces.
     */
    int cntNeighbour(int x, int y) const { return m_cntNeighbour[x][y]; }

    /**
     * @brief Gets the hash value of the current board state.
     * @return The hash value of the board.
     */
    unsigned long long getBoardHash() const;

    /**
     * @brief The size of the game board.
     */
    const static int BOARD_SIZE = 15;

    /**
     * @brief The row offsets for neighboring cells.
     */
    const static int dr[4];

    /**
     * @brief The column offsets for neighboring cells.
     */
    const static int dc[4];

   private:
    Zobrist *m_pZobristHash = nullptr; /**< Pointer to the Zobrist hash generator */
    BOARD_STATE m_boardState[BOARD_SIZE]
                            [BOARD_SIZE]; /**< The state of each cell on the board */
    int m_cntNeighbour[BOARD_SIZE][BOARD_SIZE]; /**< The number of neighboring game pieces
                                                   for each cell */
};

#endif