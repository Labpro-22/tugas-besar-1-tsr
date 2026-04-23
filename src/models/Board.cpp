#include "Board.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include <stdexcept>
#include <algorithm>

Tile& Board::getTile(int index) {
    if (tiles.empty()) {
        throw std::out_of_range("Papan masih kosong, tidak ada Tile!");
    }
    
    index = index % tiles.size(); 
    return *tiles[index];
}

void Board::addTile(std::unique_ptr<Tile> tile) {
    if (!tile) return;

    std::string color_group = tile->getColor(); 
    Tile* tile_ptr = tile.get();
    
    tiles.push_back(std::move(tile));

    if (!color_group.empty()) {
        color_tile_map[color_group].push_back(tile_ptr);
    }
}

const std::vector<Tile*>& Board::getPropertiesByGroup(const std::string& color) const {
    auto it = color_tile_map.find(color);
    if (it != color_tile_map.end()) {
        return it->second;
    }
    
    static const std::vector<Tile*> empty_list; 
    return empty_list;
}

std::vector<std::string> Board::getAllGroups() const {
    std::vector<std::string> groups;
    groups.reserve(color_tile_map.size()); 
    
    for (const auto& pair : color_tile_map) {
        groups.push_back(pair.first);
    }
    
    return groups;
}

bool Board::checkMonopoly(const Player& player, const std::string& color) const {
    auto it = color_tile_map.find(color);
    if (it == color_tile_map.end() || it->second.empty()) {
        return false; 
    }

    const std::vector<Tile*>& group_tiles = it->second;

    for (Tile* current_tile : group_tiles) {
        if (current_tile->owner() != &player) {
            return false; 
        }
    }

    return true; 
}