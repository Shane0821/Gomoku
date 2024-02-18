#ifndef HASH_H
#define HASH_H

#include "board.h"

/**
 * @class Zobrist
 * @brief Class for generating and updating Zobrist hash values for a game board.
 */
class Zobrist {
   public:
    /**
     * @brief Generates a random number for Zobrist hashing.
     * @return The generated random number.
     */
    static unsigned long long generateRandomNumber();

    /**
     * @brief Constructs a Zobrist object.
     */
    Zobrist();

    /**
     * @brief Updates the Zobrist hash value based on the given move.
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     * @param color The color of the piece placed on the board.
     */
    void update(int x, int y, Board::PIECE_COLOR color);

    /**
     * @brief Gets the current Zobrist hash value of the game board.
     * @return The Zobrist hash value.
     */
    unsigned long long getBoardHash() const { return m_boardHash; }

   private:
    unsigned long long
        m_hashTable[2][Board::BOARD_SIZE]
                   [Board::BOARD_SIZE];  // Hash table for storing random numbers
    unsigned long long m_boardHash;      // Current Zobrist hash value of the game board
};

#endif