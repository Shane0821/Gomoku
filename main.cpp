#include <cstring>

#include "judger.h"

int main(int argc, char* argv[]) {
    Judger judger;

    if (argc > 1 && std::strcmp(argv[1], "json") == 0) {
        Judger::JUDGER_MODE = Judger::MODE::ONLINE_JUDGE;
    }
    judger.startGame();
    return 0;
}