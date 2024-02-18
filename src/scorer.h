#ifndef SCORER_H
#define SCORER_H

#include "board.h"

/**
 * @class Scorer
 * @brief The Scorer class is responsible for scoring different line states in the game of
 * Gomoku.
 */
class Scorer {
   public:
    /**
     * @enum Type
     * @brief Enumeration representing different line state types.
     */
    enum Type {
        FIVE = 0,
        LIVE_FOUR = 1,
        KILL_1 = 2, /**< SLEEP FOUR + SLEEP FOUR / LIVE THREE. */
        KILL_2 = 3, /**< LIVE THREE + LIVE THREE */
        SLEEP_FOUR = 4,
        LIVE_THREE = 5,
        SLEEP_THREE = 6,
        LIVE_TWO = 7,
        SLEEP_TWO = 8,
        BASE = 9
    };

    /**
     * @brief Default constructor for the Scorer class.
     */
    Scorer();

    /**
     * @brief Destructor for the Scorer class.
     */
    ~Scorer() { delete[] m_pTypeTable; }

    /**
     * @brief Retrieves the line state type based on the given line state.
     * @param state The line state.
     * @return The line state type.
     */
    Type getTypeByLineState(int state) const { return m_pTypeTable[state]; }

    /**
     * @brief The total number of possible line states.
     */
    const static int CNT_STATES = 19683;

    /**
     * @brief The total number of line state types.
     */
    const static int CNT_TYPES = 10;

    /**
     * @brief The scores associated with each line state type.
     */
    const static int TYPE_SCORES[CNT_TYPES];

    /**
     * @brief The base scores for each position on the game board.
     */
    const static int BASE_SCORES[Board::BOARD_SIZE][Board::BOARD_SIZE];

   private:
    Type *m_pTypeTable; /**< Pointer to the table that maps line states to line state
                           types. */
};

#endif