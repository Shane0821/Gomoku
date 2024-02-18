#ifndef JUDGER_H
#define JUDGER_H

#include "core.h"

/**
 * @class Judger
 * @brief The Judger class is responsible for managing the game logic and determining the
 * winner in a Gomoku game.
 */
class Judger {
   public:
    enum MODE { ONLINE_JUDGE = 0, COMMAND_LINE = 1 };

    /**
     * @brief Sets the mode of the Judger.
     * @param mode The mode to set (either ONLINE_JUDGE or COMMAND_LINE).
     */
    static void setMode(MODE mode) { JUDGER_MODE = mode; }

    /**
     * @brief Default constructor for Judger.
     */
    Judger() = default;

    /**
     * @brief Destructor for Judger.
     */
    ~Judger();

    /**
     * @brief Prints the core move in JSON format.
     */
    void printCoreMoveByJSON();

    /**
     * @brief Initializes the game by reading JSON input.
     */
    void initGameByJSON();

    /**
     * @brief Starts the game.
     */
    void startGame();

    /**
     * @brief Checks if there are five pieces of the same color in a row, column, or
     * diagonal starting from the given position.
     * @param x The x-coordinate of the starting position.
     * @param y The y-coordinate of the starting position.
     * @param color The color of the pieces to check.
     * @return True if there are five pieces of the same color, false otherwise.
     */
    bool checkFiveAt(int x, int y, Board::PIECE_COLOR color);

    /**
     * @brief The mode of the Judger.
     */
    static MODE JUDGER_MODE;

   private:
    Core *m_pCore = nullptr;   /**< The core for making moves */
    Board *m_pBoard = nullptr; /**< The board for the game  */
};

#endif