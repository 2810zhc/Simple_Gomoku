#ifndef ROBOT_H
#define ROBOT_H

#include "Board.h"
#include "Player.h"
#include <iostream>
#include <vector>

class Robot : public Player {
public:
    Robot(char stone);  // ���캯����������������ӵķ��ţ�'X' �� 'O'��

    // �������ѡ�񣺸������̺͵�ǰ��ҵ�ʯ����������
    std::pair<int, int> makeMove(Board& board);  // ������������

private:
    // �򵥵Ĳ��ԣ�ѡ���ʤ����ֹ�Է���ʤ�����λ��
    void randomMove(Board& board);  // ���ѡ��һ���Ϸ��Ŀ�λ��
    bool canWin(Board& board, int x, int y, char stone);  // ����Ƿ��ܻ�ʤ
    bool canBlock(Board& board, int x, int y, char opponentStone);  // ����Ƿ���Ҫ��ֹ�Է�
    int evaluateMove(Board& board, int x, int y, char stone);  // ��������λ�õĵ÷�
    int minimax(Board& board, int depth, bool maximizingPlayer, int alpha, int beta, int x, int y);
    std::vector<std::pair<int, int>> getEmptyCells(Board& board);  // ��ȡ���пհ�λ��
    int evaluateBoard(Board& board, int x, int y);
    bool canFormLiveThree(Board& board, int x, int y, char opponentStone);
    bool checkLiveThreeDirection(const Board& board, int x, int y, char opponentStone, int dx, int dy);

};

#endif // ROBOT_H
