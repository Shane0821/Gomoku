#ifndef CORE_H
#define CORE_H

#include "board.h"
#include "generator.h"
#include "hash.h"
#include "scorer.h"
#include "timer.h"
#include "tt.h"

/**
 * @class Core
 * @brief The Core class represents the core logic of the Gomoku AI.
 *
 * The Core uses various algorithms and strategies to determine the best move to make.
 */
class Core {
   public:
    /**
     * @brief Constructs a Core object with the specified board.
     *
     * @param pBoard A pointer to the Board object.
     * @param color The color of the core.
     */
    Core(Board *pBoard, Board::PIECE_COLOR);

    /**
     * @brief Destroys the Core object.
     */
    ~Core() {}

    /**
     * @brief Init timer.
     */
    void initTimer() { m_timer.recordCurrent(); }

    /**
     * @brief Gets the best move found by the Core.
     *
     * @return The best move found by the Core.
     */
    MoveGenerator::Move bestMove() const { return m_bestMove; }

    /**
     * @brief Gets the best score found by the Core.
     *
     * @return The best score found by the Core.
     */
    int bestScore() const { return m_bestScore; }

    /**
     * @brief Runs the minmax search logic.
     *
     * @return Core run time.
     */
    int run();

    /**
     * @brief Makes a move on the board.
     *
     * @note This will update MoveGenerator's internal state, as well as the Board's
     * state.
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     * @param color The color of the piece to be placed.
     */
    void makeMove(int x, int y, Board::PIECE_COLOR);

    /**
     * @brief Cancels a move on the board.
     *
     * @note This will update MoveGenerator's internal state, as well as the Board's
     * state.
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     */
    void cancelMove(int x, int y);

    /**
     * @brief The minimum search depth for the negamax algorithm.
     */
    static int MIN_SEARCH_DEPTH;

    /**
     * @brief The maximum search depth for the negamax algorithm.
     */
    static int MAX_SEARCH_DEPTH;

    /**
     * @brief The depth for killer heuristic (not implemented yet).
     */
    static int KILL_DEPTH;

    /**
     * @brief Flag indicating whether to use iterative deepening in the search algorithm.
     */
    static bool ITERATIVE_DEEPENING;

    /**
     * @brief The branch factor used in move generation.
     */
    static int BRANCH_FACTOR;

    /**
     * @brief The maximum score value.
     */
    const static int INF = __INT32_MAX__ - 100;

    /**
     * @brief The time limit for the search algorithm.
     */
    const static int TIME_LIMIT = 5960;

   private:
    /**
     * @brief Performs the negamax search algorithm to find the best move.
     *
     * @param depth The current search depth.
     * @param player The color of the current player.
     * @param alpha The alpha value for alpha-beta pruning.
     * @param beta The beta value for alpha-beta pruning.
     * @return The score of the best move.
     */
    int negMiniMaxSearch(int depth, Board::PIECE_COLOR player, int alpha, int beta);

    /**
     * @brief Updates the move at the specified position on the board.
     *
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     * @param color The color of the piece.
     */
    void updateMoveAt(int x, int y, Board::PIECE_COLOR);

    /**
     * @brief Updates the move at the specified position on the board in the specified
     * direction.
     *
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     * @param dir The direction of the move.
     * @param color The color of the piece.
     */
    void updateMoveAt(int x, int y, int dir, Board::PIECE_COLOR);

    /**
     * @brief Updates the moves around the specified position on the board.
     *
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     * @param color The color of the piece.
     */
    void updateMoveAround(int x, int y, Board::PIECE_COLOR);

    /**
     * @brief Evaluates the current board state.
     *
     * @return The evaluation score.
     */
    int evaluate() const;

    Board *m_pBoard = nullptr;  ///< A pointer to the Board object.

    Timer m_timer;                  ///< The timer object.
    TT m_TT;                        ///< The transposition table object.
    MoveGenerator m_moveGenerator;  ///< The move generator object.
    Scorer m_scorer;                ///< The scorer object.

    MoveGenerator::Move m_bestMove;    ///< The best move found by the Core.
    int m_bestScore = -__INT32_MAX__;  ///< The best score found by the Core.

    Board::PIECE_COLOR m_color = Board::PIECE_COLOR::WHITE;  ///< The color of the core.

    int iterativeDepth = 4;  ///< The current depth of the iterative deepening search.
};

#endif