#include "../include/utils/IOManager.hpp"
#include "../include/utils/ConfigData.hpp"
#include "../include/core/GameManager.hpp"
#include <memory>
#include <vector>
#include <iostream>
// untuk test doang 
int main(){
    
    GameSaveData test=IOManager::loadGameData("test.txt");

    std::cout << "curr_turn: " << std::to_string(test.current_turn) << std::endl;
    std::cout << "max_turn: " << std::to_string(test.max_turn) << std::endl;
    std::cout << "player_count: " << std::to_string(test.player_count) << std::endl << std::endl;

    for(PlayerSaveData player : test.players){
        std::cout << "uname: " << player.username << std::endl;
        std::cout << "balance: " << std::to_string(player.balance) << std::endl;
        std::cout << "pos: " << player.position_code << std::endl;
        std::cout << "status: " << player.status << std::endl;
        for(CardSaveData card : player.hand_cards){
            std::cout << "    card_type: " << card.card_type << std::endl;
            std::cout << "    value: " << card.value << std::endl;
            std::cout << "    duration_left: " << card.remaining_duration << std::endl;
        }
        std::cout << std::endl;
    }

    for(std::string name : test.turn_order){
        std::cout << name << " ";
    }
    std::cout << std::endl << "current_turn: " << test.current_active_player << std::endl <<std::endl;

    for(PropertySaveData property : test.properties){
        std::cout << property.tile_code << " " << property.type << " " << property.owner_name << " " << property.status << " " << property.festival_multiplier << " " << property.festival_duration << " " << property.build_level << std::endl;
    }
    std::cout << std::endl;
    for(std::string card : test.skill_card_deck){
        std::cout << card << std::endl;
    }
    std::cout << std::endl;
    for(LogSaveData log : test.logs){
        std::cout << log.turn << " " << log.username << " " << log.action_type << " " << log.detail << std::endl;
    }
    
    // std::vector<std::shared_ptr<Player>> players;
    // players.push_back(std::make_shared<Player>("Pemain1", 1500, 0, PlayerState::FREE));
    // players.push_back(std::make_shared<Player>("Pemain2", 1500, 0, PlayerState::FREE));
    //     players.push_back(std::make_shared<Player>("Pemain3", 1500, 0, PlayerState::FREE));
    // players[0]->addSkillCard(std::make_unique<MoveSkillCard>("move"));
    //     players[0]->addSkillCard(std::make_unique<DiscountSkillCard>("discount"));
    // players[0]->addSkillCard(std::make_unique<ShieldSkillCard>("shield"));
    // players[0]->addSkillCard(std::make_unique<TeleportSkillCard>("teleport"));
    //     players[0]->addSkillCard(std::make_unique<LassoSkillCard>("lasso"));



    // auto game = std::make_unique<GameManager>(
    //     100,
    //     std::move(players),
    //     std::make_unique<CardManager>(),
    //     std::make_unique<PropertyManager>(nullptr),
    //     std::make_unique<EconomyManager>(),
    //     std::make_unique<TransactionLog>()
    // );

    // game->startGame();
    return 0;
}
