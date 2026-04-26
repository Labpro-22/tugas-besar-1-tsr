#pragma once

#include "Card.hpp"
class ActionCard: public Card
{
private:
    /* data */
public:
    ActionCard(std::string name);
    std::string toSaveFormat() const override;
    
};