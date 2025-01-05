#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    Player(std::string name, char stone);

    std::string getName() const;
    char getStone() const;
    //std::string name;
    //char stone;  // ���ӵķ��ţ�'X'��'O'

private:
    std::string name;
    char stone;  // ���ӵķ��ţ�'X'��'O'
};

#endif // PLAYER_H
