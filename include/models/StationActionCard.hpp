#pragma once

#include "ActionCard.hpp"

class StationActionCard : public ActionCard {
 
    public:
        StationActionCard(std::string name);
        void onDraw(Player& p, Board& b, std::vector<std::shared_ptr<Player>>& all, TileVisitor& visitor) override;


};