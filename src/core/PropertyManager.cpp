#include "../../include/core/PropertyManager.hpp"
#include "../../include/core/TransactionLog.hpp"
#include "../../include/core/GameManager.hpp"
#include "../../include/views/ViewGame.hpp"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <sstream>

std::unique_ptr<Board> PropertyManager::board = nullptr;

PropertyManager::PropertyManager(std::unique_ptr<Board> gameBoard) {
board = std::move(gameBoard);
}

Board& PropertyManager::getBoard() {
if (!board) {
board = std::make_unique<Board>();
}
return *board;
}

Tile& PropertyManager::getTileAt(int position) const{
    return board->getTile(position);
}

void PropertyManager::assignOwnership(PropertyTile* tile, std::shared_ptr<Player> newOwner){
    tile->setPropertyOwner(newOwner);
    tile->setPropertyStatus(OWNED);
}


bool PropertyManager::sellAllBuildingsOnColorGroup(std::shared_ptr<Player> player, const std::string& color) {
    if (!player || !board) {
        return false;
    }

    const std::vector<Tile*>& sameColorTiles = board->getPropertiesByGroup(color);

    for (Tile* groupedTile : sameColorTiles) {
        StreetTile* groupedStreet = dynamic_cast<StreetTile*>(groupedTile);
        if (groupedStreet == nullptr) {
            return false;
        }

        std::shared_ptr<Player> owner = groupedStreet->getPropertyOwner().lock();
        if (!owner || owner.get() != player.get()) {
            return false;
        }

        while (groupedStreet->canSellBuilding()) {
            const float sellPrice = (groupedStreet->getBuildingLevel() == 5)
                ? groupedStreet->getHotelPrice() / 2.0f
                : groupedStreet->getHousePrice() / 2.0f;
            GameManager::economy_manager->addMoney(*player, sellPrice);
            groupedStreet->sellBuilding();
        }
    }

    return true;
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
        const std::vector<Tile*>& sameColorTiles = board->getPropertiesByGroup(tile->getColor());
        bool hasBuildingsInGroup = false;

        for (Tile* groupedTile : sameColorTiles) {
            StreetTile* groupedStreet = dynamic_cast<StreetTile*>(groupedTile);
            if (groupedStreet == nullptr) {
                return false;
            }

            if (groupedStreet->canSellBuilding()) {
                hasBuildingsInGroup = true;
                break;
            }
        }

        if (hasBuildingsInGroup) {
            std::cout << "DO YOU WANT TO SELL ALL BUILDINGS IN COLOR GROUP: ";
            if (!ViewGame::getYesNo()) {
                return false;
            }

            if (!sellAllBuildingsOnColorGroup(player, tile->getColor())) {
                return false;
            }
        }
    }
    std::cout << "DO YOU STILL WANT TO MORTGAGE"; 
    if(ViewGame::getYesNo()){
        tile->setPropertyStatus(MORTGAGED);
        GameManager::economy_manager->addMoney(*player, tile->getMortgageValue());
        return true;
    }
    else return false;
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
    if(!GameManager::economy_manager->deductMoney(*player, tile->getBuyPrice())){
        return false;
    }

    tile->setPropertyStatus(MORTGAGED);
    return true;
}

float PropertyManager::getFinalRentPrice(PropertyTile* tile, int diceRoll) const{
    if(diceRoll == 0){
        return tile->calculateRent();
    }
    return tile->calculateRent()*diceRoll;
}

void PropertyManager::startFestival(PropertyTile* tile){
    tile->applyFestival();
}

std::vector<PropertyTile*> PropertyManager::findPropertiesOwnedByPlayer(std::shared_ptr<Player> player) const {
    std::vector<PropertyTile*> ownedProperties;

    if (!player || !board) {
        return ownedProperties;
    }

    const int boardSize = board->getSize();
    for (int i = 0; i < boardSize; ++i) {
        Tile& currentTile = board->getTile(i);
        PropertyTile* property = dynamic_cast<PropertyTile*>(&currentTile);
        if (property == nullptr) {
            continue;
        }

        std::shared_ptr<Player> owner = property->getPropertyOwner().lock();
        if (owner && owner.get() == player.get()) {
            ownedProperties.push_back(property);
        }
    }

    return ownedProperties;
}

void PropertyManager::doMortgage(std::shared_ptr<Player> player){
    std::vector<PropertyTile*> owned = findPropertiesOwnedByPlayer(player);

    ViewGame v;
    std::string input = v.getUserInput();

    if(owned.empty()) return;
    
    auto p = std::find_if(owned.begin(), owned.end(), [input](PropertyTile* n){
        return n->getCode() == input;
    });

    if(p != owned.end()){
        if(tryMortgage(player, *p)){
            std::cout << "it succeed";
        }
        else{
            throw "it failed";
        }
    }
    throw "you dont have any property";
}

void PropertyManager::doUnmortgage(std::shared_ptr<Player> player){
    std::vector<PropertyTile*> owned = findPropertiesOwnedByPlayer(player);

    ViewGame v;
    std::string input = v.getUserInput();

    if(owned.empty()) return;
    
    auto p = std::find_if(owned.begin(), owned.end(), [input](PropertyTile* n){
        return n->getCode() == input;
    });

    if(p != owned.end()){
        if(tryUnmortgage(player, *p)){
            std::cout << "you got your property";
        }
        else{
            throw "its still mortgage";
        }
    }
    throw "you dont have any property";
}

void PropertyManager::InitializeBoard(FullConfigData&& config){
    board = std::make_unique<Board>();

    std::vector<std::unique_ptr<Tile>> allTiles;
    allTiles.reserve(config.streets.size() + config.action_tiles.size());

    for (auto& t : config.streets) {
        allTiles.push_back(std::move(t));
    }
    for (auto& t : config.action_tiles) {
        allTiles.push_back(std::move(t));
    }

    std::sort(allTiles.begin(), allTiles.end(), [](const std::unique_ptr<Tile>& a, const std::unique_ptr<Tile>& b) {
        return a->getIndex() < b->getIndex();
    });

    for (auto& t : allTiles) {
        board->addTile(std::move(t));
    }

    railroad_rent_map = std::move(config.railroad_rent_map);
    utility_multiplier_map = std::move(config.utility_multiplier_map);
    RailroadTile::setMult(railroad_rent_map);
    UtilityTile::setMult(utility_multiplier_map);
}

void PropertyManager::loadBoardState(std::vector<PropertySaveData> data){
    if (!board) {
        return;
    }

    for (auto& player : GameManager::players) {
        player->owned_properties.clear();
    }

    std::unordered_map<std::string, std::shared_ptr<Player>> playersByName;
    for (const auto& player : GameManager::players) {
        playersByName[player->getname()] = player;
    }

    for (const auto& savedProperty : data) {
        PropertyTile* property = nullptr;
        for (int i = 0; i < board->getSize(); ++i) {
            Tile& tile = board->getTile(i);
            if (tile.getCode() == savedProperty.tile_code) {
                property = dynamic_cast<PropertyTile*>(&tile);
                break;
            }
        }

        if (!property) {
            continue;
        }

        property->setFestivalState(savedProperty.festival_multiplier, savedProperty.festival_duration);

        if (savedProperty.status == "OWNED" || savedProperty.status == "MORTGAGED") {
            auto ownerIt = playersByName.find(savedProperty.owner_name);
            if (ownerIt != playersByName.end() && ownerIt->second) {
                property->setPropertyOwner(ownerIt->second);
                ownerIt->second->addProperty(property);
            } else {
                property->setPropertyOwner(nullptr);
            }
        } else {
            property->setPropertyOwner(nullptr);
        }

        if (savedProperty.status == "BANK") {
            property->setPropertyStatus(BANK);
        } else if (savedProperty.status == "OWNED") {
            property->setPropertyStatus(OWNED);
        } else if (savedProperty.status == "MORTGAGED") {
            property->setPropertyStatus(MORTGAGED);
        }

        if (auto* street = dynamic_cast<StreetTile*>(property)) {
            const int desiredLevel = std::max(0, savedProperty.build_level);
            while (street->getBuildingLevel() < desiredLevel && street->canBuildHouse()) {
                street->buildHouse();
            }
            while (street->getBuildingLevel() < desiredLevel && street->canBuildHotel()) {
                street->buildHotel();
            }
            while (street->getBuildingLevel() > desiredLevel && street->canSellBuilding()) {
                street->sellBuilding();
            }
        }
    }
}

std::string PropertyManager::toSaveFormat() const {
    std::ostringstream out;
    std::vector<PropertyTile*> properties;

    //cari <JUMLAH_PROPERTI>
    int boardSize = board->getSize();
    for (int i = 0; i < boardSize; ++i) {
        Tile& tile = board->getTile(i);
        PropertyTile* prop = dynamic_cast<PropertyTile*>(&tile);
        if (prop != nullptr) {
            properties.push_back(prop);
        }
    }

    //<JUMLAH_PROPERTI>
    out << properties.size() << "\n";

    for (PropertyTile* prop : properties) {
        
        // <KODE_PETAK>
        out << prop->getCode() << " ";

        // <JENIS>
        PropertyType type = prop->getPropertyType();
        if (type == PropertyType::STREET) out << "street ";
        else if (type == PropertyType::RAILROAD) out << "railroad ";
        else if (type == PropertyType::UTILITY) out << "utility ";

        // <PEMILIK>
        std::shared_ptr<Player> owner = prop->getPropertyOwner().lock();
        if (owner) {
            out << owner->getname() << " ";
        } else {
            out << "BANK ";
        }

        // <STATUS>
        PropertyStatus status = prop->getPropertyStatus();
        if (status == BANK) out << "BANK ";
        else if (status == OWNED) out << "OWNED ";
        else if (status == MORTGAGED) out << "MORTGAGED ";

        // <FMULT> and <FDUR>
        int dur = prop->getFestivalTurnsLeft();
        int level = prop->getFestivalLevel();
        
        // If duration is 0, festival is inactive, so multiplier is 1. 
        // Otherwise,bitshift multiplier (1 << level)
        int fmult = (dur > 0) ? (1 << level) : 1; 
        
        out << fmult << " " << dur << " ";

        // <N_BANGUNAN>
        if (type == PropertyType::STREET) {
            StreetTile* street = dynamic_cast<StreetTile*>(prop);
            int bLevel = street->getBuildingLevel();
            
            // 5 is Hotel, 0-4 is houses
            if (bLevel == 5) {
                out << "H\n";
            } else {
                out << bLevel << "\n";
            }
        } else {
            out << "0\n"; 
        }
    }

    return out.str();
}