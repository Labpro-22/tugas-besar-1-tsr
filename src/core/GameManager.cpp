#include "../../include/core/GameManager.hpp"
#include "../../include/views/ViewGame.hpp"
#include <sstream>
#include <iostream>

void GameManager::printBoard(const std::string& args){
    ViewGame v;
    v.displayBoard();
}

void GameManager::printProperty(const std::string& args){
    std::cout<<"kode petak: ";
    std::string kode;
    std::cin>>kode;
    ViewGame::displayPropertyDeed(kode);
}

void GameManager::rollDice(const std::string& args){
    std::random_device rd; 
    std::mt19937 gen(rd()); 

    std::uniform_int_distribution<> distr(1, 6);
    int dice1=distr(gen);
    int dice2=distr(gen);
    auto player=players[current_player_index];
    player->movePlayer(dice1+dice2);;
    ViewGame::displayDiceRollResult(players[current_player_index]->getname(),dice1,dice2,PropertyManager::getBoard().getTile(player->getPosition()).getName());
}

void GameManager::mortgage(const std::string& args){
    ViewGame::displayMortgageList(players[current_player_index]->owned_properties);
    int index=ViewGame::getInt(10);
    bool mortgageTry = property_manager->tryMortgage(players[current_player_index],players[current_player_index]->owned_properties[index]);
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

void GameManager::visitTaxTile(TaxTile* tile, std::shared_ptr<Player> player) {
    bool trying = economy_manager->processTax(player, tile->getTaxType(), tile->getTaxAmount());;
}

// void GameManager::visitFestivalTile(FestivalTile* tile, Player& player) {
//     ViewGame v;
//     std::string input = v.getInput();

//     //Someone help with handling inputs please like parsing it
    
//     //Zek make a player set festival so i could call it

// }

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
    PropertyStatus status = tile->getPropertyStatus();
    if (status == PropertyStatus::BANK) {
        int nak;
        std::cin>>nak;
        if (player.canPay(tile->getBuyPrice())&&nak==1) {
            player.buyProperty(*tile);
        } else{
            economy_manager->startAuction(tile);
        }
    } else if (status == PropertyStatus::OWNED) {
        std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
        if(current_owner && current_owner.get() != &player){
            float rent = tile->calculateRent();
            bool success = economy_manager->transferMoney(player, *current_owner, rent);
        }
    }
    // Jika MORTGAGED, tidak ada aksi
}

void GameManager::visitRailroadTile(RailroadTile* tile, Player& player) {
    PropertyStatus status = tile->getPropertyStatus();
    if (status == PropertyStatus::BANK) {
        int nak;
        if (player.canPay(tile->getBuyPrice())&&nak==1) {
            player.buyProperty(*tile);
        } else{
            economy_manager->startAuction(tile);
        }
    } else if (status == PropertyStatus::OWNED) {
        std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
        if(current_owner && current_owner.get() != &player){
            std::cout<<current_owner->getname();
            float rent = tile->calculateRent();
            bool success = economy_manager->transferMoney(player, *current_owner, rent);
        }
    }
}


void GameManager::visitUtilityTile(UtilityTile* tile, Player& player) {
    PropertyStatus status = tile->getPropertyStatus();
    if (status == PropertyStatus::BANK) {
        int nak;
        std::cin>>nak;
        if (player.canPay(tile->getBuyPrice())&&nak==1) {
            player.buyProperty(*tile);
        } else{
            economy_manager->startAuction(tile);
        }
    } else if (status == PropertyStatus::OWNED) {
        std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
        if(current_owner && current_owner.get() != &player){
            float rent = tile->calculateRent();
            bool success = economy_manager->transferMoney(player, *current_owner, rent);
        }
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
