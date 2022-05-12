#ifdef RELEASE
#pragma GCC optimize(3, \
                     "Ofast,no-stack-protector,unroll-loops,fast-math,inline")
#endif
#include <bits/stdc++.h>

#include "Class/Agent.hpp"

using namespace std;

int main() {
    srand(time(0));
    ios::sync_with_stdio(false);
    auto ai = new Agent;
    ai->Run();
    delete ai;
    return 0;
}