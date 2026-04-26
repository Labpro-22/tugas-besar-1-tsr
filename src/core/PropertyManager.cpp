#include "../../include/core/PropertyManager.hpp"

std::unique_ptr<Board> PropertyManager::board = nullptr;

Board& PropertyManager::getBoard() {
    return *board;
}
void PropertyManager::assignOwnership(PropertyTile* tile, std::shared_ptr<Player> newOwner){}
bool PropertyManager::tryBuildHouse(std::shared_ptr<Player> player, StreetTile* tile){}
bool PropertyManager::tryBuildHotel(std::shared_ptr<Player> player, StreetTile* tile){}
bool PropertyManager::tryMortgage(std::shared_ptr<Player> player, PropertyTile* tile){}
bool PropertyManager::tryUnmortgage(std::shared_ptr<Player> player, PropertyTile* tile){}
float PropertyManager::getFinalRentPrice(PropertyTile* tile, int diceRoll) const{}
void PropertyManager::startFestival(PropertyTile* tile){}