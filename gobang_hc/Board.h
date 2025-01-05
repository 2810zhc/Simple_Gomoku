#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Board {
public:
    static const int SIZE = 15; // ���̴�С
    Board();

    void print() const;  // ��ӡ����
    bool placeStone(int x, int y, char stone);  // ����
    char getCell(int x, int y) const;  // ��ȡ����ĳ������
    bool isFull() const;  // ��������Ƿ�����
    void removeStone(int x, int y);

private:
    std::vector<std::vector<char>> grid;  // ����
};

#endif // BOARD_H

