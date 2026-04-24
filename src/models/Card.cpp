#include "Card.hpp"

std::random_device Card::rd;
std::mt19937 Card::gen(Card::rd());