#include "Robot.h"
#include "Board.h"
#include "Player.h"
#include "Rule.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

Robot::Robot(char stone) : Player("Robot", stone) {}

int Robot::evaluateBoard(Board& board , int x, int y) {
    int score = 0;

    // 这里可以根据连子数和局面判断评分
    const int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };

    for (int i = 0; i < 8; i += 2) {
        int count = Rule::countConsecutive(board, x, y, directions[i][0], directions[i][1], getStone());
        int count2 = Rule::countConsecutive(board, x, y, directions[i + 1][0], directions[i + 1][1], getStone());
        score += count + count2;
    }


    return score;
}


int Robot::minimax(Board& board, int depth, bool maximizingPlayer, int alpha, int beta,int x, int y) {
    
    int sorce = evaluateBoard(board,x,y);
    // 递归结束条件：达到最大深度或检测到胜负
    if (depth == 0) {
        return sorce;
    }

    int bestScore = maximizingPlayer ? -INT_MAX : INT_MAX;
    std::vector<std::pair<int, int>> emptyCells = getEmptyCells(board);

    for (const auto& cell : emptyCells) {
        int x = cell.first;
        int y = cell.second;
        char opponentStone = (getStone() == 'X') ? 'O' : 'X';
        board.placeStone(x, y, maximizingPlayer ? getStone() : opponentStone);

        // 递归调用
        int score = minimax(board, depth - 1, !maximizingPlayer, alpha, beta,x,y);

        // 回溯
        board.removeStone(x, y);

        // 最大化玩家的分数
        if (maximizingPlayer) {
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
        }
        else {
            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
        }

        // Alpha-Beta剪枝
        if (beta <= alpha) {
            break;
        }
    }

    return bestScore;
}


std::pair<int, int>  Robot::makeMove(Board& board) {
    int bestScore = -10000;
    int bestX = -1, bestY = -1;
    std::vector<std::pair<int, int>> emptyCells = getEmptyCells(board);
   
    //最开始随机选取
    if (emptyCells.size() >= (Board::SIZE * Board::SIZE - 1)){
        int bestX = std::rand() % Board::SIZE;  // 0 到 Board::SIZE-1 之间的随机数
        int bestY = std::rand() % Board::SIZE;  // 0 到 Board::SIZE-1 之间的随机数
        board.placeStone(bestX, bestY, getStone());
        // 返回随机位置
        return { bestX, bestY };
    }

    // 尝试每个空白位置，选择最佳位置
    for (const auto& cell : emptyCells) {
        int x = cell.first;
        int y = cell.second;
        if (x == 1 && y == 4) {
            int x = 0;
        }
        // 优先选择能获胜的位置
        if (canWin(board, x, y, getStone())) {
            //board.removeStone(x, y);
            board.placeStone(x, y, getStone());
            return{x,y};
        }

        // 阻止对方获胜
        char opponentStone = (getStone() == 'X') ? 'O' : 'X';
        if (canBlock(board, x, y, opponentStone)) {
            //board.removeStone(x, y);
            board.placeStone(x, y, getStone());
            return{ x,y };
        }

        // 评估当前落子位置的得分
        //int score = evaluateMove(board, x, y, getStone());
        board.placeStone(x, y, getStone());

        int score = minimax(board, 2, false, -INT_MAX, INT_MAX, x, y);
        board.removeStone(x, y);  // 回溯
        // 更新最佳位置
        if (score > bestScore) {
            bestScore = score;
            bestX = x;
            bestY = y;
        }
    }

    // 如果没有获胜或阻止对方的落子，选择最佳评估位置
    if (bestX != -1 && bestY != -1) {
        //board.removeStone(bestX, bestY);
        board.placeStone(bestX, bestY, getStone());
    }
    return { bestX, bestY };
}

bool Robot::canWin(Board& board, int x, int y, char stone) {
    board.placeStone(x, y, stone);
    bool win = Rule::checkWin(board, x, y, stone);  // 假设 Rule::checkWin 是一个检查是否胜利的函数
    board.removeStone(x, y);  // 撤回该步
    return win;
}

bool Robot::canBlock(Board& board, int x, int y, char opponentStone) {

    if (canWin(board, x, y, opponentStone)) {
        return true;
    }

    // 检查是否能阻止对方形成活三（即连续3子+2个空位）
    if (canFormLiveThree(board, x, y, opponentStone)) {
        return true;
    }

    return false;
}
bool Robot::canFormLiveThree(Board& board, int x, int y, char opponentStone) {
    // 检查所有可能的方向：横向、纵向、主对角线、副对角线
    return checkLiveThreeDirection(board, x, y, opponentStone, 0, 1) ||  // 横向
        checkLiveThreeDirection(board, x, y, opponentStone, 1, 0) ||  // 纵向
        checkLiveThreeDirection(board, x, y, opponentStone, 1, 1) ||  // 主对角线
        checkLiveThreeDirection(board, x, y, opponentStone, 1, -1);   // 副对角线
}

// 检查指定方向上是否可以形成活三
bool Robot::checkLiveThreeDirection(const Board& board, int x, int y, char opponentStone, int dx, int dy) {
    
    Board board2 = board;
    board2.placeStone(x, y, opponentStone);

    int startX = x, startY = y;
    int x_num = 0;  // 初始化连续的 'X' 数量
    int k_num = 0;  // 初始化空位数量

    // 向方向 (dx, dy) 扩展，直到遇到边界或棋盘上的其它棋子
    while (x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE) {
        char cell = board2.getCell(x, y);

        if (cell == 'X') {
            // 找到连续的 'X'
            x_num++;
        }
        else if (cell == '.') {
            // 空位
            k_num++;
            break;  // 遇到空位后停止扩展
        }
        else {
            break;  // 如果遇到对方的棋子，停止扩展
        }

        // 向下一个位置扩展
        x += dx;
        y += dy;
    }

    x = startX;
    y = startY;
    while (x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE) {
        char cell = board2.getCell(x, y);

        if (cell == 'X') {
            // 找到连续的 'X'
            x_num++;
        }
        else if (cell == '.') {
            // 空位
            k_num++;
            break;  // 遇到空位后停止扩展
        }
        else {
            break;  // 如果遇到对方的棋子，停止扩展
        }

        // 向下一个位置扩展
        x -= dx;
        y -= dy;
    }
    if (x_num == 4 && k_num == 2) {
        board2.removeStone(x, y);
        return true;
    }
    if (x_num == 5 && k_num == 2) {
        board2.removeStone(x, y);
        return true;
    }
    else {
        board2.removeStone(x, y);
    }
    //board.removeStone(x, y);


    board2.placeStone(x, y, opponentStone);
    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            int nx = x + i;
            int ny = y + j;
            if (nx == 1 && ny == 3) {
                int x = 0;
            }
            // 检查索引是否在棋盘内
            if (nx >= 0 && nx < Board::SIZE && ny >= 0 && ny < Board::SIZE) {
                if (board2.getCell(nx, ny) == '.') {
                    // 三三禁手或四四禁手均为禁手
                    int x_num9, k_num9;
                    Rule::jumpThree(board2, nx, ny, dx, y, x_num9, k_num9);   // 下方向 (1, 0)
                    if (x_num9 == 3 && k_num9 == 4) {

                        for (int i = 0; i < 3; ++i) {
                            int newX1 = nx + i * dx;
                            int newY1 = ny + i * dy;
                            int newX2 = nx - i * dx;
                            int newY2 = ny - i * dy;
                            if ((newX1 == x && newY1 == y) || (newX2 == x && newY2 == y)) {
                                board2.removeStone(x, y);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    //int count = 0;  // 连续的对方棋子数
    //int emptyCount = 0;  // 空位数

    //// 检查正向（dx, dy）
    //for (int i = 0; i < 4; ++i) {
    //    int newX = x + i * dx;
    //    int newY = y + i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // 越界，跳过
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == 'X') {
    //        count++;
    //    }
    //    else if (current == '.') {
    //        emptyCount++;  // 空位
    //        if (emptyCount == 2) {
    //            break;
    //        }
    //    }
    //}


    //// 检查反向（-dx, -dy）
    //int count2 = 0;
    //int emptyCount2 = 0;

    //for (int i = 0; i < 4; ++i) {
    //    int newX = x - i * dx;
    //    int newY = y - i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // 越界，跳过
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == 'X') {
    //        count2++;  // 对方棋子
    //    }
    //    else if (current == '.') {
    //        emptyCount2++;  // 空位
    //        if (emptyCount2 == 2) {
    //            break;
    //        }
    //    }
    //}
    //int x_num = count2 + count;
    //int k_num = emptyCount2 + emptyCount;
    //if (x_num == 3 && k_num == 4) {
    //    return true;
    //}
    //
    //
    //int count = 0;  // 连续的对方棋子数
    //int emptyCount = 0;  // 空位数

    //// 检查正向（dx, dy）
    //for (int i = 0; i < 4; ++i) {
    //    int newX = x + i * dx;
    //    int newY = y + i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // 越界，跳过
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == opponentStone) {
    //        count++;  // 对方棋子
    //    }
    //    else if (current == '.') {
    //        emptyCount++;  // 空位
    //    }
    //}

    //// 检查活三的条件：3个对方棋子 + 1个空位（注意空位的数量）
    //if (count == 3 && emptyCount == 1) {
    //    return true;
    //}

    //// 检查反向（-dx, -dy）
    //int count2 = 0;
    //int emptyCount2 = 0;

    //for (int i = 0; i < 4; ++i) {
    //    int newX = x - i * dx;
    //    int newY = y - i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // 越界，跳过
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == opponentStone) {
    //        count2++;  // 对方棋子
    //    }
    //    else if (current == '.') {
    //        emptyCount2++;  // 空位
    //    }
    //}

    //// 如果反向也满足活三条件
    //if (count2 == 3 && emptyCount2 == 1) {
    //    return true;
    //}

    //if ((count2 + count) == 3) {
    //    return true;
    //}
    board2.removeStone(x, y);
    return false;
}
//int Robot::evaluateMove(Board& board, int x, int y, char stone) {
//    // 计算当前落子位置的得分，评分机制可以根据实际需求进行调整
//    // 这里简单返回一个基于连子数的评估值
//    int score = 0;
//    if (board.getCell(x, y) == '.') {
//        const int directions[8][2] = {
//         {-1, 0}, {1, 0}, {0, -1}, {0, 1},
//         {-1, -1}, {1, 1}, {-1, 1}, {1, -1} 
//            };
//        for (int i = 0; i < 8; i += 2) {
//            int count = Rule::countConsecutive(board, x, y, directions[i][0], directions[i][1], stone);
//            int count2 = Rule::countConsecutive(board, x, y, directions[i + 1][0], directions[i + 1][1], stone);
//            score += count + count2;
//        }
//    }
//    return score;
//}


std::vector<std::pair<int, int>> Robot::getEmptyCells(Board& board) {
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            if (board.getCell(i, j) == '.') {
                emptyCells.push_back({ i, j });
            }
        }
    }
    return emptyCells;
}
