#ifndef ROBOT_H
#define ROBOT_H

#include "Board.h"
#include "Player.h"
#include <iostream>
#include <vector>

class Robot : public Player {
public:
    Robot(char stone);  // 构造函数，传入机器人棋子的符号（'X' 或 'O'）

    // 计算机的选择：根据棋盘和当前玩家的石子做出决策
    std::pair<int, int> makeMove(Board& board);  // 返回落子坐标

private:
    // 简单的策略：选择获胜或阻止对方获胜的最佳位置
    void randomMove(Board& board);  // 随机选择一个合法的空位置
    bool canWin(Board& board, int x, int y, char stone);  // 检查是否能获胜
    bool canBlock(Board& board, int x, int y, char opponentStone);  // 检查是否需要阻止对方
    int evaluateMove(Board& board, int x, int y, char stone);  // 评估落子位置的得分
    int minimax(Board& board, int depth, bool maximizingPlayer, int alpha, int beta, int x, int y);
    std::vector<std::pair<int, int>> getEmptyCells(Board& board);  // 获取所有空白位置
    int evaluateBoard(Board& board, int x, int y);
    bool canFormLiveThree(Board& board, int x, int y, char opponentStone);
    bool checkLiveThreeDirection(const Board& board, int x, int y, char opponentStone, int dx, int dy);

};

#endif // ROBOT_H
