#include "../../include/core/GameManager.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_set>



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

GameManager::GameManager(
int maxTurns,
std::vector<std::shared_ptr<Player>> initialPlayers,
std::unique_ptr<CardManager> cMgr,
std::unique_ptr<PropertyManager> pMgr,
std::unique_ptr<EconomyManager> eMgr,
std::unique_ptr<TransactionLog> tLogger
)
: current_player_index(0),
current_turn_count(1),
max_turns(maxTurns),
current_state(GameState::START_TURN) {
players = std::move(initialPlayers);
card_manager = std::move(cMgr);
property_manager = std::move(pMgr);
economy_manager = std::move(eMgr);
logger = std::move(tLogger);
}

void GameManager::startGame() {
// Minimal runtime loop placeholder so program can run
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

void GameManager::loadConfig(){
    std::string configPath= "./config";
    std::ifstream sanityCheck(configPath + "/property.txt");
    if (!sanityCheck.good()) {
        configPath = "./config";
    }
    FullConfigData config = IOManager::loadAllConfigs(configPath);

    max_turns = config.max_turn;
    const float initialBalance = static_cast<float>(config.initial_balance);

    for (auto& p : players) {
        const float current = p->getmoney();
        if (current < initialBalance) {
            *p += (initialBalance - current);
        } else if (current > initialBalance) {
            *p -= (current - initialBalance);
        }
    }

    if (property_manager) {
        property_manager->InitializeBoard(std::move(config));
    }
}

void GameManager::loadSaveState(std::string filename){
    GameSaveData data = IOManager::loadGameData(filename);

    current_turn_count = data.current_turn;
    max_turns = data.max_turn;
    current_player_index = 0;

    if (property_manager) {
        std::string configPath = "./config";
        std::ifstream sanityCheck(configPath + "/property.txt");
        if (!sanityCheck.good()) {
            configPath = "./config";
        }
        FullConfigData config = IOManager::loadAllConfigs(configPath);
        property_manager->InitializeBoard(std::move(config));
    }

    auto resolvePositionIndex = [this](const std::string& positionCode) -> int {
        if (positionCode.empty()) {
            return 0;
        }

        try {
            size_t parsedLength = 0;
            int numericPosition = std::stoi(positionCode, &parsedLength);
            if (parsedLength == positionCode.size()) {
                return std::max(0, numericPosition);
            }
        } catch (...) {
        }

        if (property_manager) {
            Board& board = PropertyManager::getBoard();
            const int boardSize = board.getSize();
            for (int i = 0; i < boardSize; ++i) {
                if (board.getTile(i).getCode() == positionCode) {
                    return i;
                }
            }
        }

        return 0;
    };

    players.clear();
    for (const auto& savedPlayer : data.players) {
        PlayerState state = PlayerState::FREE;
        if (savedPlayer.status == "INJAIL") {
            state = PlayerState::INJAIL;
        } else if (savedPlayer.status == "BANKRUPT") {
            state = PlayerState::BANKCRUPT;
        }

        auto player = std::make_shared<Player>(
            savedPlayer.username,
            savedPlayer.balance,
            resolvePositionIndex(savedPlayer.position_code),
            state
        );
        players.push_back(player);
    }

    if (!data.turn_order.empty()) {
        std::unordered_map<std::string, std::shared_ptr<Player>> playersByName;
        for (const auto& player : players) {
            playersByName[player->getname()] = player;
        }

        std::vector<std::shared_ptr<Player>> orderedPlayers;
        orderedPlayers.reserve(players.size());
        std::unordered_set<std::string> alreadyAdded;

        for (const auto& username : data.turn_order) {
            auto it = playersByName.find(username);
            if (it != playersByName.end() && alreadyAdded.insert(username).second) {
                orderedPlayers.push_back(it->second);
            }
        }

        for (const auto& player : players) {
            if (alreadyAdded.insert(player->getname()).second) {
                orderedPlayers.push_back(player);
            }
        }

        if (orderedPlayers.size() == players.size()) {
            players = std::move(orderedPlayers);
        }
    }

    if (!players.empty() && !data.current_active_player.empty()) {
        auto activeIt = std::find_if(players.begin(), players.end(), [&data](const std::shared_ptr<Player>& player) {
            return player && player->getname() == data.current_active_player;
        });
        if (activeIt != players.end()) {
            current_player_index = static_cast<int>(std::distance(players.begin(), activeIt));
        }
    }

    if (property_manager) {
        property_manager->loadBoardState(data.properties);
    }

    if (card_manager) {
        card_manager->loadCardState(data);
    }

    if (logger) {
        logger->loadLogState(data.logs);
    }
}
