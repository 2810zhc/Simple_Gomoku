#ifndef RULE_H
#define RULE_H

#include "Board.h"
#include <iostream>
class Rule {
public:
    Rule();  // ���캯��boardForbidden
    static bool checkWin(const Board& board, int x, int y, char stone);
    static bool isForbiddenMove(Board& board, int x, int y, char stone, int& liveThreeCount, int& liveFourCount);
    static bool checkLine(const Board& board, int x, int y, int dx, int dy, char stone);
    static int countConsecutive(const Board& board,int x, int y, int dx, int dy, char stone);
    static void expandInDirection(const Board& board, int startX, int startY, int dx, int dy, int& x_num, int& k_num);
    static void jumpThree(const Board& board, int startX, int startY, int dx, int dy, int& x_num, int& k_num);
private:
    int countThree;  // ��������
    int countFour;   // ���ļ���
};

#endif // RULE_H
