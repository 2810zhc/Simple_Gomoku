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

    // ������Ը����������;����ж�����
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
    // �ݹ�����������ﵽ�����Ȼ��⵽ʤ��
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

        // �ݹ����
        int score = minimax(board, depth - 1, !maximizingPlayer, alpha, beta,x,y);

        // ����
        board.removeStone(x, y);

        // �����ҵķ���
        if (maximizingPlayer) {
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
        }
        else {
            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
        }

        // Alpha-Beta��֦
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
   
    //�ʼ���ѡȡ
    if (emptyCells.size() >= (Board::SIZE * Board::SIZE - 1)){
        int bestX = std::rand() % Board::SIZE;  // 0 �� Board::SIZE-1 ֮��������
        int bestY = std::rand() % Board::SIZE;  // 0 �� Board::SIZE-1 ֮��������
        board.placeStone(bestX, bestY, getStone());
        // �������λ��
        return { bestX, bestY };
    }

    // ����ÿ���հ�λ�ã�ѡ�����λ��
    for (const auto& cell : emptyCells) {
        int x = cell.first;
        int y = cell.second;
        if (x == 1 && y == 4) {
            int x = 0;
        }
        // ����ѡ���ܻ�ʤ��λ��
        if (canWin(board, x, y, getStone())) {
            //board.removeStone(x, y);
            board.placeStone(x, y, getStone());
            return{x,y};
        }

        // ��ֹ�Է���ʤ
        char opponentStone = (getStone() == 'X') ? 'O' : 'X';
        if (canBlock(board, x, y, opponentStone)) {
            //board.removeStone(x, y);
            board.placeStone(x, y, getStone());
            return{ x,y };
        }

        // ������ǰ����λ�õĵ÷�
        //int score = evaluateMove(board, x, y, getStone());
        board.placeStone(x, y, getStone());

        int score = minimax(board, 2, false, -INT_MAX, INT_MAX, x, y);
        board.removeStone(x, y);  // ����
        // �������λ��
        if (score > bestScore) {
            bestScore = score;
            bestX = x;
            bestY = y;
        }
    }

    // ���û�л�ʤ����ֹ�Է������ӣ�ѡ���������λ��
    if (bestX != -1 && bestY != -1) {
        //board.removeStone(bestX, bestY);
        board.placeStone(bestX, bestY, getStone());
    }
    return { bestX, bestY };
}

bool Robot::canWin(Board& board, int x, int y, char stone) {
    board.placeStone(x, y, stone);
    bool win = Rule::checkWin(board, x, y, stone);  // ���� Rule::checkWin ��һ������Ƿ�ʤ���ĺ���
    board.removeStone(x, y);  // ���ظò�
    return win;
}

bool Robot::canBlock(Board& board, int x, int y, char opponentStone) {

    if (canWin(board, x, y, opponentStone)) {
        return true;
    }

    // ����Ƿ�����ֹ�Է��γɻ�����������3��+2����λ��
    if (canFormLiveThree(board, x, y, opponentStone)) {
        return true;
    }

    return false;
}
bool Robot::canFormLiveThree(Board& board, int x, int y, char opponentStone) {
    // ������п��ܵķ��򣺺����������Խ��ߡ����Խ���
    return checkLiveThreeDirection(board, x, y, opponentStone, 0, 1) ||  // ����
        checkLiveThreeDirection(board, x, y, opponentStone, 1, 0) ||  // ����
        checkLiveThreeDirection(board, x, y, opponentStone, 1, 1) ||  // ���Խ���
        checkLiveThreeDirection(board, x, y, opponentStone, 1, -1);   // ���Խ���
}

// ���ָ���������Ƿ�����γɻ���
bool Robot::checkLiveThreeDirection(const Board& board, int x, int y, char opponentStone, int dx, int dy) {
    
    Board board2 = board;
    board2.placeStone(x, y, opponentStone);

    int startX = x, startY = y;
    int x_num = 0;  // ��ʼ�������� 'X' ����
    int k_num = 0;  // ��ʼ����λ����

    // ���� (dx, dy) ��չ��ֱ�������߽�������ϵ���������
    while (x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE) {
        char cell = board2.getCell(x, y);

        if (cell == 'X') {
            // �ҵ������� 'X'
            x_num++;
        }
        else if (cell == '.') {
            // ��λ
            k_num++;
            break;  // ������λ��ֹͣ��չ
        }
        else {
            break;  // ��������Է������ӣ�ֹͣ��չ
        }

        // ����һ��λ����չ
        x += dx;
        y += dy;
    }

    x = startX;
    y = startY;
    while (x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE) {
        char cell = board2.getCell(x, y);

        if (cell == 'X') {
            // �ҵ������� 'X'
            x_num++;
        }
        else if (cell == '.') {
            // ��λ
            k_num++;
            break;  // ������λ��ֹͣ��չ
        }
        else {
            break;  // ��������Է������ӣ�ֹͣ��չ
        }

        // ����һ��λ����չ
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
            // ��������Ƿ���������
            if (nx >= 0 && nx < Board::SIZE && ny >= 0 && ny < Board::SIZE) {
                if (board2.getCell(nx, ny) == '.') {
                    // �������ֻ����Ľ��־�Ϊ����
                    int x_num9, k_num9;
                    Rule::jumpThree(board2, nx, ny, dx, y, x_num9, k_num9);   // �·��� (1, 0)
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
    //int count = 0;  // �����ĶԷ�������
    //int emptyCount = 0;  // ��λ��

    //// �������dx, dy��
    //for (int i = 0; i < 4; ++i) {
    //    int newX = x + i * dx;
    //    int newY = y + i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // Խ�磬����
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == 'X') {
    //        count++;
    //    }
    //    else if (current == '.') {
    //        emptyCount++;  // ��λ
    //        if (emptyCount == 2) {
    //            break;
    //        }
    //    }
    //}


    //// ��鷴��-dx, -dy��
    //int count2 = 0;
    //int emptyCount2 = 0;

    //for (int i = 0; i < 4; ++i) {
    //    int newX = x - i * dx;
    //    int newY = y - i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // Խ�磬����
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == 'X') {
    //        count2++;  // �Է�����
    //    }
    //    else if (current == '.') {
    //        emptyCount2++;  // ��λ
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
    //int count = 0;  // �����ĶԷ�������
    //int emptyCount = 0;  // ��λ��

    //// �������dx, dy��
    //for (int i = 0; i < 4; ++i) {
    //    int newX = x + i * dx;
    //    int newY = y + i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // Խ�磬����
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == opponentStone) {
    //        count++;  // �Է�����
    //    }
    //    else if (current == '.') {
    //        emptyCount++;  // ��λ
    //    }
    //}

    //// ��������������3���Է����� + 1����λ��ע���λ��������
    //if (count == 3 && emptyCount == 1) {
    //    return true;
    //}

    //// ��鷴��-dx, -dy��
    //int count2 = 0;
    //int emptyCount2 = 0;

    //for (int i = 0; i < 4; ++i) {
    //    int newX = x - i * dx;
    //    int newY = y - i * dy;
    //    if (newX < 0 || newX >= Board::SIZE || newY < 0 || newY >= Board::SIZE) {
    //        continue;  // Խ�磬����
    //    }

    //    char current = board.getCell(newX, newY);
    //    if (current == opponentStone) {
    //        count2++;  // �Է�����
    //    }
    //    else if (current == '.') {
    //        emptyCount2++;  // ��λ
    //    }
    //}

    //// �������Ҳ�����������
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
//    // ���㵱ǰ����λ�õĵ÷֣����ֻ��ƿ��Ը���ʵ��������е���
//    // ����򵥷���һ������������������ֵ
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
