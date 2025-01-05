#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>

class Timer {
public:
    // 构造函数：传入每步时间限制（秒）和游戏总时间限制（秒）
    Timer(int moveLimit, int gameLimit);

    // 启动每步计时器
    void startMoveTimer();
    // 启动游戏计时器
    void startGameTimer();
    // 停止计时器
    void stop();

    // 获取已用时间（秒）
    double getElapsedSeconds() const;

    // 判断每步是否超时
    bool isMoveTimeUp() const;
    // 判断游戏总时限是否结束
    bool isGameTimeUp() const;

    // 重置每步计时器
    void resetMoveTimer();
    // 重置游戏计时器
    void resetGameTimer();

    // 检查是否超时（每步）
    void checkMoveTimeout();

private:
    // 用于存储时间点
    std::chrono::time_point<std::chrono::high_resolution_clock> moveStartTime, gameStartTime, endTime;

    // 用于存储时间限制
    std::chrono::duration<int> moveLimit, gameLimit;

    // 状态变量
    bool running;       // 是否正在运行计时
    bool moveTimeUp;    // 是否超时（每步）
    bool gameTimeUp;    // 是否超时（游戏总时限）
};

#endif // TIMER_H
