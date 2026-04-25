#include "../../include/models/JailActionCard.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/core/GameManager.hpp"


void JailActionCard::onDraw(Player& p){
    auto& b=PropertyManager::getBoard();
    int current_pos = p.getPosition();
    
    int total_tiles = b.getSize(); 
    for (int i = 1; i <= total_tiles; ++i) {
        int check_index = (current_pos + i) % total_tiles;
        Tile& ref = b.getTile(check_index);
        GoToJailTile* jail = dynamic_cast<GoToJailTile*>(&ref);
        if (jail != nullptr) {
            p.movePlayer(i);
            jail->onLand(p);
            
            return;
        }
    }
}