#include "core.h"

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) >= (b) ? (a) : (b))

bool Core::ITERATIVE_DEEPENING = true;
int Core::BRANCH_FACTOR = 25;
int Core::MIN_SEARCH_DEPTH = 4;
int Core::MAX_SEARCH_DEPTH = 10;
int Core::KILL_DEPTH = 4;

Core::Core(Board *pBoard, Board::PIECE_COLOR color) : m_pBoard(pBoard), m_color(color) {
    if (!pBoard) return;

    for (int i = 0; i < Board::BOARD_SIZE; ++i) {
        for (int j = 0; j < Board::BOARD_SIZE; ++j) {
            // only consider the points around the placed points
            if (pBoard->getState(i, j) == Board::UNPLACE &&
                pBoard->cntNeighbour(i, j) > 0) {
                m_moveGenerator.addMove({i, j});
                updateMoveAt(i, j, Board::PIECE_COLOR::BLACK);
                updateMoveAt(i, j, Board::PIECE_COLOR::WHITE);
            }
        }
    }
}

int Core::maxKillSearch(int depth) {
    if (depth == 0) {
        return 0;
    }

    if (m_timer.getTimePass() >= TIME_LIMIT) {
        return Timer::TIME_OUT;
    }

    auto moves = m_moveGenerator.generateKillMovesList();

    for (auto &move : moves) {
        auto blackScore =
            m_moveGenerator.playerMoveScore(move, Board::PIECE_COLOR::BLACK);
        if (blackScore >= Scorer::TYPE_SCORES[Scorer::FIVE]) {
            return INF + depth;
        } else if (blackScore < Scorer::TYPE_SCORES[Scorer::KILL_2]) {
            continue;
        }
        makeMove(move.x, move.y, Board::PIECE_COLOR::BLACK);
        int val = minKillSearch(depth - 1);
        cancelMove(move.x, move.y);
        if (val == Timer::TIME_OUT) return Timer::TIME_OUT;
        if (val) return val;
    }
    return 0;
}

int Core::minKillSearch(int depth) {
    if (m_timer.getTimePass() >= TIME_LIMIT) {
        return Timer::TIME_OUT;
    }

    auto moves = m_moveGenerator.generateKillMovesList();
    int minVal = INF + KILL_DEPTH;
    for (auto &move : moves) {
        if (m_moveGenerator.playerMoveScore(move, Board::PIECE_COLOR::WHITE) >=
            Scorer::TYPE_SCORES[Scorer::FIVE]) {
            return 0;
        }
        makeMove(move.x, move.y, Board::PIECE_COLOR::WHITE);
        int val = maxKillSearch(depth - 1);
        cancelMove(move.x, move.y);
        if (val == Timer::TIME_OUT) return Timer::TIME_OUT;
        if (val == 0) return 0;
        if (val < minVal) minVal = val;
    }
    return minVal;
}

int Core::negMiniMaxSearch(int depth, Board::PIECE_COLOR player, int alpha, int beta) {
    if (depth == 0) {
        int val = maxKillSearch(KILL_DEPTH);

        if (val == 0 || val == Timer::TIME_OUT) val = evaluate();

        m_TT.insert(m_pBoard->getBoardHash(), depth, val, TT::EXACT, player);
        return val;
    }

    if (m_timer.getTimePass() >= TIME_LIMIT) {
        return Timer::TIME_OUT;
    }

    if (depth != iterativeDepth) {
        int val = m_TT.find(m_pBoard->getBoardHash(), depth, alpha, beta, player);
        if (val != TT::TT_NOT_HIT) {
            return val;
        }
    }

    TT::Flag flag = TT::UPPER;

    std::vector<MoveGenerator::Move> moves =
        m_moveGenerator.generateMovesList(BRANCH_FACTOR);
    int cntMoves = moves.size();
    Board::PIECE_COLOR opponent = static_cast<Board::PIECE_COLOR>(player ^ 1);

    bool opponentHasFive = false;
    int i = 0;
    while (i < cntMoves) {
        // only consider the moves with score higher than FIVE
        auto &move = moves[i];
        int maxMoveScore = m_moveGenerator.maxMoveScore(move);
        if (maxMoveScore < Scorer::TYPE_SCORES[Scorer::FIVE]) {
            break;
        }
        int playerMoveScore = m_moveGenerator.playerMoveScore(move, player);
        if (playerMoveScore >= Scorer::TYPE_SCORES[Scorer::FIVE]) {
            // win
            int val = INF + depth + KILL_DEPTH;

            if (depth == iterativeDepth && val > m_bestScore) {
                m_bestMove = move;
                m_bestScore = val;
            }
            if (val >= beta) {
                m_TT.insert(m_pBoard->getBoardHash(), depth, beta, TT::LOWER, player);
                return val;
            }
            if (val > alpha) {
                flag = TT::EXACT;
                alpha = val;
            }
            m_TT.insert(m_pBoard->getBoardHash(), depth, alpha, flag, player);
            return alpha;
        } else {
            opponentHasFive = true;
        }
        i++;
    }

    if (opponentHasFive) {
        // must block the opponent's FIVE
        i--;

        auto &move = moves[i];

        makeMove(move.x, move.y, player);
        int val = -negMiniMaxSearch(depth - 1, opponent, -beta, -alpha);
        cancelMove(move.x, move.y);

        if (val == -Timer::TIME_OUT) return Timer::TIME_OUT;

        if (depth == iterativeDepth && val > m_bestScore) {
            m_bestMove = move;
            m_bestScore = val;
        }
        if (val >= beta) {
            m_TT.insert(m_pBoard->getBoardHash(), depth, beta, TT::LOWER, player);
            return val;
        }
        if (val > alpha) {
            flag = TT::EXACT;
            alpha = val;
        }
    } else {
        bool fFoundPv = false;
        while (i < cntMoves) {
            auto &move = moves[i];

            int val = alpha;

            if (m_moveGenerator.playerMoveScore(move, player) >=
                Scorer::TYPE_SCORES[Scorer::KILL_1]) {
                val = INF + depth + KILL_DEPTH - 1;

                if (depth == iterativeDepth && val > m_bestScore) {
                    m_bestMove = move;
                    m_bestScore = val;
                }
                if (val >= beta) {
                    m_TT.insert(m_pBoard->getBoardHash(), depth, beta, TT::LOWER, player);
                    return val;
                }
                if (val > alpha) {
                    flag = TT::EXACT;
                    fFoundPv = true;
                    alpha = val;
                }
                m_TT.insert(m_pBoard->getBoardHash(), depth, alpha, flag, player);
                return alpha;
            } else {
                makeMove(move.x, move.y, player);
                if (fFoundPv) {
                    val = -negMiniMaxSearch(depth - 1, opponent, -alpha - 1, -alpha);
                    if ((val > alpha) && (val < beta)) {
                        val = -negMiniMaxSearch(depth - 1, opponent, -beta, -alpha);
                    }
                } else {
                    val = -negMiniMaxSearch(depth - 1, opponent, -beta, -alpha);
                }
                cancelMove(move.x, move.y);
                if (val == -Timer::TIME_OUT) return Timer::TIME_OUT;
            }

            if (depth == iterativeDepth && val > m_bestScore) {
                m_bestMove = move;
                m_bestScore = val;
            }
            if (val >= beta) {
                m_TT.insert(m_pBoard->getBoardHash(), depth, beta, TT::LOWER, player);
                return val;
            }
            if (val > alpha) {
                flag = TT::EXACT;
                fFoundPv = true;
                alpha = val;
            }

            i++;
        }
    }

    m_TT.insert(m_pBoard->getBoardHash(), depth, alpha, flag, player);
    return alpha;
}

int Core::run() {
    if (!m_pBoard) return -1;

    // if core is white, we search for odd depth, so that evaluation is done at black
    // player's point of view
    iterativeDepth = MIN_SEARCH_DEPTH + 1 - m_color;

    if (ITERATIVE_DEEPENING) {
        m_bestMove = {-1, -1};
        int prevBestScore = -__INT32_MAX__;
        MoveGenerator::Move prevBestMove = {-1, -1};
        for (; iterativeDepth <= MAX_SEARCH_DEPTH + 1 - m_color; iterativeDepth += 2) {
            m_bestScore = -__INT32_MAX__;
            int val = negMiniMaxSearch(iterativeDepth, m_color,
                                       -INF - iterativeDepth - KILL_DEPTH,
                                       INF + iterativeDepth + KILL_DEPTH);
            if (val == Timer::TIME_OUT) {
                m_bestScore = prevBestScore;
                m_bestMove = prevBestMove;
                break;
            } else if (val >= INF) {
                break;
            }
            prevBestScore = m_bestScore;
            prevBestMove = m_bestMove;
        }
    } else {
        m_bestMove = {-1, -1};
        m_bestScore = -__INT32_MAX__;
        negMiniMaxSearch(iterativeDepth, m_color, -INF - iterativeDepth - KILL_DEPTH,
                         INF + iterativeDepth + KILL_DEPTH);
    }
    return m_timer.getTimePass();
}

void Core::makeMove(int x, int y, Board::PIECE_COLOR player) {
    m_pBoard->placeAt(x, y, player);
    m_moveGenerator.eraseMove({x, y});
    updateMoveAround(x, y, Board::PIECE_COLOR::BLACK);
    updateMoveAround(x, y, Board::PIECE_COLOR::WHITE);
}

void Core::cancelMove(int x, int y) {
    m_pBoard->unplaceAt(x, y);
    m_moveGenerator.addMove({x, y});
    updateMoveAt(x, y, Board::PIECE_COLOR::BLACK);
    updateMoveAt(x, y, Board::PIECE_COLOR::WHITE);
    updateMoveAround(x, y, Board::PIECE_COLOR::BLACK);
    updateMoveAround(x, y, Board::PIECE_COLOR::WHITE);
}

void Core::updateMoveAt(int x, int y, int dir, Board::PIECE_COLOR player) {
    int tx = x, ty = y, cnt2 = 0;
    Board::BOARD_STATE state;

    int lineState = 0, base = 1;
    for (int step = 1; step <= 4; step++) {
        tx -= Board::dr[dir];
        ty -= Board::dc[dir];
        state = m_pBoard->getState(tx, ty);

        if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
            break;
        }

        lineState = lineState + (state == Board::UNPLACE ? 2 : 1) * base;
        base *= 3;

        if (state == Board::BOARD_STATE::UNPLACE) {
            cnt2++;
        } else {
            cnt2 = 0;
        }
        if (cnt2 == 2) {
            break;
        }
    }

    lineState = lineState * 3 + 1;

    tx = x, ty = y, cnt2 = 0;
    for (int step = 1; step <= 4; step++) {
        tx += Board::dr[dir];
        ty += Board::dc[dir];
        state = m_pBoard->getState(tx, ty);

        if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
            break;
        }

        lineState = lineState * 3 + (state == Board::UNPLACE ? 2 : 1);

        if (state == Board::BOARD_STATE::UNPLACE) {
            cnt2++;
        } else {
            cnt2 = 0;
        }
        if (cnt2 == 2) {
            break;
        }
    }

    m_moveGenerator.updateMoveScoreByDir({x, y}, dir,
                                         m_scorer.getTypeByLineState(lineState), player);
}

void Core::updateMoveAt(int x, int y, Board::PIECE_COLOR player) {
    for (int dir = 0; dir < 4; dir++) {
        int tx = x, ty = y, cnt2 = 0;
        Board::BOARD_STATE state;

        int lineState = 0, base = 1;
        for (int step = 1; step <= 4; step++) {
            tx -= Board::dr[dir];
            ty -= Board::dc[dir];
            state = m_pBoard->getState(tx, ty);

            if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
                break;
            }

            lineState = lineState + (state == Board::UNPLACE ? 2 : 1) * base;
            base *= 3;

            if (state == Board::BOARD_STATE::UNPLACE) {
                cnt2++;
            } else {
                cnt2 = 0;
            }
            if (cnt2 == 2) {
                break;
            }
        }

        lineState = lineState * 3 + 1;

        tx = x, ty = y, cnt2 = 0;
        for (int step = 1; step <= 4; step++) {
            tx += Board::dr[dir];
            ty += Board::dc[dir];
            state = m_pBoard->getState(tx, ty);

            if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
                break;
            }

            lineState = lineState * 3 + (state == Board::UNPLACE ? 2 : 1);

            if (state == Board::BOARD_STATE::UNPLACE) {
                cnt2++;
            } else {
                cnt2 = 0;
            }
            if (cnt2 == 2) {
                break;
            }
        }

        m_moveGenerator.updateMoveScoreByDir(
            {x, y}, dir, m_scorer.getTypeByLineState(lineState), player);
    }
}

void Core::updateMoveAround(int x, int y, Board::PIECE_COLOR player) {
    // TODO: use sliding window and only update score in current direction
    for (int dir = 0; dir < 4; dir++) {
        int tx = x, ty = y;
        for (int i = 1; i <= 4; i++) {
            tx += Board::dr[dir];
            ty += Board::dc[dir];

            int state = m_pBoard->getState(tx, ty);

            if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
                break;
            }

            if (state == player) {
                continue;
            }

            if (m_pBoard->cntNeighbour(tx, ty) == 0) {
                if (m_moveGenerator.existsMove({tx, ty})) {
                    m_moveGenerator.eraseMove({tx, ty});
                }
                if (i >= 2) break;
            } else {
                if (!m_moveGenerator.existsMove({tx, ty})) {
                    m_moveGenerator.addMove({tx, ty});
                }
                updateMoveAt(tx, ty, dir, player);
            }
        }

        tx = x, ty = y;
        for (int i = 1; i <= 4; i++) {
            tx -= Board::dr[dir];
            ty -= Board::dc[dir];

            int state = m_pBoard->getState(tx, ty);

            if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
                break;
            }

            if (state == player) {
                continue;
            }

            if (m_pBoard->cntNeighbour(tx, ty) == 0) {
                if (m_moveGenerator.existsMove({tx, ty})) {
                    m_moveGenerator.eraseMove({tx, ty});
                }
                if (i >= 2) break;
            } else {
                if (!m_moveGenerator.existsMove({tx, ty})) {
                    m_moveGenerator.addMove({tx, ty});
                }
                updateMoveAt(tx, ty, dir, player);
            }
        }
    }
}

int Core::evaluate() const {
    return m_moveGenerator.sumPlayerScore(Board::PIECE_COLOR::BLACK) * 5 -
           m_moveGenerator.sumPlayerScore(Board::PIECE_COLOR::WHITE);
}