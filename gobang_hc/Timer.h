#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>

class Timer {
public:
    // ���캯��������ÿ��ʱ�����ƣ��룩����Ϸ��ʱ�����ƣ��룩
    Timer(int moveLimit, int gameLimit);

    // ����ÿ����ʱ��
    void startMoveTimer();
    // ������Ϸ��ʱ��
    void startGameTimer();
    // ֹͣ��ʱ��
    void stop();

    // ��ȡ����ʱ�䣨�룩
    double getElapsedSeconds() const;

    // �ж�ÿ���Ƿ�ʱ
    bool isMoveTimeUp() const;
    // �ж���Ϸ��ʱ���Ƿ����
    bool isGameTimeUp() const;

    // ����ÿ����ʱ��
    void resetMoveTimer();
    // ������Ϸ��ʱ��
    void resetGameTimer();

    // ����Ƿ�ʱ��ÿ����
    void checkMoveTimeout();

private:
    // ���ڴ洢ʱ���
    std::chrono::time_point<std::chrono::high_resolution_clock> moveStartTime, gameStartTime, endTime;

    // ���ڴ洢ʱ������
    std::chrono::duration<int> moveLimit, gameLimit;

    // ״̬����
    bool running;       // �Ƿ��������м�ʱ
    bool moveTimeUp;    // �Ƿ�ʱ��ÿ����
    bool gameTimeUp;    // �Ƿ�ʱ����Ϸ��ʱ�ޣ�
};

#endif // TIMER_H
