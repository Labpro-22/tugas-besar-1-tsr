#pragma once

#include "ActionCard.hpp"

class StationActionCard : public ActionCard {
 
    public:
        StationActionCard(std::string name);
        void onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;


};