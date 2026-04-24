#include "../../include/models/MoveActionCard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/PropertyTile.hpp"

std::random_device MoveActionCard::rd;
std::mt19937 MoveActionCard::gen(MoveActionCard::rd());

MoveActionCard::MoveActionCard(){
    std::uniform_int_distribution<> move(1,5);
    std::uniform_int_distribution<> forwardorbackwardhmmiwonder(0,1);
    walkDistance = forwardorbackwardhmmiwonder(gen)?move(gen):move(gen)*(-1);
}

void MoveActionCard::onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor){
    p.movePlayer(walkDistance);
    Tile& t = b.getTile(p.getPosition());
    PropertyTile* pt = dynamic_cast<PropertyTile*>(&t);
    if(pt!=nullptr){
        pt->onLand(p,visitor);
    }
}