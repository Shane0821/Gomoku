#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>

#include "board.h"
#include "scorer.h"

/**
 * @class MoveGenerator
 * @brief Generates and manages moves for the Gomoku game.
 */
class MoveGenerator {
   public:
    /**
     * @struct Move
     * @brief Represents a move in the game.
     */
    struct Move {
        int x = -1; /**< The x-coordinate of the move. */
        int y = -1; /**< The y-coordinate of the move. */

        /**
         * @brief Overloaded equality operator for comparing moves.
         * @param other The move to compare with.
         * @return True if the moves are equal, false otherwise.
         */
        bool operator==(const Move &other) const { return x == other.x && y == other.y; }
    };

    /**
     * @brief Default constructor for MoveGenerator.
     */
    MoveGenerator();

    /**
     * @brief Sorts the moves in the move list.
     */
    void sortMoves();

    /**
     * @brief Updates the move score based on the direction, scorer type, and piece color.
     * @param move The move to update the score for.
     * @param dir The direction of the move.
     * @param type The scorer type.
     * @param color The piece color.
     */
    void updateMoveScoreByDir(const Move &move, int dir, Scorer::Type,
                              Board::PIECE_COLOR);

    /**
     * @brief Adds a move to the move list.
     * @param move The move to add.
     */
    void addMove(const Move &move);

    /**
     * @brief Erases a move from the move list.
     * @param move The move to erase.
     */
    void eraseMove(const Move &move);

    /**
     * @brief Checks if a move exists in the move list.
     * @param move The move to check.
     * @return True if the move exists, false otherwise.
     */
    bool existsMove(const Move &move);

    /**
     * @brief Generates a list of moves.
     * @param cnt The number of moves to generate.
     * @return The generated list of moves.
     */
    std::vector<Move> generateMovesList(int cnt);

    /**
     * @brief Calculates the score of a player's move.
     * @param move The move to calculate the score for.
     * @param color The player's piece color.
     * @return The score of the player's move.
     */
    int playerMoveScore(const Move &move, Board::PIECE_COLOR color) const;

    /**
     * @brief Calculates the maximum score of a move.
     * @param move The move to calculate the maximum score for.
     * @return The maximum score of the move.
     */
    int maxMoveScore(const Move &move) const;

    /**
     * @brief Calculates the sum of a player's scores.
     * @param color The player's piece color.
     * @return The sum of the player's scores.
     */
    int sumPlayerScore(Board::PIECE_COLOR color) const;

    const static int INVALID_MOVE_WEIGHT =
        -__INT32_MAX__; /**< The weight assigned to an invalid move. */

   public:
    std::vector<Move> m_moves; /**< The list of moves. */
    int m_recorded[Board::BOARD_SIZE]
                  [Board::BOARD_SIZE]; /**< The recorded moves on the board. */
    Scorer::Type m_dirType[2][4][Board::BOARD_SIZE]
                          [Board::BOARD_SIZE]; /**< The scorer types for each direction on
                                                  the board. */
    int m_playerMoveScore[2][Board::BOARD_SIZE]
                         [Board::BOARD_SIZE]; /**< The scores of the player's moves. */
    int m_maxScore[Board::BOARD_SIZE]
                  [Board::BOARD_SIZE]; /**< The maximum scores of moves. */
    int m_cntS4[2][Board::BOARD_SIZE]
               [Board::BOARD_SIZE]; /**< The count of S4 patterns for each move of each
                                       player. */
    int m_cntL3[2][Board::BOARD_SIZE]
               [Board::BOARD_SIZE];   /**< The count of L3 patterns for each move of each
                                         player. */
    int m_sumPlayerScore[2] = {0, 0}; /**< The sum of each player's scores. */
};

#endif