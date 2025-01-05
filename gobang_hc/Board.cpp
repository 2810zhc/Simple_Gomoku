#include "Board.h"
#include <iostream>
#include <iomanip>

Board::Board() : grid(SIZE, std::vector<char>(SIZE, '.')) {}

void Board::print() const {
    std::cout << "  ";
    for (int i = 0; i < SIZE; ++i) std::cout << std::setw(3) << i;
    std::cout << "\n";

    for (int i = 0; i < SIZE; ++i) {
        std::cout << std::setw(3) << i << " ";
        for (int j = 0; j < SIZE; ++j) {
            std::cout << grid[i][j] << "  ";
        }
        std::cout << "\n";
    }
}

bool Board::placeStone(int x, int y, char stone) {
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && grid[x][y] == '.') {
        grid[x][y] = stone;
        return true;
    }
    return false;
}

char Board::getCell(int x, int y) const {
    return (x >= 0 && x < SIZE&& y >= 0 && y < SIZE) ? grid[x][y] : '\0';
}

bool Board::isFull() const {
    for (const auto& row : grid)
        for (char cell : row)
            if (cell == '.')
                return false;
    return true;
}

void Board::removeStone(int x, int y) {
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        grid[x][y] = '.';  // Çå¿Õ¸ÃÎ»ÖÃ
    }
}