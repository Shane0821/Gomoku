#include "tt.h"

TT::TT() {
    m_pTable[0] = new Item[LENGTH];
    m_pTable[1] = new Item[LENGTH];
}

TT::~TT() {
    delete[] m_pTable[0];
    delete[] m_pTable[1];
}

int TT::find(unsigned long long hash, int depth, int alpha, int beta,
             Board::PIECE_COLOR color) const {
    int idx = getHashIndex(hash);

    Item &item = m_pTable[color][idx];

    if (item.flag == EMPTY) return TT_NOT_HIT;

    if (item.hash == hash && item.depth >= depth) {
        switch (item.flag) {
            case EXACT:
                return item.value;
            case LOWER:
                // val >= beta, the opponent will not choose this node
                if (item.value >= beta) return item.value;
                break;
            case UPPER:
                // val <= alpha, we will not choose this node
                if (item.value <= alpha) return item.value;
                break;
            default:
                break;
        }
    }

    return TT_NOT_HIT;
}

void TT::insert(unsigned long long hash, int depth, int value, Flag flag,
                Board::PIECE_COLOR color) {
    int idx = getHashIndex(hash);

    Item &item = m_pTable[color][idx];

    // always replace the entry
    item.depth = depth;
    item.hash = hash;
    item.value = value;
    item.flag = flag;
}