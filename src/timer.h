#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
   public:
    Timer() { m_preTime = std::chrono::high_resolution_clock::now(); }

    void recordCurrent() { m_preTime = std::chrono::high_resolution_clock::now(); }

    int getTimePass() const {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - m_preTime);
        return duration.count();
    }

    const static int TIME_OUT = __INT32_MAX__;

   private:
    std::chrono::high_resolution_clock::time_point m_preTime;
};

#endif