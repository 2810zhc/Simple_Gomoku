#include "Player.h"

Player::Player(std::string name, char stone) : name(std::move(name)), stone(stone) {}

std::string Player::getName() const {
    return name;
}

char Player::getStone() const {
    return stone;
}
