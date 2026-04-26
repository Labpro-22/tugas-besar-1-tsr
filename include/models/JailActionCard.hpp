#pragma once

#include "ActionCard.hpp"
#include <random>
class JailActionCard : public ActionCard { 
    public:
        JailActionCard(std::string name);
        void onDraw(Player& p) override;
};