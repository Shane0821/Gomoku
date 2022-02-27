#define ONLINE_JUDGE
#define ITERATIVE_DEEPENING
#define RELEASE
#ifdef RELEASE
#pragma GCC optimize(3, \
                     "Ofast,no-stack-protector,unroll-loops,fast-math,inline")
#endif
#include <bits/stdc++.h>
using namespace std;
// #include <Const.h>
#ifndef CONST
#define CONST
#include <bits/stdc++.h>
// 放置一些公用常量，不要格式化

#define debug(x) cerr << #x << " = " << x << endl

//#define ONLINE_JUDGE

typedef long long LL;

const LL INF = 1E16;

const LL TIME_OUT = INF + 1;
// bestDropId 表示未设置
const pair<int, int> POS_UNDEFINED = {-1, -1};

// 最大分支数
const int BRANCH_LIMIT = 20;

// 搜索深度默认为 6, 优化后再升级
const int SEARCH_DEPTH = 10;

// 各局面价值表，待完善
const int valueTable = {0};
// 棋盘大小
const int SIZE = 15;
// 方向增量
const int dr[] = {0, 1, 1, 1, 0, -1, -1, -1};
const int dc[] = {1, 1, 0, -1, -1, 1, 0, -1};

// 每个位置落子类型：
// -1 无子，0 白，1 黑，2 下标非法
const int UNPLACE = -1;
const int WHITE = 0;
const int BLACK = 1;
const int MAX = 2;
const int INVALID = -2;

// 棋子类型得分，参数可以调整
// 拆分远近活三，远中近活二
const LL LIVEFOURMARK = 2000000;
const LL SLEEPFOURMARK = 1000000;
const LL NEARLIVETHREEMARK = 102000;
const LL FARLIVETHREEMARK = 100000;
const LL SLEEPTHREEMARK = 50000;
const LL NEARLIVETWOMARK = 1200;
const LL MIDLIVETWOMARK = 1050;
const LL FARLIVETWOMARK = 1000;
const LL SLEEPTWOMARK = 500;
const LL ONEMARK = 1;

int SEARCHCNT[] = {0, 6, 6, 6, 6, 6, 6, 7, 7, 8, 225};
const LL MARKS[][2] = {{10, 1},
                       {1000, 100},
                       {100000, 20000},
                       {10000000, 200000},
                       {1000000000, 1000000000}};

const int BASE_MARK[15][15] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
#endif
#ifndef BOARD_H
#define BOARD_H

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
        int leftUnplace = RelativePosState(curX, curY, i, -left - 1) == UNPLACE,
            rightUnplace =
                RelativePosState(curX, curY, i, right + 1) == UNPLACE;
        if (left + right >= 4) {
            return MARKS[4][0];
        }
        if (leftUnplace || rightUnplace) {
            total += MARKS[left + right][leftUnplace ^ rightUnplace];
        }
    }
    return total + BASE_MARK[curX][curY];
}

#endif
// #include <Position.hpp>
#ifndef POSITION_H

struct Position {
    int x, y;
    LL w;
};

bool operator<(const Position &lhs, const Position &rhs) {
    return (lhs.w == rhs.w)
               ? ((lhs.x == rhs.x) ? (lhs.y < rhs.y) : (lhs.x < rhs.x))
               : (lhs.w > rhs.w);
}

#ifndef TIMER_H
#define TIMER_H
#include <bits/stdc++.h>

using namespace std;

struct Timer {
    time_t preTime;
    time_t preline;
    int16_t cnt;

    Timer() {
        preTime = 0;
        preline = 0;
        cnt = 0;
    }

    // how to use: timer->prepare(__LINE__);
    void prepare(int line) {
        cnt = 1;
        preline = line;
        preTime = clock();
    }
    // how to use: timer->getTimePass(__LINE__);
    void getTimePass(int line) {
        if (cnt != 1) {
            cerr << "line " << line << ": ";
            cerr << "Please call prepare() first!" << endl;
            return;
        }
        cout << "Time taken from line " + to_string(preline) + " to line " +
                    to_string(line) + " is: ";
        auto tmp = clock() - preTime;
        cout << setprecision(3) << fixed << (1.0 * tmp / CLOCKS_PER_SEC)
             << endl;
        cnt = 0;
    }
};

#endif

#endif
// #include <Agent.hpp>
#ifndef AGENT_HPP
#define AGENT_HPP

#include <bits/stdc++.h>

#include "jsoncpp/json.h"

using namespace std;

struct Agent {
    // 默认后手，持白子
    int color;
    // 当前迭代深度
    int iterDepth;
    //计时器
    Timer *myTimer;
    clock_t st;
    // 棋盘
    Board myBoard;
    // 最优落子
    pair<int, int> bestDropPos;
    // 最高得分
    LL bestScore;
    // 落子位置和对应的权重, 用于在set中查找.
    // -1表示有棋子的点
    LL weight[2][15][15];
    // 按照权排序的落子位置集合
    set<Position> nextPos[3];
    // 白子和黑子的权值之和
    LL sumWeight[2];

    // 构造函数
    Agent() {
        color = WHITE;
        myTimer = nullptr;
        iterDepth = SEARCH_DEPTH;
    }
    ~Agent() { delete myTimer; }
    // 运行
    void Run();
    // 判断AI是否为先手
    void DetermineColor(const Json::Value &);
    // 局面估值
    LL Evaluate(int);
    // 更改一个点的颜色, 同时更新附近的 9 * 4 格子内的空闲点权值信息
    void Update(int x, int y, int color);
    // 局面预处理
    void Init();
    // 搜素
    LL MinMaxSearch(int, LL, LL, int);
    // Jason形式输出落子
    void PrintJson();
};

LL Agent::MinMaxSearch(int depth, LL alpha, LL beta, int curColor) {
    // 临近超时
    if (1.0 * (clock() - st) / CLOCKS_PER_SEC >= 0.98) {
        // PrintJson();
        // exit(0);
        // if (curColor == color) return -INF - 1;
        // return INF + 1;
        return -TIME_OUT;
    }
    // 搜索完成估值返回
    if (depth <= 0) return Evaluate(curColor);
    // 无子可走
    if (!nextPos[MAX].size()) return alpha;
    auto pos = nextPos[MAX].begin();
    for (int i = 0; i < SEARCHCNT[depth] && pos != nextPos[MAX].end();
         i++, pos++) {
        auto x = pos->x, y = pos->y;
        auto w = pos->w;

        // 继续搜索
        LL val;
        if (weight[curColor][x][y] >= MARKS[4][0]) {
            val = INF - (SEARCH_DEPTH - depth);
        } else {
            // 落子
            Update(x, y, curColor);
            val = -MinMaxSearch(depth - 1, -beta, -alpha, !curColor);
            // 取消落子 更新得分
            Update(x, y, UNPLACE);
            pos = nextPos[MAX].find(Position{x, y, w});
#ifndef ONLINE_JUDGE
            assert(pos != nextPos[MAX].end());
#endif
        }

        if (val == TIME_OUT) return -TIME_OUT;

        if (depth == iterDepth &&
            (val > bestScore || bestDropPos == POS_UNDEFINED))
            bestScore = val, bestDropPos = {x, y};
        if (val >= beta) return val;
        if (val > alpha) alpha = val;
    }
    return alpha;
}

void Agent::Run() {
#ifndef ONLINE_JUDGE
    myBoard.Show();
    while (true) {
        Init();
        int x, y;
        cout << "Your drop position: ";
        cin >> x >> y;
        if (myBoard.RelativePosState(x, y, 0, 0) != UNPLACE) {
            cout << "This position is already occupied!" << endl;
            continue;
        }
        Update(x, y, BLACK);
        if (myBoard.CheckFive(BLACK)) {
            myBoard.Show();
            cout << "you win" << endl;
            break;
        }
        myTimer = new Timer;
        myTimer->prepare(__LINE__);

        st = clock();
#ifndef ITERATIVE_DEEPENING
        MinMaxSearch(SEARCH_DEPTH, -INF, INF, color);
#else
        for (int i = 2; i <= SEARCH_DEPTH; i += 2) {
            iterDepth = i;
            MinMaxSearch(i, -INF, INF, color);
        }
#endif

        myTimer->getTimePass(__LINE__);
        cout << "Opponent: " << bestDropPos.first << " " << bestDropPos.second
             << endl;
        // 落子
        Update(bestDropPos.first, bestDropPos.second, WHITE);
        // debug(nextPos[WHITE].size());
        // debug(nextPos[BLACK].size());
        debug(nextPos[MAX].size());
        debug(bestScore);
        myBoard.Show();
        if (myBoard.CheckFive(WHITE)) {
            cout << "you lose" << endl;
            break;
        }
    }
#else
    Init();
    st = clock();
#ifndef ITERATIVE_DEEPENING
    MinMaxSearch(SEARCH_DEPTH, -INF, INF, color);
#else
    for (int i = 6; i <= SEARCH_DEPTH; i += 2) {
        iterDepth = i;
        MinMaxSearch(i, -INF, INF, color);
    }
#endif
    // assert(bestScore != INF + 1);
    PrintJson();
#endif
}

void Agent::PrintJson() {
    Json::Value ret;
    ret["response"]["x"] = bestDropPos.first;
    ret["response"]["y"] = bestDropPos.second;
    Json::FastWriter writer;
    cout << writer.write(ret) << endl;
}

void Agent::DetermineColor(const Json::Value &input) {
    if (input["requests"][0u]["x"].asInt() == -1 &&
        input["requests"][0u]["y"].asInt() == -1) {
        color = BLACK;
    } else {
        color = WHITE;
    }
}

void Agent::Init() {
    bestDropPos = POS_UNDEFINED;
    bestScore = -INF;

#ifdef ONLINE_JUDGE
    // 读入JSON
    string str;
    getline(cin, str);
    Json::Reader reader;
    Json::Value input;
    reader.parse(str, input);
    // 分析自己收到的输入和自己过往的输出，并恢复状态
    int turnID = input["responses"].size();
    DetermineColor(input);
    for (int i = 0; i < turnID; i++) {
        myBoard.PlaceAt(input["requests"][i]["x"].asInt(),
                        input["requests"][i]["y"].asInt(), !color);
        myBoard.PlaceAt(input["responses"][i]["x"].asInt(),
                        input["responses"][i]["y"].asInt(), color);
    }
    myBoard.PlaceAt(input["requests"][turnID]["x"].asInt(),
                    input["requests"][turnID]["y"].asInt(), !color);
#endif

    // 初始化 weight 和 nextPoss
    sumWeight[0] = sumWeight[1] = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // 如果位置上已有棋子, 标记权重为-1
            // 否则计算权重, 并分别存放在weight和nextPos中
            if (myBoard.boardState[i][j] != UNPLACE) {
                weight[BLACK][i][j] = weight[WHITE][i][j] = -1;
            } else {
                weight[BLACK][i][j] = myBoard.MarkOfPoint(i, j, BLACK);
                weight[WHITE][i][j] = myBoard.MarkOfPoint(i, j, WHITE);
                sumWeight[BLACK] += weight[BLACK][i][j];
                sumWeight[WHITE] += weight[WHITE][i][j];
                nextPos[MAX].insert(Position{
                    i, j, max(weight[WHITE][i][j], weight[BLACK][i][j])});
                // nextPos[WHITE].insert(Position{i, j, weight[WHITE][i][j]});
                // nextPos[BLACK].insert(Position{i, j, weight[BLACK][i][j]});
            }
        }
    }
}

void Agent::Update(int x, int y, int color) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
        return;
    }
    // 检查是否均为或均不为未放置
    assert(!(weight[BLACK][x][y] == -1) ^ (weight[WHITE][x][y] == -1));
    // 原始为未放置=>清除点在weight和nextPos中的记录
    if (weight[BLACK][x][y] != -1 && weight[WHITE][x][y] != -1) {
#ifndef ONLINE_JUDGE
        assert(nextPos[MAX].count(Position{
                   x, y, max(weight[WHITE][x][y], weight[BLACK][x][y])}) == 1);
        // assert(nextPos[WHITE].count(Position{x, y, weight[WHITE][x][y]}) ==
        // 1); assert(nextPos[BLACK].count(Position{x, y, weight[BLACK][x][y]})
        // == 1);
#endif
        nextPos[MAX].erase(
            Position{x, y, max(weight[WHITE][x][y], weight[BLACK][x][y])});
        // nextPos[WHITE].erase(Position{x, y, weight[WHITE][x][y]});
        // nextPos[BLACK].erase(Position{x, y, weight[BLACK][x][y]});
        sumWeight[WHITE] -= weight[WHITE][x][y];
        sumWeight[BLACK] -= weight[BLACK][x][y];
        weight[BLACK][x][y] = weight[WHITE][x][y] = -1;
    }
    myBoard.boardState[x][y] = color;
    // 更改为未放置=>增加点在weight和nextPos中的记录
    if (color == UNPLACE) {
        weight[BLACK][x][y] = myBoard.MarkOfPoint(x, y, BLACK);
        weight[WHITE][x][y] = myBoard.MarkOfPoint(x, y, WHITE);
        sumWeight[WHITE] += weight[WHITE][x][y];
        sumWeight[BLACK] += weight[BLACK][x][y];
        nextPos[MAX].insert(
            Position{x, y, max(weight[WHITE][x][y], weight[BLACK][x][y])});
        // nextPos[WHITE].insert(Position{x, y, weight[WHITE][x][y]});
        // nextPos[BLACK].insert(Position{x, y, weight[BLACK][x][y]});
    }
    // 修改完成后, 在8*4范围内修改空闲点的权值
    for (int dir = 0; dir < 8; dir++) {
        for (int off = 1; off < 5; off++) {
            if (myBoard.RelativePosState(x, y, dir, off) == UNPLACE) {
                int i = x + dr[dir] * off, j = y + dc[dir] * off;
// 删除现存权值记录
#ifndef ONLINE_JUDGE
                assert(
                    nextPos[MAX].count(Position{
                        i, j, max(weight[WHITE][i][j], weight[BLACK][i][j])}) ==
                    1);
                // assert(nextPos[BLACK].count(
                //            Position{i, j, weight[BLACK][i][j]}) == 1);
                // assert(nextPos[WHITE].count(
                //            Position{i, j, weight[WHITE][i][j]}) == 1);
#endif
                nextPos[MAX].erase(Position{
                    i, j, max(weight[WHITE][i][j], weight[BLACK][i][j])});
                // nextPos[WHITE].erase(Position{i, j, weight[WHITE][i][j]});
                // nextPos[BLACK].erase(Position{i, j, weight[BLACK][i][j]});
                sumWeight[WHITE] -= weight[WHITE][i][j];
                sumWeight[BLACK] -= weight[BLACK][i][j];
                // 求出新权值记录并保存
                weight[BLACK][i][j] = myBoard.MarkOfPoint(i, j, BLACK);
                weight[WHITE][i][j] = myBoard.MarkOfPoint(i, j, WHITE);
                sumWeight[WHITE] += weight[WHITE][i][j];
                sumWeight[BLACK] += weight[BLACK][i][j];
                // 新增记录
                nextPos[MAX].insert(Position{
                    i, j, max(weight[WHITE][i][j], weight[BLACK][i][j])});
                // nextPos[WHITE].insert(Position{i, j, weight[WHITE][i][j]});
                // nextPos[BLACK].insert(Position{i, j, weight[BLACK][i][j]});
                break;
            }
        }
    }
}

LL Agent::Evaluate(int color) {
    return sumWeight[color] * 2 - sumWeight[color ^ 1];
}

#endif

int main() {
    ios::sync_with_stdio(false);
    auto ai = new Agent;
    ai->Run();
    delete ai;
    return 0;
}