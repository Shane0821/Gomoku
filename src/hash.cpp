#include "hash.h"

#include <ctime>
#include <random>

Zobrist::Zobrist() {
    for (int i = 0; i < 2; i++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            for (int y = 0; y < Board::BOARD_SIZE; y++) {
                m_hashTable[i][x][y] = generateRandomNumber();
            }
        }
    }
    m_boardHash = generateRandomNumber();
}

unsigned long long Zobrist::generateRandomNumber() {
    static std::mt19937 mt(time(NULL));
    static std::uniform_int_distribution<unsigned long long> dist(0, UINT64_MAX);
    return dist(mt);
}

void Zobrist::update(int x, int y, Board::PIECE_COLOR color) {
    m_boardHash ^= m_hashTable[color][x][y];
}
