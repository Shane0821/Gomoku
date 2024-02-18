#include "generator.h"

#include <algorithm>

#include "scorer.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))

MoveGenerator::MoveGenerator() {
    for (int i = 0; i < Board::BOARD_SIZE; ++i) {
        for (int j = 0; j < Board::BOARD_SIZE; ++j) {
            m_maxScore[i][j] = INVALID_MOVE_WEIGHT;
            m_recorded[i][j] = 0;
            m_killMovePos[i][j] = -1;
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

void MoveGenerator::updateMoveScoreByDir(const Move &move, int dir, Scorer::Type type,
                                         Board::PIECE_COLOR player) {
    if (type == m_dirType[player][dir][move.x][move.y]) return;

    Scorer::Type preType = m_dirType[player][dir][move.x][move.y];
    m_dirType[player][dir][move.x][move.y] = type;

    int dw = Scorer::TYPE_SCORES[type] - Scorer::TYPE_SCORES[preType];

    if (preType == Scorer::SLEEP_FOUR) {
        m_cntS4[player][move.x][move.y]--;

        if (m_cntS4[player][move.x][move.y]) {
            dw -= Scorer::TYPE_SCORES[Scorer::KILL_1];
        }

        if (m_cntL3[player][move.x][move.y]) {
            dw -= Scorer::TYPE_SCORES[Scorer::KILL_1];
        }
    } else if (preType == Scorer::LIVE_THREE) {
        m_cntL3[player][move.x][move.y]--;

        if (m_cntS4[player][move.x][move.y]) {
            dw -= Scorer::TYPE_SCORES[Scorer::KILL_1];
        }

        if (m_cntL3[player][move.x][move.y]) {
            dw -= Scorer::TYPE_SCORES[Scorer::KILL_2];
        }
    }

    if (type == Scorer::SLEEP_FOUR) {
        if (m_cntS4[player][move.x][move.y]) {
            dw += Scorer::TYPE_SCORES[Scorer::KILL_1];
        }

        if (m_cntL3[player][move.x][move.y]) {
            dw += Scorer::TYPE_SCORES[Scorer::KILL_1];
        }

        m_cntS4[player][move.x][move.y]++;
    } else if (type == Scorer::LIVE_THREE) {
        if (m_cntS4[player][move.x][move.y]) {
            dw += Scorer::TYPE_SCORES[Scorer::KILL_1];
        }

        if (m_cntL3[player][move.x][move.y]) {
            dw += Scorer::TYPE_SCORES[Scorer::KILL_2];
        }

        m_cntL3[player][move.x][move.y]++;
    }

    m_playerMoveScore[player][move.x][move.y] += dw;
    m_sumPlayerScore[player] += dw;

    int preMaxScore = m_maxScore[move.x][move.y];
    int newMaxScore = max(m_playerMoveScore[Board::PIECE_COLOR::WHITE][move.x][move.y],
                          m_playerMoveScore[Board::PIECE_COLOR::BLACK][move.x][move.y]);

    m_maxScore[move.x][move.y] = newMaxScore;

    if (preMaxScore < Scorer::TYPE_SCORES[Scorer::KILL_2] &&
        newMaxScore >= Scorer::TYPE_SCORES[Scorer::KILL_2]) {
        addKillMove(move);
    } else if (preMaxScore >= Scorer::TYPE_SCORES[Scorer::KILL_2] &&
               newMaxScore < Scorer::TYPE_SCORES[Scorer::KILL_2]) {
        eraseKillMove(move);
    }
}

void MoveGenerator::addMove(const Move &move) {
    if (!m_recorded[move.x][move.y]) {
        m_moves.push_back(move);
        m_recorded[move.x][move.y] = 1;
    }

    int baseScore = Scorer::BASE_SCORES[move.x][move.y];

    for (int i = 0; i < 4; i++) {
        m_dirType[Board::PIECE_COLOR::BLACK][i][move.x][move.y] =
            m_dirType[Board::PIECE_COLOR::WHITE][i][move.x][move.y] = Scorer::BASE;
    }

    m_playerMoveScore[Board::PIECE_COLOR::BLACK][move.x][move.y] =
        m_playerMoveScore[Board::PIECE_COLOR::WHITE][move.x][move.y] = baseScore;

    m_maxScore[move.x][move.y] = baseScore;

    m_cntL3[Board::PIECE_COLOR::BLACK][move.x][move.y] =
        m_cntL3[Board::PIECE_COLOR::WHITE][move.x][move.y] = 0;

    m_cntS4[Board::PIECE_COLOR::BLACK][move.x][move.y] =
        m_cntS4[Board::PIECE_COLOR::WHITE][move.x][move.y] = 0;

    m_sumPlayerScore[Board::PIECE_COLOR::BLACK] += baseScore;
    m_sumPlayerScore[Board::PIECE_COLOR::WHITE] += baseScore;
}

void MoveGenerator::eraseMove(const Move &move) {
    m_sumPlayerScore[Board::PIECE_COLOR::BLACK] -=
        m_playerMoveScore[Board::PIECE_COLOR::BLACK][move.x][move.y];
    m_sumPlayerScore[Board::PIECE_COLOR::WHITE] -=
        m_playerMoveScore[Board::PIECE_COLOR::WHITE][move.x][move.y];

    m_maxScore[move.x][move.y] = INVALID_MOVE_WEIGHT;

    if (m_killMovePos[move.x][move.y] != -1) {
        eraseKillMove(move);
    }
}

void MoveGenerator::eraseKillMove(const Move &move) {
    m_killMovePos[m_killMoves.back().x][m_killMoves.back().y] =
        m_killMovePos[move.x][move.y];
    std::swap(m_killMoves[m_killMovePos[move.x][move.y]], m_killMoves.back());
    m_killMovePos[move.x][move.y] = -1;
    m_killMoves.pop_back();
}

void MoveGenerator::addKillMove(const Move &move) {
    m_killMovePos[move.x][move.y] = m_killMoves.size();
    m_killMoves.push_back(move);
}

std::vector<MoveGenerator::Move> MoveGenerator::generateMovesList(int cnt) {
    sortMoves();
    return cnt < m_moves.size()
               ? std::vector<Move>(m_moves.begin(), m_moves.begin() + cnt)
               : m_moves;
}

std::vector<MoveGenerator::Move> MoveGenerator::generateKillMovesList() {
    return m_killMoves;
}

bool MoveGenerator::existsMove(const Move &move) {
    return m_maxScore[move.x][move.y] != INVALID_MOVE_WEIGHT;
}

int MoveGenerator::playerMoveScore(const Move &move, Board::PIECE_COLOR color) const {
    return m_playerMoveScore[color][move.x][move.y];
}

int MoveGenerator::maxMoveScore(const Move &move) const {
    return m_maxScore[move.x][move.y];
}

int MoveGenerator::sumPlayerScore(Board::PIECE_COLOR color) const {
    return m_sumPlayerScore[color];
}