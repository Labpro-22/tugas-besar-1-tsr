#include "../../include/models/Card.hpp"

Card::Card(std::string name):name(name){}

std::random_device Card::rd;
std::mt19937 Card::gen(Card::rd());