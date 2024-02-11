#include "generator.h"

#include <algorithm>

#include "scorer.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))

MoveGenerator::MoveGenerator() {
    for (int i = 0; i < Board::BOARD_SIZE; ++i) {
        for (int j = 0; j < Board::BOARD_SIZE; ++j) {
            m_maxScore[i][j] = INVALID_MOVE_WEIGHT;
            m_recorded[i][j] = 0;
        }
    }
}

void MoveGenerator::sortMoves() {
    std::sort(m_moves.begin(), m_moves.end(), [&](const Move &a, const Move &b) {
        return m_maxScore[a.x][a.y] > m_maxScore[b.x][b.y];
    });

    // remove erased moves
    while (m_moves.size() > 0) {
        const Move &last = m_moves.back();
        if (m_maxScore[last.x][last.y] != INVALID_MOVE_WEIGHT) break;
        m_moves.pop_back();
        m_recorded[last.x][last.y] = 0;
    }
}

void MoveGenerator::updateMoveScoreByDir(const Move &move, int dir, int w,
                                         Board::CHESS_COLOR player) {
    m_sumPlayerScore[player] -= m_playerMoveScore[player][move.x][move.y];

    m_playerMoveScore[player][move.x][move.y] -= m_dirScore[player][dir][move.x][move.y];
    if (m_dirScore[player][dir][move.x][move.y] >=
            Scorer::TYPE_SCORES[Scorer::SLEEP_FOUR] ||
        m_dirScore[player][dir][move.x][move.y] >=
            Scorer::TYPE_SCORES[Scorer::LIVE_THREE]) {
        m_cntKill[player][move.x][move.y]--;
        if (m_cntKill[player][move.x][move.y] == 1) {
            m_playerMoveScore[player][move.x][move.y] -=
                Scorer::TYPE_SCORES[Scorer::KILL];
        }
    }

    m_dirScore[player][dir][move.x][move.y] = w;
    m_playerMoveScore[player][move.x][move.y] += w;
    if (w >= Scorer::TYPE_SCORES[Scorer::SLEEP_FOUR] ||
        w >= Scorer::TYPE_SCORES[Scorer::LIVE_THREE]) {
        m_cntKill[player][move.x][move.y]++;
        if (m_cntKill[player][move.x][move.y] == 2) {
            m_playerMoveScore[player][move.x][move.y] +=
                Scorer::TYPE_SCORES[Scorer::KILL];
        }
    }

    m_sumPlayerScore[player] += m_playerMoveScore[player][move.x][move.y];
    m_maxScore[move.x][move.y] = max(m_playerMoveScore[player][move.x][move.y],
                                     m_playerMoveScore[player ^ 1][move.x][move.y]);
}

void MoveGenerator::addMove(const Move &move) {
    if (!m_recorded[move.x][move.y]) {
        m_moves.push_back(move);
        m_recorded[move.x][move.y] = 1;
    }

    int baseScore = Scorer::BASE_SCORES[move.x][move.y];

    for (int i = 0; i < 4; i++) {
        m_dirScore[Board::CHESS_COLOR::BLACK][i][move.x][move.y] =
            m_dirScore[Board::CHESS_COLOR::WHITE][i][move.x][move.y] = 0;
    }

    m_playerMoveScore[Board::CHESS_COLOR::BLACK][move.x][move.y] =
        m_playerMoveScore[Board::CHESS_COLOR::WHITE][move.x][move.y] = baseScore;

    m_maxScore[move.x][move.y] = baseScore;

    m_cntKill[Board::CHESS_COLOR::BLACK][move.x][move.y] =
        m_cntKill[Board::CHESS_COLOR::WHITE][move.x][move.y] = 0;

    m_sumPlayerScore[Board::CHESS_COLOR::BLACK] += baseScore;
    m_sumPlayerScore[Board::CHESS_COLOR::WHITE] += baseScore;
}

void MoveGenerator::eraseMove(const Move &move) {
    m_sumPlayerScore[Board::CHESS_COLOR::BLACK] -=
        m_playerMoveScore[Board::CHESS_COLOR::BLACK][move.x][move.y];
    m_sumPlayerScore[Board::CHESS_COLOR::WHITE] -=
        m_playerMoveScore[Board::CHESS_COLOR::WHITE][move.x][move.y];

    m_maxScore[move.x][move.y] = INVALID_MOVE_WEIGHT;
}

std::vector<MoveGenerator::Move> MoveGenerator::generateMovesList(int cnt) {
    sortMoves();
    return cnt < m_moves.size()
               ? std::vector<Move>(m_moves.begin(), m_moves.begin() + cnt)
               : m_moves;
}

bool MoveGenerator::existsMove(const Move &move) {
    return m_maxScore[move.x][move.y] != INVALID_MOVE_WEIGHT;
}

int MoveGenerator::playerMoveScore(const Move &move, Board::CHESS_COLOR color) const {
    return m_playerMoveScore[color][move.x][move.y];
}

int MoveGenerator::maxMoveScore(const Move &move) const {
    return m_maxScore[move.x][move.y];
}

int MoveGenerator::sumPlayerScore(Board::CHESS_COLOR color) const {
    return m_sumPlayerScore[color];
}