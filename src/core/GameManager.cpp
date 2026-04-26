#include "../../include/core/GameManager.hpp"
#include <sstream>
#include <iostream>



std::unique_ptr<CardManager> GameManager::card_manager=nullptr;
std::unique_ptr<PropertyManager> GameManager::property_manager=nullptr;
std::unique_ptr<EconomyManager> GameManager::economy_manager=nullptr;
std::unique_ptr<TransactionLog> GameManager::logger=nullptr;
std::vector<std::shared_ptr<Player>> GameManager::players={};

void GameManager::visitCardTile(CardTile* tile, Player& player) {
    if(tile->getType() == CHANCE){
        card_manager->drawKesempatan(player);
    }
    else{
        card_manager->drawDanaUmum(player);
    }
}

void GameManager::visitTaxTile(TaxTile* tile, Player& player) {
    economy_manager->deductMoney(player, tile->getTaxAmount());
}

void GameManager::visitFestivalTile(FestivalTile* tile, Player& player) {
    //idk right now
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
}


void GameManager::visitUtilityTile(UtilityTile* tile, Player& player) {
    std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
    int rent = tile->calculateRent();
    if(current_owner && current_owner.get() != &player){
        bool success = economy_manager->transferMoney(player, *current_owner, rent);
    }
}


std::string GameManager::toSaveFormat() const{
    std::ostringstream out;
    // <TURN_SAAT_INI> <MAX_TURN>
    out << current_turn_count << " " << max_turns << "\n";
    // <JUMLAH_PEMAIN>
    out << players.size();

    //<STATE_PEMAIN_1..N>
    for(const std::shared_ptr<Player>& p:players){
        out << p->toSaveFormat();
    }
    //<URUTAN_GILIRAN_1> <URUTAN_GILIRAN_2> … <URUTAN_GILIRAN_N>
    for(int i = 0; i<current_turn_count; i++){
        if(i!=current_turn_count-1) out << players[i]->getname() << " ";
        else out << players[i]->getname() << "\n";
    }
    //<GILIRAN_AKTIF_SAAT_INI>
    out << players[players.size()%current_turn_count]->getname() << "\n";

    return out.str();
}
