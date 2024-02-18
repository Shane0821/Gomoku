#ifndef TT_H
#define TT_H

#include "board.h"

/**
 * @class TT
 * @brief Transposition Table class for storing and retrieving game positions.
 *
 * The TT class provides a transposition table implementation for storing and retrieving
 * game positions. It is used to cache previously computed positions to improve the
 * efficiency of game search algorithms.
 */
class TT {
   public:
    /**
     * @enum Flag
     * @brief Enumeration for different types of transposition table entries.
     */
    enum Flag { EMPTY, EXACT, LOWER, UPPER };

    /**
     * @struct Item
     * @brief Structure representing a transposition table entry.
     */
    struct Item {
        int depth;               /**< Depth of the search when the entry was stored. */
        unsigned long long hash; /**< Hash value of the game position. */
        int value;               /**< Evaluation value of the game position. */
        Flag flag = EMPTY;       /**< Flag indicating the type of the entry. */
    };

    /**
     * @brief Default constructor for the TT class.
     */
    TT();

    /**
     * @brief Destructor for the TT class.
     */
    ~TT();

    /**
     * @brief Finds a game position in the transposition table.
     * @param hash The hash value of the game position.
     * @param depth The depth of the search.
     * @param alpha The lower bound of the search window.
     * @param beta The upper bound of the search window.
     * @param color The color of the current player.
     * @return The stored evaluation value if found, otherwise TT_NOT_HIT.
     */
    int find(unsigned long long hash, int depth, int alpha, int beta,
             Board::PIECE_COLOR color) const;

    /**
     * @brief Inserts a game position into the transposition table.
     * @param hash The hash value of the game position.
     * @param depth The depth of the search.
     * @param value The evaluation value of the game position.
     * @param flag The type of the entry.
     * @param color The color of the current player.
     */
    void insert(unsigned long long hash, int depth, int value, Flag flag,
                Board::PIECE_COLOR color);

    /**
     * @brief Value indicating that a transposition table entry was not found.
     */
    const static int TT_NOT_HIT = __INT32_MAX__ - 1;

   private:
    /**
     * @brief Calculates the hash index for a given hash value.
     * @param hash The hash value of the game position.
     * @return The hash index.
     */
    int getHashIndex(unsigned long long hash) const { return hash & 0xFFFFF; }

    /**
     * @brief Length of the transposition table.
     */
    const static int LENGTH = 1 << 20;

    Item *m_pTable[2] = {nullptr, nullptr}; /**< Array of transposition table entries. */
};

#endif