#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Board {
public:
    static const int SIZE = 15; // 棋盘大小
    Board();

    void print() const;  // 打印棋盘
    bool placeStone(int x, int y, char stone);  // 落子
    char getCell(int x, int y) const;  // 获取棋盘某点内容
    bool isFull() const;  // 检查棋盘是否下满
    void removeStone(int x, int y);

private:
    std::vector<std::vector<char>> grid;  // 棋盘
};

#endif // BOARD_H

