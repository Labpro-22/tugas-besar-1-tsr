#pragma once

#include "ActionCard.hpp"
#include <random>
class JailActionCard : public ActionCard { 
    public:
        void onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
};