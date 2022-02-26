// #define ONLINE_JUDGE
#define ITERATIVE_DEEPENING

#ifndef CONST
#define CONST
#include <utility>
using std::pair;

// 放置一些公用常量

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

int SEARCHCNT[] = {0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 225};
const LL MARKS[][2] = {
    {10, 1}, {1000, 100}, {100000, 20000}, {10000000, 200000}};

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