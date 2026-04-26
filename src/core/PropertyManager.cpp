#include "../../include/core/PropertyManager.hpp"
#include "../../include/core/TransactionLog.hpp"
#include "../../include/core/GameManager.hpp"


Tile& PropertyManager::getTileAt(int position) const{
    return board->getTile(position);
}

void PropertyManager::assignOwnership(PropertyTile* tile, std::shared_ptr<Player> newOwner){
    tile->setPropertyOwner(newOwner);
    tile->setPropertyStatus(OWNED);
}


bool PropertyManager::tryBuildHouse(std::shared_ptr<Player> player, StreetTile* tile){
    if (!player || !tile || !board) {
        return false;
    }

    if (tile->getPropertyType() != PropertyType::STREET) {
        return false;
    }

    std::shared_ptr<Player> owner = tile->getPropertyOwner().lock();
    if (!owner || owner.get() != player.get()) {
        return false;
    }

    if (tile->getPropertyStatus() != OWNED) {
        return false;
    }

    if (!board->checkMonopoly(*player, tile->getColor())) {
        return false;
    }

    if (!tile->canBuildHouse()) {
        return false;
    }

    const std::vector<Tile*>& sameColorTiles = board->getPropertiesByGroup(tile->getColor());
    const int currentLevel = tile->getBuildingLevel();

    for (Tile* groupedTile : sameColorTiles) {
        StreetTile* groupedStreet = dynamic_cast<StreetTile*>(groupedTile);
        if (groupedStreet == nullptr) {
            return false;
        }

        if (groupedStreet->getBuildingLevel() < currentLevel) {
            return false;
        }
    }

    tile->buildHouse();
    return true;
}
    

bool PropertyManager::tryBuildHotel(std::shared_ptr<Player> player, StreetTile* tile){
    if (!player || !tile || !board) {
        return false;
    }
    if (tile->getPropertyType() != PropertyType::STREET) {
        return false;
    }
    std::shared_ptr<Player> owner = tile->getPropertyOwner().lock();
    if (!owner || owner.get() != player.get()) {
        return false;
    }
    if (tile->getPropertyStatus() != OWNED) {
        return false;
    }
    if (!board->checkMonopoly(*player, tile->getColor())) {
        return false;
    }
    if (!tile->canBuildHotel()) {
        return false;
    }
    const std::vector<Tile*>& sameColorTiles = board->getPropertiesByGroup(tile->getColor());
    const int currentLevel = tile->getBuildingLevel();

    for (Tile* groupedTile : sameColorTiles) {
        StreetTile* groupedStreet = dynamic_cast<StreetTile*>(groupedTile);
        if (groupedStreet == nullptr) {
            return false;
        }

        if (groupedStreet->getBuildingLevel() < currentLevel) {
            return false;
        }
    }

    tile->buildHotel();
    return true;
}

bool PropertyManager::tryMortgage(std::shared_ptr<Player> player, PropertyTile* tile){
    if (!player || !tile || !board) {
        return false;
    }

    std::shared_ptr<Player> owner = tile->getPropertyOwner().lock();
    if (!owner || owner.get() != player.get()) {
        return false;
    }

    if (tile->getPropertyStatus() != OWNED) {
        return false;
    }

    if (tile->getPropertyType() == PropertyType::STREET) {
        StreetTile* street = dynamic_cast<StreetTile*>(tile);
        if (street == nullptr) {
            return false;
        }

        while (street->canSellBuilding()) {
            street->sellBuilding();
        }
    }

    tile->setPropertyStatus(MORTGAGED);
    GameManager::economy_manager->addMoney(*player, tile->getMortgageValue());
    return true;
}

bool PropertyManager::tryUnmortgage(std::shared_ptr<Player> player, PropertyTile* tile){
    if (!player || !tile || !board) {
        return false;
    }

    std::shared_ptr<Player> owner = tile->getPropertyOwner().lock();
    if (!owner || owner.get() != player.get()) {
        return false;
    }

    if (tile->getPropertyStatus() != MORTGAGED) {
        return false;
    }

    if (tile->getPropertyType() == PropertyType::STREET) {
        StreetTile* street = dynamic_cast<StreetTile*>(tile);
        if (street == nullptr) {
            return false;
        }
    }
    if(GameManager::economy_manager->deductMoney(*player, tile->getBuyPrice())){
        return false;
    }

    tile->setPropertyStatus(MORTGAGED);
    return true;
}

int PropertyManager::getFinalRentPrice(PropertyTile* tile, int diceRoll = 0) const{
    if(diceRoll == 0){
        return tile->calculateRent();
    }
    return tile->calculateRent()*diceRoll;
}

void PropertyManager::startFestival(PropertyTile* tile){
    tile->applyFestival();
}