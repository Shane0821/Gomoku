// for online submission only

#pragma GCC optimize("O3")

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "jsoncpp/json.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

class Board {
   public:
    enum PIECE_COLOR {
        WHITE = 0,
        BLACK = 1,
    };

    enum BOARD_STATE {
        INVALID = -1,
        WHITE_PIECE = PIECE_COLOR::WHITE,
        BLACK_PIECE = PIECE_COLOR::BLACK,
        UNPLACE = 2
    };

    Board();

    /** Update state at (x, y) */
    void placeAt(int x, int y, PIECE_COLOR color);

    void unplaceAt(int x, int y);

    /** Get the state of (x, y) */
    BOARD_STATE getState(int x, int y) const;

    int cntNeighbour(int x, int y) const { return m_cntNeighbour[x][y]; }

    const static int BOARD_SIZE = 15;
    const static int dr[4];
    const static int dc[4];

   private:
    BOARD_STATE m_boardState[BOARD_SIZE][BOARD_SIZE];
    int m_cntNeighbour[BOARD_SIZE][BOARD_SIZE];
};

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
}

Board::BOARD_STATE Board::getState(int x, int y) const {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return BOARD_STATE::INVALID;
    }
    return m_boardState[x][y];
}

void Board::placeAt(int x, int y, Board::PIECE_COLOR color) {
    m_boardState[x][y] = static_cast<BOARD_STATE>(color);

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

class Scorer {
   public:
    enum Type {
        FIVE = 0,
        LIVE_FOUR = 1,
        KILL = 2,
        SLEEP_FOUR = 3,
        LIVE_THREE = 4,
        SLEEP_THREE = 5,
        LIVE_TWO = 6,
        SLEEP_TWO = 7,
        BASE = 8
    };

    Scorer();
    ~Scorer() { delete[] m_pTypeTable; }

    int getScoreByLineState(int state) const { return TYPE_SCORES[m_pTypeTable[state]]; }

    const static int CNT_STATES = 19683;
    const static int CNT_TYPES = 9;
    const static int TYPE_SCORES[CNT_TYPES];
    const static int BASE_SCORES[Board::BOARD_SIZE][Board::BOARD_SIZE];

   private:
    Type *m_pTypeTable;
};

const int Scorer::TYPE_SCORES[CNT_TYPES] = {999999, 100000, 10000, 1000, 1000,
                                            100,    100,    10,    0};

const int Scorer::BASE_SCORES[Board::BOARD_SIZE][Board::BOARD_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

Scorer::Scorer() {
    m_pTypeTable = new Type[CNT_STATES];
    for (int i = 0; i < CNT_STATES; i++) m_pTypeTable[i] = BASE;
    m_pTypeTable[121] = FIVE;
    m_pTypeTable[122] = SLEEP_FOUR;
    m_pTypeTable[124] = SLEEP_FOUR;
    m_pTypeTable[125] = SLEEP_THREE;
    m_pTypeTable[130] = SLEEP_FOUR;
    m_pTypeTable[131] = SLEEP_THREE;
    m_pTypeTable[148] = SLEEP_FOUR;
    m_pTypeTable[149] = SLEEP_THREE;
    m_pTypeTable[151] = SLEEP_THREE;
    m_pTypeTable[152] = SLEEP_TWO;
    m_pTypeTable[202] = SLEEP_FOUR;
    m_pTypeTable[203] = SLEEP_THREE;
    m_pTypeTable[205] = SLEEP_THREE;
    m_pTypeTable[206] = SLEEP_TWO;
    m_pTypeTable[211] = SLEEP_THREE;
    m_pTypeTable[212] = SLEEP_TWO;
    m_pTypeTable[229] = SLEEP_THREE;
    m_pTypeTable[230] = SLEEP_TWO;
    m_pTypeTable[232] = SLEEP_TWO;
    m_pTypeTable[233] = BASE;
    m_pTypeTable[365] = FIVE;
    m_pTypeTable[367] = SLEEP_FOUR;
    m_pTypeTable[368] = SLEEP_FOUR;
    m_pTypeTable[373] = SLEEP_FOUR;
    m_pTypeTable[374] = SLEEP_FOUR;
    m_pTypeTable[391] = SLEEP_FOUR;
    m_pTypeTable[392] = SLEEP_FOUR;
    m_pTypeTable[394] = SLEEP_THREE;
    m_pTypeTable[395] = SLEEP_THREE;
    m_pTypeTable[445] = SLEEP_FOUR;
    m_pTypeTable[446] = SLEEP_FOUR;
    m_pTypeTable[448] = SLEEP_THREE;
    m_pTypeTable[449] = SLEEP_THREE;
    m_pTypeTable[454] = SLEEP_THREE;
    m_pTypeTable[455] = SLEEP_THREE;
    m_pTypeTable[607] = FIVE;
    m_pTypeTable[608] = LIVE_FOUR;
    m_pTypeTable[610] = SLEEP_FOUR;
    m_pTypeTable[611] = LIVE_THREE;
    m_pTypeTable[616] = SLEEP_FOUR;
    m_pTypeTable[617] = LIVE_THREE;
    m_pTypeTable[634] = SLEEP_FOUR;
    m_pTypeTable[635] = LIVE_THREE;
    m_pTypeTable[637] = SLEEP_THREE;
    m_pTypeTable[638] = LIVE_TWO;
    m_pTypeTable[688] = SLEEP_FOUR;
    m_pTypeTable[689] = LIVE_THREE;
    m_pTypeTable[691] = SLEEP_THREE;
    m_pTypeTable[692] = LIVE_TWO;
    m_pTypeTable[697] = SLEEP_THREE;
    m_pTypeTable[698] = LIVE_TWO;
    m_pTypeTable[1096] = FIVE;
    m_pTypeTable[1097] = FIVE;
    m_pTypeTable[1102] = SLEEP_FOUR;
    m_pTypeTable[1103] = SLEEP_FOUR;
    m_pTypeTable[1120] = SLEEP_FOUR;
    m_pTypeTable[1121] = SLEEP_FOUR;
    m_pTypeTable[1123] = SLEEP_FOUR;
    m_pTypeTable[1124] = SLEEP_FOUR;
    m_pTypeTable[1174] = SLEEP_FOUR;
    m_pTypeTable[1175] = SLEEP_FOUR;
    m_pTypeTable[1177] = SLEEP_FOUR;
    m_pTypeTable[1178] = SLEEP_FOUR;
    m_pTypeTable[1183] = SLEEP_THREE;
    m_pTypeTable[1184] = SLEEP_THREE;
    m_pTypeTable[1336] = FIVE;
    m_pTypeTable[1337] = LIVE_FOUR;
    m_pTypeTable[1339] = LIVE_FOUR;
    m_pTypeTable[1340] = SLEEP_FOUR;
    m_pTypeTable[1345] = SLEEP_FOUR;
    m_pTypeTable[1346] = LIVE_THREE;
    m_pTypeTable[1363] = SLEEP_FOUR;
    m_pTypeTable[1364] = LIVE_THREE;
    m_pTypeTable[1366] = LIVE_THREE;
    m_pTypeTable[1367] = SLEEP_THREE;
    m_pTypeTable[1823] = FIVE;
    m_pTypeTable[1825] = LIVE_FOUR;
    m_pTypeTable[1826] = LIVE_FOUR;
    m_pTypeTable[1831] = SLEEP_FOUR;
    m_pTypeTable[1832] = SLEEP_FOUR;
    m_pTypeTable[1849] = SLEEP_FOUR;
    m_pTypeTable[1850] = SLEEP_FOUR;
    m_pTypeTable[1852] = LIVE_THREE;
    m_pTypeTable[1853] = LIVE_THREE;
    m_pTypeTable[1903] = SLEEP_FOUR;
    m_pTypeTable[1904] = SLEEP_FOUR;
    m_pTypeTable[1906] = LIVE_THREE;
    m_pTypeTable[1907] = LIVE_THREE;
    m_pTypeTable[1912] = SLEEP_THREE;
    m_pTypeTable[1913] = SLEEP_THREE;
    m_pTypeTable[2065] = FIVE;
    m_pTypeTable[2066] = LIVE_FOUR;
    m_pTypeTable[2068] = SLEEP_FOUR;
    m_pTypeTable[2069] = LIVE_THREE;
    m_pTypeTable[2074] = SLEEP_FOUR;
    m_pTypeTable[2075] = LIVE_THREE;
    m_pTypeTable[2092] = SLEEP_FOUR;
    m_pTypeTable[2093] = LIVE_THREE;
    m_pTypeTable[2095] = SLEEP_THREE;
    m_pTypeTable[2096] = LIVE_TWO;
    m_pTypeTable[3289] = FIVE;
    m_pTypeTable[3290] = FIVE;
    m_pTypeTable[3307] = SLEEP_FOUR;
    m_pTypeTable[3308] = SLEEP_FOUR;
    m_pTypeTable[3310] = SLEEP_FOUR;
    m_pTypeTable[3311] = SLEEP_FOUR;
    m_pTypeTable[3361] = SLEEP_FOUR;
    m_pTypeTable[3362] = SLEEP_FOUR;
    m_pTypeTable[3364] = SLEEP_FOUR;
    m_pTypeTable[3365] = SLEEP_FOUR;
    m_pTypeTable[3370] = SLEEP_FOUR;
    m_pTypeTable[3371] = SLEEP_FOUR;
    m_pTypeTable[3523] = FIVE;
    m_pTypeTable[3524] = LIVE_FOUR;
    m_pTypeTable[3526] = LIVE_FOUR;
    m_pTypeTable[3527] = SLEEP_FOUR;
    m_pTypeTable[3532] = LIVE_FOUR;
    m_pTypeTable[3533] = SLEEP_FOUR;
    m_pTypeTable[3550] = SLEEP_FOUR;
    m_pTypeTable[3551] = LIVE_THREE;
    m_pTypeTable[3553] = LIVE_THREE;
    m_pTypeTable[3554] = SLEEP_THREE;
    m_pTypeTable[4010] = FIVE;
    m_pTypeTable[4012] = LIVE_FOUR;
    m_pTypeTable[4013] = LIVE_FOUR;
    m_pTypeTable[4018] = LIVE_FOUR;
    m_pTypeTable[4019] = LIVE_FOUR;
    m_pTypeTable[4036] = SLEEP_FOUR;
    m_pTypeTable[4037] = SLEEP_FOUR;
    m_pTypeTable[4039] = LIVE_THREE;
    m_pTypeTable[4040] = LIVE_THREE;
    m_pTypeTable[4090] = SLEEP_FOUR;
    m_pTypeTable[4091] = SLEEP_FOUR;
    m_pTypeTable[4093] = LIVE_THREE;
    m_pTypeTable[4094] = LIVE_THREE;
    m_pTypeTable[4099] = LIVE_THREE;
    m_pTypeTable[4100] = LIVE_THREE;
    m_pTypeTable[5470] = FIVE;
    m_pTypeTable[5471] = FIVE;
    m_pTypeTable[5476] = LIVE_FOUR;
    m_pTypeTable[5477] = LIVE_FOUR;
    m_pTypeTable[5494] = SLEEP_FOUR;
    m_pTypeTable[5495] = SLEEP_FOUR;
    m_pTypeTable[5497] = SLEEP_FOUR;
    m_pTypeTable[5498] = SLEEP_FOUR;
    m_pTypeTable[5548] = SLEEP_FOUR;
    m_pTypeTable[5549] = SLEEP_FOUR;
    m_pTypeTable[5551] = SLEEP_FOUR;
    m_pTypeTable[5552] = SLEEP_FOUR;
    m_pTypeTable[5557] = LIVE_THREE;
    m_pTypeTable[5558] = LIVE_THREE;
    m_pTypeTable[5710] = FIVE;
    m_pTypeTable[5711] = LIVE_FOUR;
    m_pTypeTable[5713] = LIVE_FOUR;
    m_pTypeTable[5714] = SLEEP_FOUR;
    m_pTypeTable[5719] = SLEEP_FOUR;
    m_pTypeTable[5720] = LIVE_THREE;
    m_pTypeTable[5737] = SLEEP_FOUR;
    m_pTypeTable[5738] = LIVE_THREE;
    m_pTypeTable[5740] = LIVE_THREE;
    m_pTypeTable[5741] = SLEEP_THREE;
    m_pTypeTable[6197] = FIVE;
    m_pTypeTable[6199] = LIVE_FOUR;
    m_pTypeTable[6200] = LIVE_FOUR;
    m_pTypeTable[6205] = SLEEP_FOUR;
    m_pTypeTable[6206] = SLEEP_FOUR;
    m_pTypeTable[6227] = LIVE_THREE;
    m_pTypeTable[6277] = SLEEP_FOUR;
    m_pTypeTable[6278] = SLEEP_FOUR;
    m_pTypeTable[6280] = LIVE_THREE;
    m_pTypeTable[6281] = LIVE_THREE;
    m_pTypeTable[6286] = SLEEP_THREE;
    m_pTypeTable[6287] = SLEEP_THREE;
    m_pTypeTable[9868] = FIVE;
    m_pTypeTable[9869] = FIVE;
    m_pTypeTable[9871] = FIVE;
    m_pTypeTable[9872] = FIVE;
    m_pTypeTable[10084] = FIVE;
    m_pTypeTable[10085] = LIVE_FOUR;
    m_pTypeTable[10087] = LIVE_FOUR;
    m_pTypeTable[10088] = SLEEP_FOUR;
    m_pTypeTable[10093] = LIVE_FOUR;
    m_pTypeTable[10094] = SLEEP_FOUR;
    m_pTypeTable[10111] = SLEEP_FOUR;
    m_pTypeTable[10112] = SLEEP_FOUR;
    m_pTypeTable[10114] = SLEEP_FOUR;
    m_pTypeTable[10115] = SLEEP_FOUR;
    m_pTypeTable[10571] = FIVE;
    m_pTypeTable[10573] = LIVE_FOUR;
    m_pTypeTable[10574] = LIVE_FOUR;
    m_pTypeTable[10579] = LIVE_FOUR;
    m_pTypeTable[10580] = LIVE_FOUR;
    m_pTypeTable[10597] = LIVE_FOUR;
    m_pTypeTable[10598] = LIVE_FOUR;
    m_pTypeTable[10600] = SLEEP_FOUR;
    m_pTypeTable[10601] = SLEEP_FOUR;
    m_pTypeTable[12031] = FIVE;
    m_pTypeTable[12032] = FIVE;
    m_pTypeTable[12037] = LIVE_FOUR;
    m_pTypeTable[12038] = LIVE_FOUR;
    m_pTypeTable[12055] = LIVE_FOUR;
    m_pTypeTable[12056] = LIVE_FOUR;
    m_pTypeTable[12058] = LIVE_FOUR;
    m_pTypeTable[12059] = LIVE_FOUR;
    m_pTypeTable[12271] = FIVE;
    m_pTypeTable[12272] = LIVE_FOUR;
    m_pTypeTable[12274] = LIVE_FOUR;
    m_pTypeTable[12275] = SLEEP_FOUR;
    m_pTypeTable[12280] = SLEEP_FOUR;
    m_pTypeTable[12281] = LIVE_THREE;
    m_pTypeTable[12298] = SLEEP_FOUR;
    m_pTypeTable[12299] = LIVE_THREE;
    m_pTypeTable[12301] = LIVE_THREE;
    m_pTypeTable[12302] = LIVE_THREE;
    m_pTypeTable[16411] = FIVE;
    m_pTypeTable[16412] = FIVE;
    m_pTypeTable[16429] = LIVE_FOUR;
    m_pTypeTable[16430] = LIVE_FOUR;
    m_pTypeTable[16432] = LIVE_FOUR;
    m_pTypeTable[16433] = LIVE_FOUR;
    m_pTypeTable[16645] = FIVE;
    m_pTypeTable[16646] = LIVE_FOUR;
    m_pTypeTable[16648] = LIVE_FOUR;
    m_pTypeTable[16649] = SLEEP_FOUR;
    m_pTypeTable[16654] = LIVE_FOUR;
    m_pTypeTable[16655] = SLEEP_FOUR;
    m_pTypeTable[16672] = SLEEP_FOUR;
    m_pTypeTable[16673] = LIVE_THREE;
    m_pTypeTable[16675] = LIVE_THREE;
    m_pTypeTable[16676] = LIVE_THREE;
    m_pTypeTable[17132] = FIVE;
    m_pTypeTable[17134] = LIVE_FOUR;
    m_pTypeTable[17135] = LIVE_FOUR;
    m_pTypeTable[17140] = LIVE_FOUR;
    m_pTypeTable[17141] = LIVE_FOUR;
    m_pTypeTable[17158] = SLEEP_FOUR;
    m_pTypeTable[17159] = SLEEP_FOUR;
    m_pTypeTable[17161] = LIVE_THREE;
    m_pTypeTable[17162] = LIVE_THREE;
    m_pTypeTable[18592] = FIVE;
    m_pTypeTable[18593] = FIVE;
    m_pTypeTable[18598] = LIVE_FOUR;
    m_pTypeTable[18599] = LIVE_FOUR;
    m_pTypeTable[18616] = SLEEP_FOUR;
    m_pTypeTable[18617] = SLEEP_FOUR;
    m_pTypeTable[18619] = SLEEP_FOUR;
    m_pTypeTable[18620] = SLEEP_FOUR;
    m_pTypeTable[18832] = FIVE;
    m_pTypeTable[18833] = LIVE_FOUR;
    m_pTypeTable[18835] = LIVE_FOUR;
    m_pTypeTable[18836] = SLEEP_FOUR;
    m_pTypeTable[18841] = SLEEP_FOUR;
    m_pTypeTable[18842] = LIVE_THREE;
    m_pTypeTable[18859] = SLEEP_FOUR;
    m_pTypeTable[18860] = LIVE_THREE;
    m_pTypeTable[18862] = LIVE_THREE;
    m_pTypeTable[18863] = SLEEP_THREE;
}

class MoveGenerator {
   public:
    struct Move {
        int x = -1;
        int y = -1;

        bool operator==(const Move &other) const { return x == other.x && y == other.y; }
    };

    MoveGenerator();

    void sortMoves();
    void updateMoveScoreByDir(const Move &move, int dir, int w, Board::PIECE_COLOR);
    void addMove(const Move &move);
    void eraseMove(const Move &move);
    bool existsMove(const Move &move);
    std::vector<Move> generateMovesList(int cnt);
    int playerMoveScore(const Move &move, Board::PIECE_COLOR color) const;
    int maxMoveScore(const Move &move) const;
    int sumPlayerScore(Board::PIECE_COLOR color) const;

    const static int INVALID_MOVE_WEIGHT = -__INT32_MAX__;

   public:
    std::vector<Move> m_moves;
    int m_recorded[Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_dirScore[2][4][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_playerMoveScore[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_maxScore[Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_cntKill[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    int m_sumPlayerScore[2] = {0, 0};
};

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
                                         Board::PIECE_COLOR player) {
    m_sumPlayerScore[player] -= m_playerMoveScore[player][move.x][move.y];

    m_playerMoveScore[player][move.x][move.y] -= m_dirScore[player][dir][move.x][move.y];
    if (m_dirScore[player][dir][move.x][move.y] ==
            Scorer::TYPE_SCORES[Scorer::SLEEP_FOUR] ||
        m_dirScore[player][dir][move.x][move.y] ==
            Scorer::TYPE_SCORES[Scorer::LIVE_THREE]) {
        m_cntKill[player][move.x][move.y]--;
        if (m_cntKill[player][move.x][move.y] == 1) {
            m_playerMoveScore[player][move.x][move.y] -=
                Scorer::TYPE_SCORES[Scorer::KILL];
        }
    }

    m_dirScore[player][dir][move.x][move.y] = w;
    m_playerMoveScore[player][move.x][move.y] += w;
    if (w == Scorer::TYPE_SCORES[Scorer::SLEEP_FOUR] ||
        w == Scorer::TYPE_SCORES[Scorer::LIVE_THREE]) {
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
        m_dirScore[Board::PIECE_COLOR::BLACK][i][move.x][move.y] =
            m_dirScore[Board::PIECE_COLOR::WHITE][i][move.x][move.y] = 0;
    }

    m_playerMoveScore[Board::PIECE_COLOR::BLACK][move.x][move.y] =
        m_playerMoveScore[Board::PIECE_COLOR::WHITE][move.x][move.y] = baseScore;

    m_maxScore[move.x][move.y] = baseScore;

    m_cntKill[Board::PIECE_COLOR::BLACK][move.x][move.y] =
        m_cntKill[Board::PIECE_COLOR::WHITE][move.x][move.y] = 0;

    m_sumPlayerScore[Board::PIECE_COLOR::BLACK] += baseScore;
    m_sumPlayerScore[Board::PIECE_COLOR::WHITE] += baseScore;
}

void MoveGenerator::eraseMove(const Move &move) {
    m_sumPlayerScore[Board::PIECE_COLOR::BLACK] -=
        m_playerMoveScore[Board::PIECE_COLOR::BLACK][move.x][move.y];
    m_sumPlayerScore[Board::PIECE_COLOR::WHITE] -=
        m_playerMoveScore[Board::PIECE_COLOR::WHITE][move.x][move.y];

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

int MoveGenerator::playerMoveScore(const Move &move, Board::PIECE_COLOR color) const {
    return m_playerMoveScore[color][move.x][move.y];
}

int MoveGenerator::maxMoveScore(const Move &move) const {
    return m_maxScore[move.x][move.y];
}

int MoveGenerator::sumPlayerScore(Board::PIECE_COLOR color) const {
    return m_sumPlayerScore[color];
}

class Zobrist {
   public:
    Zobrist();

    static unsigned long long generateRandomNumber();

    unsigned long long update(int x, int y, Board::PIECE_COLOR);

    unsigned long long getBoardHash() { return m_boardHash; }

   private:
    unsigned long long m_hashTable[2][Board::BOARD_SIZE][Board::BOARD_SIZE];
    unsigned long long m_boardHash;
};

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

unsigned long long Zobrist::update(int x, int y, Board::PIECE_COLOR color) {
    m_boardHash ^= m_hashTable[color][x][y];
    return m_boardHash;
}

class TT {
   public:
    enum Flag { EMPTY, EXACT, LOWER, UPPER };

    struct Item {
        int depth;
        unsigned long long hash;
        int value;
        Flag flag = EMPTY;
    };

    TT();
    ~TT();

    int find(unsigned long long hash, int depth, int alpha, int beta,
             Board::PIECE_COLOR) const;
    void insert(unsigned long long hash, int depth, int value, Flag, Board::PIECE_COLOR);

    const static int LENGTH = 1 << 20;
    const static int TT_NOT_HIT = __INT32_MAX__ - 1;

   private:
    int getHashIndex(unsigned long long hash) const { return hash & 0xFFFFF; }

    Item *m_pTable[2] = {nullptr, nullptr};
};

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
                if (item.value >= beta) return item.value;
                break;
            case UPPER:
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

    item.depth = depth;
    item.hash = hash;
    item.value = value;
    item.flag = flag;
}

class Timer {
   public:
    Timer() { m_preTime = clock(); }

    void recordCurrent() { m_preTime = clock(); }

    int getTimePass() const {
        return 1.0 * (clock() - m_preTime) / CLOCKS_PER_SEC * 1000;
    }

    const static int TIME_OUT = __INT32_MAX__;

   private:
    time_t m_preTime;
};

class Core {
   public:
    Core(Board *pBoard);
    ~Core() {}

    void setColor(Board::PIECE_COLOR color) { m_color = color; }
    MoveGenerator::Move bestMove() const { return m_bestMove; }
    int bestScore() const { return m_bestScore; }
    int run();
    void makeMove(int x, int y, Board::PIECE_COLOR);
    void cancelMove(int x, int y);

    static int MIN_SEARCH_DEPTH;
    static int MAX_SEARCH_DEPTH;
    static int KILL_DEPTH;
    static bool ITERATIVE_DEEPENING;
    static int BRANCH_FACTOR;
    const static int INF = __INT32_MAX__ - 100;
    const static int TIME_LIMIT = 980;

   private:
    int negMiniMaxSearch(int depth, Board::PIECE_COLOR player, int alpha, int beta);
    void updateMoveAt(int x, int y, Board::PIECE_COLOR);
    void updateMoveAt(int x, int y, int dir, Board::PIECE_COLOR);
    void updateMoveAround(int x, int y, Board::PIECE_COLOR);

    int evaluate() const;

    Board *m_pBoard = nullptr;

    Timer m_timer;
    TT m_TT;
    Zobrist m_zobristHash;
    MoveGenerator m_moveGenerator;
    Scorer m_scorer;

    Board::PIECE_COLOR m_color = Board::PIECE_COLOR::WHITE;

    MoveGenerator::Move m_bestMove;
    int m_bestScore = -__INT32_MAX__;
    int iterativeDepth = 4;
};

bool Core::ITERATIVE_DEEPENING = true;
int Core::BRANCH_FACTOR = 16;
int Core::MIN_SEARCH_DEPTH = 4;
int Core::MAX_SEARCH_DEPTH = 10;
int Core::KILL_DEPTH = 4;

Core::Core(Board *pBoard) : m_pBoard(pBoard) {
    if (!pBoard) return;
    for (int i = 0; i < Board::BOARD_SIZE; i++) {
        for (int j = 0; j < Board::BOARD_SIZE; j++) {
            Board::BOARD_STATE state = pBoard->getState(i, j);
            if (state == Board::BOARD_STATE::WHITE_PIECE ||
                state == Board::BOARD_STATE::BLACK_PIECE) {
                m_zobristHash.update(i, j, static_cast<Board::PIECE_COLOR>(state));
            }
        }
    }

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

int Core::negMiniMaxSearch(int depth, Board::PIECE_COLOR player, int alpha, int beta) {
    if (depth == 0) {
        int val = evaluate();
        m_TT.insert(m_zobristHash.getBoardHash(), depth, val, TT::EXACT, player);
        return val;
    }

    if (m_timer.getTimePass() >= TIME_LIMIT) {
        return Timer::TIME_OUT;
    }

    if (depth != iterativeDepth) {
        int val = m_TT.find(m_zobristHash.getBoardHash(), depth, alpha, beta, player);
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
                m_TT.insert(m_zobristHash.getBoardHash(), depth, beta, TT::LOWER, player);
                return val;
            }
            if (val > alpha) {
                flag = TT::EXACT;
                alpha = val;
            }
            m_TT.insert(m_zobristHash.getBoardHash(), depth, alpha, flag, player);
            return alpha;
        } else {
            opponentHasFive = true;
        }
        i++;
    }

    int val = alpha;

    if (opponentHasFive) {
        // must block the opponent's FIVE
        i--;

        auto &move = moves[i];

        makeMove(move.x, move.y, player);
        val = -negMiniMaxSearch(depth - 1, opponent, -beta, -alpha);
        cancelMove(move.x, move.y);

        if (val == -Timer::TIME_OUT) return Timer::TIME_OUT;

        if (depth == iterativeDepth && val > m_bestScore) {
            m_bestMove = move;
            m_bestScore = val;
        }
        if (val >= beta) {
            m_TT.insert(m_zobristHash.getBoardHash(), depth, beta, TT::LOWER, player);
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

            if (depth == iterativeDepth && val > m_bestScore) {
                m_bestMove = move;
                m_bestScore = val;
            }
            if (val >= beta) {
                m_TT.insert(m_zobristHash.getBoardHash(), depth, beta, TT::LOWER, player);
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

    m_TT.insert(m_zobristHash.getBoardHash(), depth, alpha, flag, player);
    return alpha;
}

int Core::run() {
    if (!m_pBoard) return -1;

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
    if (m_pBoard->getState(x, y) != Board::BOARD_STATE::UNPLACE) {
        return;
    }
    m_zobristHash.update(x, y, player);
    m_pBoard->placeAt(x, y, player);
    m_moveGenerator.eraseMove({x, y});
    updateMoveAround(x, y, Board::PIECE_COLOR::BLACK);
    updateMoveAround(x, y, Board::PIECE_COLOR::WHITE);
}

void Core::cancelMove(int x, int y) {
    Board::BOARD_STATE preState = m_pBoard->getState(x, y);
    if (preState == Board::BOARD_STATE::UNPLACE ||
        preState == Board::BOARD_STATE::INVALID) {
        return;
    }
    m_zobristHash.update(x, y, static_cast<Board::PIECE_COLOR>(preState));
    m_pBoard->unplaceAt(x, y);
    m_moveGenerator.addMove({x, y});
    updateMoveAt(x, y, Board::PIECE_COLOR::BLACK);
    updateMoveAt(x, y, Board::PIECE_COLOR::WHITE);
    updateMoveAround(x, y, Board::PIECE_COLOR::BLACK);
    updateMoveAround(x, y, Board::PIECE_COLOR::WHITE);
}

void Core::updateMoveAt(int x, int y, int dir, Board::PIECE_COLOR player) {
    int lx = x, ly = y, l = 0, cnt2 = 0;
    while (l < 4) {
        Board::BOARD_STATE state =
            m_pBoard->getState(lx - Board::dr[dir], ly - Board::dc[dir]);
        if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
            break;
        }

        l++;
        lx -= Board::dr[dir];
        ly -= Board::dc[dir];

        if (state == Board::BOARD_STATE::UNPLACE) {
            cnt2++;
        } else {
            cnt2 = 0;
        }
        if (cnt2 == 2) {
            break;
        }
    }

    int lineState = 0;
    for (int step = 1; step <= l; step++) {
        lineState =
            lineState * 3 + (m_pBoard->getState(lx, ly) == Board::UNPLACE ? 2 : 1);
        lx += Board::dr[dir];
        ly += Board::dc[dir];
    }

    lineState = lineState * 3 + 1;

    int rx = x, ry = y, r = 0;
    cnt2 = 0;
    while (r < 4) {
        Board::BOARD_STATE state =
            m_pBoard->getState(rx + Board::dr[dir], ry + Board::dc[dir]);
        if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
            break;
        }

        r++;
        rx += Board::dr[dir];
        ry += Board::dc[dir];

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
                                         m_scorer.getScoreByLineState(lineState), player);
}

void Core::updateMoveAt(int x, int y, Board::PIECE_COLOR player) {
    for (int dir = 0; dir < 4; dir++) {
        int lx = x, ly = y, l = 0, cnt2 = 0;
        while (l < 4) {
            Board::BOARD_STATE state =
                m_pBoard->getState(lx - Board::dr[dir], ly - Board::dc[dir]);
            if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
                break;
            }

            l++;
            lx -= Board::dr[dir];
            ly -= Board::dc[dir];

            if (state == Board::BOARD_STATE::UNPLACE) {
                cnt2++;
            } else {
                cnt2 = 0;
            }
            if (cnt2 == 2) {
                break;
            }
        }

        int lineState = 0;
        for (int step = 1; step <= l; step++) {
            lineState =
                lineState * 3 + (m_pBoard->getState(lx, ly) == Board::UNPLACE ? 2 : 1);
            lx += Board::dr[dir];
            ly += Board::dc[dir];
        }

        lineState = lineState * 3 + 1;

        int rx = x, ry = y, r = 0;
        cnt2 = 0;
        while (r < 4) {
            Board::BOARD_STATE state =
                m_pBoard->getState(rx + Board::dr[dir], ry + Board::dc[dir]);
            if (state == Board::BOARD_STATE::INVALID || state == (player ^ 1)) {
                break;
            }

            r++;
            rx += Board::dr[dir];
            ry += Board::dc[dir];

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
            {x, y}, dir, m_scorer.getScoreByLineState(lineState), player);
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
    return m_moveGenerator.sumPlayerScore(Board::PIECE_COLOR::BLACK) * 6 -
           m_moveGenerator.sumPlayerScore(Board::PIECE_COLOR::WHITE);
}

class Judger {
   public:
    enum MODE { ONLINE_JUDGE = 0, COMMAND_LINE = 1 };

    Judger() {}
    ~Judger();

    static void setMode(MODE mode) { JUDGER_MODE = mode; }

    void printCoreMoveByJSON();
    void initGameByJSON();
    void startGame();
    bool checkFiveAt(int x, int y, Board::PIECE_COLOR);

    static MODE JUDGER_MODE;

   private:
    Core *m_pCore = nullptr;
    Board *m_pBoard = nullptr;
};

Judger::MODE Judger::JUDGER_MODE = Judger::MODE::COMMAND_LINE;

Judger::~Judger() {
    if (m_pBoard != nullptr) delete m_pBoard;
    if (m_pCore != nullptr) delete m_pCore;
}

void Judger::initGameByJSON() {
    std::string str;
    getline(std::cin, str);
    Json::Reader reader;
    Json::Value input;
    reader.parse(str, input);

    Board::PIECE_COLOR coreColor = Board::PIECE_COLOR::WHITE;
    if (input["requests"][0u]["x"].asInt() == -1 &&
        input["requests"][0u]["y"].asInt() == -1) {
        coreColor = Board::PIECE_COLOR::BLACK;
    }
    Board::PIECE_COLOR opponentColor = static_cast<Board::PIECE_COLOR>(coreColor ^ 1);

    int turnID = input["responses"].size();

    if (coreColor == Board::PIECE_COLOR::WHITE) {
        for (int i = 0; i < turnID; i++) {
            m_pBoard->placeAt(input["requests"][i]["x"].asInt(),
                              input["requests"][i]["y"].asInt(), opponentColor);
            m_pBoard->placeAt(input["responses"][i]["x"].asInt(),
                              input["responses"][i]["y"].asInt(), coreColor);
        }
        m_pBoard->placeAt(input["requests"][turnID]["x"].asInt(),
                          input["requests"][turnID]["y"].asInt(), opponentColor);
    } else {
        m_pBoard->placeAt(input["responses"][0u]["x"].asInt(),
                          input["responses"][0u]["y"].asInt(), coreColor);
        for (int i = 1; i < turnID; i++) {
            m_pBoard->placeAt(input["requests"][i]["x"].asInt(),
                              input["requests"][i]["y"].asInt(), opponentColor);
            m_pBoard->placeAt(input["responses"][i]["x"].asInt(),
                              input["responses"][i]["y"].asInt(), coreColor);
        }
        m_pBoard->placeAt(input["requests"][turnID]["x"].asInt(),
                          input["requests"][turnID]["y"].asInt(), opponentColor);
    }

    m_pCore = new Core(m_pBoard);
    m_pCore->setColor(coreColor);
}

void Judger::printCoreMoveByJSON() {
    Json::Value ret;
    MoveGenerator::Move best = m_pCore->bestMove();
    ret["response"]["x"] = best.x;
    ret["response"]["y"] = best.y;
    Json::FastWriter writer;
    std::cout << writer.write(ret) << std::endl;
}

void Judger::startGame() {
    if (m_pBoard != nullptr) delete m_pBoard;
    m_pBoard = new Board();
    if (m_pCore != nullptr) delete m_pCore;

    if (Judger::JUDGER_MODE == MODE::ONLINE_JUDGE) {
        initGameByJSON();
        m_pCore->run();
        printCoreMoveByJSON();
        return;
    }
}

int main() {
    Judger judger;
    Judger::JUDGER_MODE = Judger::MODE::ONLINE_JUDGE;
    judger.startGame();
    return 0;
}