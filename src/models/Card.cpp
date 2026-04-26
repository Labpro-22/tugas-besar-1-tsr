#include "../../include/models/Card.hpp"

Card::Card(std::string name):name(name){}
std::string Card::getName() const{
    return name;
}

std::random_device Card::rd;
std::mt19937 Card::gen(Card::rd());