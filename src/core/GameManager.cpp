#include "../../include/core/GameManager.hpp"
#include "../../include/views/ViewGame.hpp"
#include <sstream>
#include <iostream>

void GameManager::printBoard(const std::string& args){
    ViewGame v;
    v.renderBoard(property_manager->getBoard());
}

void GameManager::printProperty(const std::string& args){
    
}

int rollDice(const std::string& args){
    std::random_device rd; 
    std::mt19937 gen(rd()); 

    std::uniform_int_distribution<> distr(1, 6);

    return distr(gen) + distr(gen);
}

void GameManager::mortgage(const std::string& args){
    bool mortgageTry = property_manager->tryMortgage();
}





 


//VisitTilesFunctions are below





void GameManager::visitCardTile(CardTile* tile, Player& player) {
    if(tile->getType() == CHANCE){
        card_manager->drawKesempatan(player);
    }
    else{
        card_manager->drawDanaUmum(player);
    }
}

void GameManager::visitTaxTile(TaxTile* tile, Player& player) {
    bool trying = economy_manager->processTax(player, tile->getTaxType(), tile->getTaxAmount(), logger);;
}

void GameManager::visitFestivalTile(FestivalTile* tile, Player& player) {
    ViewGame v;
    std::string input = v.getInput();

    //Someone help with handling inputs please like parsing it
    
}

void GameManager::visitGoTile(GoTile* tile, Player& player) {
    economy_manager->addMoney(player, tile->getReward());
}

void GameManager::visitGoToJailTile(GoToJailTile* tile, Player& player) {
    player.setInJail();
}

void GameManager::visitFreeParkingTile(FreeParkingTile* tile, Player& player) {
    //do nothing
}

void GameManager::visitJailTile(JailTile* tile, Player& player) {
    //do nothing
}

void GameManager::visitStreetTile(StreetTile* tile, Player& player) {
    std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
    int rent = tile->calculateRent();
    if(current_owner && current_owner.get() != &player){
        bool success = economy_manager->transferMoney(player, *current_owner, rent);
    }
}

void GameManager::visitRailroadTile(RailroadTile* tile, Player& player) {
    std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
    int rent = tile->calculateRent();
    if(current_owner && current_owner.get() != &player){
        bool success = economy_manager->transferMoney(player, *current_owner, rent);
    }
    else{
        player.buyProperty(*tile);
    }
}


void GameManager::visitUtilityTile(UtilityTile* tile, Player& player) {
    std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
    int rent = tile->calculateRent();
    if(current_owner && current_owner.get() != &player){
        bool success = economy_manager->transferMoney(player, *current_owner, rent);
    }
    else{
        player.buyProperty(*tile);
    }
}


std::string GameManager::toSaveFormat() const{
    std::ostringstream out;
    out << current_turn_count << " " << max_turns << "\n";
    out << players.size();
    for(const std::shared_ptr<Player>& p:players){
        out << p->toSaveFormat();
    }
    return out.str();
}
