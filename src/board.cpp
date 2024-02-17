#include "board.h"

#include <iostream>

#include "hash.h"

const int Board::dr[4] = {0, 1, 1, 1};
const int Board::dc[4] = {1, 1, 0, -1};

Board::Board() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++) {
            m_boardState[i][j] = UNPLACE;
            m_cntNeighbour[i][j] = 0;
        }

    for (int i = 6; i <= 8; i++)
        for (int j = 6; j <= 8; j++) {
            m_cntNeighbour[i][j] = 1;
        }

    m_pZobristHash = new Zobrist();
}

Board::~Board() { delete m_pZobristHash; }

Board::BOARD_STATE Board::getState(int x, int y) const {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return BOARD_STATE::INVALID;
    }
    return m_boardState[x][y];
}

void Board::display() const {
    std::cout << "  ";
    for (int i = 0; i < BOARD_SIZE; i++)
        std::cout << char(i < 10 ? (int)i + '0' : 'A' + (int)i - 10) << " ";
    std::cout << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << char(i < 10 ? (int)i + '0' : 'A' + (int)i - 10) << "|";
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (m_boardState[i][j] == BOARD_STATE::UNPLACE)
                std::cout << "* ";
            else
                std::cout << (int)m_boardState[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Board::placeAt(int x, int y, Board::PIECE_COLOR color) {
    m_boardState[x][y] = static_cast<BOARD_STATE>(color);
    m_pZobristHash->update(x, y, color);

    for (int k = 0; k < 4; k++) {
        int tx = x;
        int ty = y;
        for (int step = 1; step <= 2; step++) {
            tx += dr[k];
            ty += dc[k];
            if (getState(tx, ty) == INVALID) break;
            m_cntNeighbour[tx][ty]++;
        }

        tx = x;
        ty = y;
        for (int step = 1; step <= 2; step++) {
            tx -= dr[k];
            ty -= dc[k];
            if (getState(tx, ty) == INVALID) break;
            m_cntNeighbour[tx][ty]++;
        }
    }
}

void Board::unplaceAt(int x, int y) {
    m_pZobristHash->update(x, y, static_cast<PIECE_COLOR>(m_boardState[x][y]));
    m_boardState[x][y] = Board::BOARD_STATE::UNPLACE;

    for (int k = 0; k < 4; k++) {
        int tx = x;
        int ty = y;
        for (int step = 1; step <= 2; step++) {
            tx += dr[k];
            ty += dc[k];
            if (getState(tx, ty) == INVALID) break;
            m_cntNeighbour[tx][ty]--;
        }

        tx = x;
        ty = y;
        for (int step = 1; step <= 2; step++) {
            tx -= dr[k];
            ty -= dc[k];
            if (getState(tx, ty) == INVALID) break;
            m_cntNeighbour[tx][ty]--;
        }
    }
}

unsigned long long Board::getBoardHash() const { return m_pZobristHash->getBoardHash(); }