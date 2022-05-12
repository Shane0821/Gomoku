#ifndef POSITION_H
#include "Const.h"

struct Position {
    int x, y;
    LL w;
};

bool operator<(const Position &lhs, const Position &rhs) {
    return (lhs.w == rhs.w)
               ? ((lhs.x == rhs.x) ? (lhs.y < rhs.y) : (lhs.x < rhs.x))
               : (lhs.w > rhs.w);
}
#endif