#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    Player(std::string name, char stone);

    std::string getName() const;
    char getStone() const;
    //std::string name;
    //char stone;  // 棋子的符号：'X'或'O'

private:
    std::string name;
    char stone;  // 棋子的符号：'X'或'O'
};

#endif // PLAYER_H
