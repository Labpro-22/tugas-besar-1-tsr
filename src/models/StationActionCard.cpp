#include "../../include/models/StationActionCard.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/core/PropertyManager.hpp"

StationActionCard::StationActionCard(std::string name):ActionCard(name){}
void StationActionCard::onDraw(Player& p) {
    Board& b=PropertyManager::getBoard();
    int current_pos = p.getPosition();
    
    int total_tiles = b.getSize(); 
    for (int i = 1; i <= total_tiles; ++i) {
        int check_index = (current_pos + i) % total_tiles;
        Tile& ref = b.getTile(check_index);
        RailroadTile* station = dynamic_cast<RailroadTile*>(&ref);
        if (station != nullptr) {
            p.movePlayer(i);
            station->onLand(p);
            return;
        }
    }
}