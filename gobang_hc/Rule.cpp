#include "Rule.h"


bool Rule::checkWin(const Board& board, int x, int y, char stone) {
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };//方向变换
    for (const auto& dir : directions) {
        int count = 1;
        for (int step = 1; step < 5; ++step) {
            int nx = x + step * dir[0], ny = y + step * dir[1];
            if (board.getCell(nx, ny) == stone) count++;
            else break;
        }
        for (int step = 1; step < 5; ++step) {
            int nx = x - step * dir[0], ny = y - step * dir[1];
            if (board.getCell(nx, ny) == stone) count++;
            else break;
        }
        if (count >= 5) return true;
    }
    return false;
}

bool Rule::isForbiddenMove(Board& board, int x, int y, char stone, int& liveThreeCount, int& liveFourCount) {
    if (stone != 'X') return false;  // 只有黑棋需要检测禁手
    board.removeStone(x, y);
    int temp_three = liveThreeCount;
    int flag1 = 0; int flag2 = 0;
    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            if (board.getCell(i, j) == 'X') {
               //往各个方位生长
                int x_num, k_num;
                expandInDirection(board, i, j, 1, 0, x_num, k_num);   // 下方向 (1, 0)
                if (x_num == 4 && k_num == 2) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }
                if (x_num == 5 && k_num == 2) {
                    liveFourCount++;  // 存在活四
                    flag2 = 1;
                }

                int x_num2, k_num2;
                expandInDirection(board, i, j, 0, 1, x_num2, k_num2);   // 右方向 (0, 1)
                if (x_num2 == 4 && k_num2 == 2) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }
                if (x_num2 == 5 && k_num2 == 2) {
                    liveFourCount++;  // 存在活四
                    flag2 = 1;
                }

                int x_num3, k_num3;
                expandInDirection(board, i, j, 1, 1, x_num3, k_num3);   // 右下方向 (1, 1)
                if (x_num3 == 4 && k_num3 == 2) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }
                if (x_num3 == 5 && k_num3 == 2) {
                    liveFourCount++;  // 存在活四
                    flag2 = 1;
                }

                int x_num4, k_num4;
                expandInDirection(board, i, j, 1, -1, x_num4, k_num4);  // 左下方向 (1, -1)
                if (x_num4 == 4 && k_num4 == 2) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }
                if (x_num4 == 5 && k_num4 == 2) {
                    liveFourCount++;  // 存在活四
                    flag2 = 1;
                }
            }

            else {
                //检查跳跃活三
                int x_num, k_num;
                jumpThree(board, i, j, 1, 0, x_num, k_num);   // 下方向 (1, 0)
                if (x_num == 3 && k_num == 4) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }

                int x_num2, k_num2;
                jumpThree(board, i, j, 0, 1, x_num2, k_num2);   // 右方向 (0, 1)
                if (x_num2 == 3 && k_num2 == 4) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }


                int x_num3, k_num3;
                jumpThree(board, i, j, 1, 1, x_num3, k_num3);   // 右下方向 (1, 1)
                if (x_num3 == 3 && k_num3 == 4) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }

                int x_num4, k_num4;
                jumpThree(board, i, j, 1, -1, x_num4, k_num4);  // 左下方向 (1, -1)
                if (x_num4 == 3 && k_num4 == 4) {
                    liveThreeCount++;  // 存在活三
                    flag1 = 1;
                }

            }
        }
    }
    if (flag1 == 1) {
        liveThreeCount = 1;
    }
    else {
        liveThreeCount = 0;
    }
    if (flag2 == 1) {
        liveFourCount = 1;
    }
    else {
        liveFourCount = 0;
    }

    board.placeStone(x, y,stone);
    bool count_checkline = false;
    // 检查黑棋是否存在三三禁手和四四禁手
    const int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},  // 横、竖
        {-1, -1}, {1, 1}, {-1, 1}, {1, -1} // 对角线
    };
    for (int i = 0; i < 8; i += 2) {
        bool count_checkline_div = checkLine(board, x, y, directions[i][0], directions[i][1], stone);

        if (count_checkline_div) {
            count_checkline = true;
            std::cout << "5" << std::endl;
        }
    }

    int x_num5, k_num5;
    expandInDirection(board, x, y, 1, 0, x_num5, k_num5);   // 下方向 (1, 0)
    if (x_num5 == 4 && k_num5 == 2) {
        liveThreeCount++;  // 存在活三
    }
    if (x_num5 == 5 && k_num5 == 2) {
        liveFourCount++;  // 存在活四
    }

    int x_num6, k_num6;
    expandInDirection(board, x, y, 0, 1, x_num6, k_num6);   // 右方向 (0, 1)
    if (x_num6 == 4 && k_num6 == 2) {
        liveThreeCount++;  // 存在活三
    }
    if (x_num6 == 5 && k_num6 == 2) {
        liveFourCount++;  // 存在活四
    }

    int x_num7, k_num7;
    expandInDirection(board, x, y, 1, 1, x_num7, k_num7);   // 右下方向 (1, 1)
    if (x_num7 == 4 && k_num7 == 2) {
        liveThreeCount++;  // 存在活三
    }
    if (x_num7 == 5 && k_num7 == 2) {
        liveFourCount++;  // 存在活四
    }

    int x_num8, k_num8;
    expandInDirection(board, x, y, 1, -1, x_num8, k_num8);  // 左下方向 (1, -1)
    if (x_num8 == 4 && k_num8 == 2) {
        liveThreeCount++;  // 存在活三
    }
    if (x_num8 == 5 && k_num8 == 2) {
        liveFourCount++;  // 存在活四
    }
    if (liveThreeCount == temp_three) {
        //找到x y 【-2，2】 5*5矩阵内的‘.’
        for (int i = -2; i <= 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                int nx = x + i;
                int ny = y + j;
                if (nx == 1 && ny == 3) {
                    int x = 0;
                }
                // 检查索引是否在棋盘内
                if (nx >= 0 && nx < Board::SIZE && ny >= 0 && ny < Board::SIZE) {
                    if (board.getCell(nx, ny) == '.') {
                        // 三三禁手或四四禁手均为禁手
                        int x_num9, k_num9;
                        jumpThree(board, nx, ny, 1, 0, x_num9, k_num9);   // 下方向 (1, 0)
                        if (x_num9 == 3 && k_num9 == 4) {
                          
                            for (int i = 0; i < 3; ++i) {
                                int newX1 = nx + i * 1;
                                int newY1 = ny + i * 0;
                                int newX2 = nx - i * 1;
                                int newY2 = ny - i * 0;
                                if ((newX1 == x && newY1 == y) || (newX2 == x && newY2 == y)) {
                                    liveThreeCount++;  // 存在活三
                                }
                            }
                        }

                        int x_num10, k_num10;
                        jumpThree(board, nx, ny, 0, 1, x_num10, k_num10);   // 右方向 (0, 1)
                        if (x_num10 == 3 && k_num10 == 4) {
                            for (int i = 0; i < 3; ++i) {
                                int newX1 = nx + i * 0;
                                int newY1 = ny + i * 1;
                                int newX2 = nx - i * 0;
                                int newY2 = ny - i * 1;
                                if ((newX1 == x && newY1 == y) || (newX2 == x && newY2 == y)) {
                                    liveThreeCount++;  // 存在活三
                                }
                            }

                        }


                        int x_num11, k_num11;
                        jumpThree(board, nx, ny, 1, 1, x_num11, k_num11);   // 右下方向 (1, 1)
                        if (x_num11 == 3 && k_num11 == 4) {
                            for (int i = 0; i < 3; ++i) {
                                int newX1 = nx + i * 1;
                                int newY1 = ny + i * 1;
                                int newX2 = nx - i * 1;
                                int newY2 = ny - i * 1;
                                if ((newX1 == x && newY1 == y) || (newX2 == x && newY2 == y)) {
                                    liveThreeCount++;  // 存在活三
                                }
                            }
                        }

                        int x_num12, k_num12;
                        jumpThree(board, nx, ny, 1, -1, x_num12, k_num12);  // 左下方向 (1, -1)
                        if (x_num12 == 3 && k_num12 == 4) {
                            for (int i = 0; i < 3; ++i) {
                                int newX1 = nx + i * 1;
                                int newY1 = ny + i * -1;
                                int newX2 = nx - i * 1;
                                int newY2 = ny - i * -1;
                                if ((newX1 == x && newY1 == y) || (newX2 == x && newY2 == y)) {
                                    liveThreeCount++;  // 存在活三
                                }
                            }
                        }
                    }
                }
            }
        }
        
    }
  


    return  liveThreeCount==2 || liveFourCount==2 || count_checkline;
}

bool Rule::checkLine(const Board& board, int x, int y, int dx, int dy, char stone) {
    int count = 0;
    for (int i = -4; i <= 4; ++i) {
        int nx = x + i * dx, ny = y + i * dy;
        if (nx >= 0 && nx < Board::SIZE && ny >= 0 && ny < Board::SIZE && board.getCell(nx, ny) == stone) {
            count++;
        }
    }
    return count > 5; // 检查是否为长连
}

int Rule::countConsecutive(const Board& board, int x, int y, int dx, int dy, char stone) {
    int count = 0;
    int nx = x, ny = y;

    while (nx >= 0 && nx < Board::SIZE && ny >= 0 && ny < Board::SIZE && board.getCell(nx, ny) == stone) {
        count++;
        nx += dx;
        ny += dy;
    }


    return count;
}


void Rule::expandInDirection(const Board& board, int startX, int startY, int dx, int dy, int& x_num, int& k_num) {
    int x = startX, y = startY;
    x_num = 0;  // 初始化连续的 'X' 数量
    k_num = 0;  // 初始化空位数量

    // 向方向 (dx, dy) 扩展，直到遇到边界或棋盘上的其它棋子
    while (x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE) {
        char cell = board.getCell(x, y);

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
        char cell = board.getCell(x, y);

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
}
void Rule::jumpThree(const Board& board, int startX, int startY, int dx, int dy, int& x_num, int& k_num) {
    int count = 0;  // 连续的对方棋子数
    int emptyCount = 0;  // 空位数

    // 检查正向（dx, dy）
    for (int i = 0; i < 4; ++i) {
        int newX = startX + i * dx;
        int newY = startY + i * dy;
        if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
            continue;  // 越界，跳过
        }

        char current = board.getCell(newX, newY);
        if (current == 'X') {
            count++;  
        }
        else if (current == '.') {
            emptyCount++;  // 空位
            if (emptyCount == 2) {
                break;
            }
        }
    }


    // 检查反向（-dx, -dy）
    int count2 = 0;
    int emptyCount2 = 0;

    for (int i = 0; i < 4; ++i) {
        int newX = startX - i * dx;
        int newY = startY - i * dy;
        if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
            continue;  // 越界，跳过
        }

        char current = board.getCell(newX, newY);
        if (current == 'X') {
            count2++;  // 对方棋子
        }
        else if (current == '.') {
            emptyCount2++;  // 空位
            if (emptyCount2 == 2) {
                break;
            }
        }
    }
    x_num = count2 + count;
    k_num = emptyCount2 + emptyCount;
}

