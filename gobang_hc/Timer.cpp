#include "Timer.h"
#include <chrono>
#include <iostream>
#include <thread>

Timer::Timer(int moveLimitSec, int gameLimitSec)
    : moveLimit(std::chrono::seconds(moveLimitSec)), gameLimit(std::chrono::seconds(gameLimitSec)), running(false), moveTimeUp(false), gameTimeUp(false) {}

void Timer::startMoveTimer() {
    moveStartTime = std::chrono::high_resolution_clock::now();
    moveTimeUp = false;
    running = true;
}

void Timer::startGameTimer() {
    gameStartTime = std::chrono::high_resolution_clock::now();
    gameTimeUp = false;
    running = true;
}

void Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    running = false;
}

double Timer::getElapsedSeconds() const {
    using namespace std::chrono;
    if (running) {
        return duration_cast<duration<double>>(high_resolution_clock::now() - gameStartTime).count();
    }
    return duration_cast<duration<double>>(endTime - gameStartTime).count();
}

bool Timer::isMoveTimeUp() const {
    using namespace std::chrono;
    if (running) {
        return duration_cast<seconds>(high_resolution_clock::now() - moveStartTime) >= moveLimit;
    }
    return moveTimeUp;
}

bool Timer::isGameTimeUp() const {
    using namespace std::chrono;
    if (running) {
        return duration_cast<seconds>(high_resolution_clock::now() - gameStartTime) >= gameLimit;
    }
    return gameTimeUp;
}

void Timer::resetMoveTimer() {
    moveStartTime = std::chrono::high_resolution_clock::now();
    moveTimeUp = false;
}

void Timer::resetGameTimer() {
    gameStartTime = std::chrono::high_resolution_clock::now();
    gameTimeUp = false;
}

void Timer::checkMoveTimeout() {
    if (isMoveTimeUp()) {
        moveTimeUp = true;
        running = false;
    }
}
