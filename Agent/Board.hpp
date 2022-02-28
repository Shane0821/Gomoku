#ifndef BOARD_H
#define BOARD_H

#include <bits/stdc++.h>

#include "Const.h"

using namespace std;

struct Board {
    // 每个位置落子类型：-1 无子，0 白，1 黑
    int boardState[SIZE][SIZE];

    // 构造函数
    Board();
    // 输出棋盘
    void Show();
    // 落子
    void PlaceAt(int, int, int);
    // 取消落子
    void UnPlaceAt(int, int);
    // 全局检查五子
    bool CheckFive(int);
    // 上次落子位置周围检查五子
    bool CheckFive(int, int, bool);
    // 和当前点相对位置的格子状态
    int RelativePosState(int, int, int, int);
    // 判断当前点的得分情况, 详见函数处注释
    LL MarkOfPoint(int, int, int);
};

Board::Board() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            boardState[i][j] = UNPLACE;
        }
}

void Board::Show() {
    cout << "  ";
    for (int i = 0; i < SIZE; i++)
        cout << char(i < 10 ? (int)i + '0' : 'A' + (int)i - 10) << " ";
    cout << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << char(i < 10 ? (int)i + '0' : 'A' + (int)i - 10) << "|";
        for (int j = 0; j < SIZE; j++) {
            if (boardState[i][j] == -1)
                cout << "* ";
            else
                cout << (int)boardState[i][j] << " ";
        }
        cout << endl;
    }
}

void Board::PlaceAt(int x, int y, int color) {
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE)
        boardState[(int)x][(int)y] = color;

#ifndef ONLINE_JUDGE
    cout << "Chess Placed at: "
         << "(" << (int)x << ", " << (int)y << ")" << endl;
#endif
}

void Board::UnPlaceAt(int x, int y) { boardState[x][y] = -1; }

bool Board::CheckFive(int color) {
    // 遍历每个位置
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            if (boardState[i][j] != color) continue;
            // 遍历 4 个方向，另外 4 个等价
            for (int k = 0; k < 4; k++) {
                int ti = i, tj = j;
                // 遍历方向上每个棋子
                for (int s = 1; s <= 4; s++) {
                    ti += dr[k];
                    tj += dc[k];
                    if (ti < 0 || ti >= SIZE || tj < 0 || tj >= SIZE) continue;
                    if (boardState[ti][tj] != color) break;
                    if (s == 4) return true;
                }
            }
        }
    return false;
}

bool Board::CheckFive(int i, int j, bool color) {
    // 遍历 4 个方向，另外 4 个等价
    for (int k = 0; k < 4; k++) {
        // 遍历每个棋子
        int cnt = 0;
        for (int s = 1; s <= 4; s++) {
            int ti = i + s * dr[k];
            int tj = j + s * dc[k];
            if (ti < 0 || ti >= SIZE || tj < 0 || tj >= SIZE) break;
            if (boardState[ti][tj] != color) break;
            cnt++;
        }
        if (cnt >= 4) return true;
        for (int s = 1; s <= 4; s++) {
            int ti = i - s * dr[k];
            int tj = j - s * dc[k];
            if (ti < 0 || ti >= SIZE || tj < 0 || tj >= SIZE) break;
            if (boardState[ti][tj] != color) break;
            cnt++;
        }
        if (cnt >= 4) return true;
    }
    return false;
}

int Board::RelativePosState(int curX, int curY, int direction, int offset) {
    curX = curX + dr[direction] * offset;
    curY = curY + dc[direction] * offset;
    if (curX < 0 || curX >= SIZE || curY < 0 || curY >= SIZE) {
        return INVALID;
    }
    return boardState[curX][curY];
}

LL Board::MarkOfPoint(int curX, int curY, int playerColor) {
    LL total = 0;
    for (int i = 0; i < 4; i++) {
        int left = 0, right = 0;
        while (RelativePosState(curX, curY, i, -left - 1) == playerColor)
            left++;
        while (RelativePosState(curX, curY, i, right + 1) == playerColor)
            right++;
        if (left + right >= 4) {
            return MARKS[4][0];
        }

        int leftUnplace = RelativePosState(curX, curY, i, -left - 1) == UNPLACE,
            rightUnplace =
                RelativePosState(curX, curY, i, right + 1) == UNPLACE;
        int leftLeftEq =
                RelativePosState(curX, curY, i, -left - 2) == playerColor,
            rightRightEq =
                RelativePosState(curX, curY, i, right + 2) == playerColor;

        if (leftUnplace || rightUnplace) {
            if (left + right == 3)
                total += MARKS[left + right][leftUnplace ^ rightUnplace];
            else {
                bool tagLeft = (leftUnplace & leftLeftEq),
                     tagRight = (rightUnplace & rightRightEq);
                if (tagLeft && tagRight)
                    total +=
                        MARKS[left + right][leftUnplace ^ rightUnplace] * 50;
                else if (tagLeft || tagRight)
                    total +=
                        MARKS[left + right][leftUnplace ^ rightUnplace] * 5;
                else
                    total += MARKS[left + right][leftUnplace ^ rightUnplace];
            }
        }
    }
    return total + BASE_MARK[curX][curY];
}

#endif