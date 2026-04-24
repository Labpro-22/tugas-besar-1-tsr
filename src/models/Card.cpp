#include "Card.hpp"

std::random_device Card::rd;
std::mt19937 Card::gen(Card::rd());

Card::Card(std::string name):name(name){}
std::string Card::getName(){
    return name;
}