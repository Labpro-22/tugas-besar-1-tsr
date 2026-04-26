#include "../../include/models/MoveActionCard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/core/PropertyManager.hpp"

std::random_device MoveActionCard::rd;
std::mt19937 MoveActionCard::gen(MoveActionCard::rd());

MoveActionCard::MoveActionCard(std::string name):ActionCard(name){
    std::uniform_int_distribution<> move(1,5);
    std::uniform_int_distribution<> forwardorbackwardhmmiwonder(0,1);
    walkDistance = forwardorbackwardhmmiwonder(gen)?move(gen):move(gen)*(-1);
}

void MoveActionCard::onDraw(Player& p){
    Board& b=PropertyManager::getBoard();
    p.movePlayer(walkDistance);
    Tile& t = b.getTile(p.getPosition());
    t.onLand(p);

}