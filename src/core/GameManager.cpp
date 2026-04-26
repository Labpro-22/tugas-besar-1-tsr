#include "../../include/core/GameManager.hpp"
#include "../../include/views/ViewGame.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
std::unique_ptr<CardManager> GameManager::card_manager = nullptr;
std::unique_ptr<PropertyManager> GameManager::property_manager = nullptr;
std::unique_ptr<EconomyManager> GameManager::economy_manager = nullptr;
std::unique_ptr<TransactionLog> GameManager::logger = nullptr;
std::vector<std::shared_ptr<Player>> GameManager::players = {};
std::unordered_map<std::string, std::function<void(const std::string&)>> GameManager::command_map = {};

GameManager::GameManager(int maxTurns, std::vector<std::shared_ptr<Player>> initialPlayers, 
                std::unique_ptr<CardManager> cMgr, 
                std::unique_ptr<PropertyManager> pMgr, 
                std::unique_ptr<EconomyManager> eMgr,
                                std::unique_ptr<TransactionLog> tLogger):
            current_turn_count(0),
            max_turns(maxTurns),
            current_state(GameState::START_TURN) {
        current_player_index=0;
        players = std::move(initialPlayers);
        card_manager = std::move(cMgr);
        property_manager = std::move(pMgr);
        economy_manager = std::move(eMgr);
        logger = std::move(tLogger);
        command_map["CETAK_PAPAN"] = [this](const std::string& args) { this->printBoard(args); };
        command_map["LEMPAR_DADU"] = [this](const std::string& args) { this->rollDice(args); };
        command_map["ATUR_DADU"] = [this](const std::string& args) { this->setDice(args); };
        command_map["CETAK_AKTA"] = [this](const std::string& args) { this->printCertificate(args); };
        command_map["CETAK_PROPERTI"] = [this](const std::string& args) { this->printProperty(args); };
        command_map["GADAI"] = [this](const std::string& args) { this->mortgage(args); };
        command_map["TEBUS"] = [this](const std::string& args) { this->redeem(args); };
        command_map["BANGUN"] = [this](const std::string& args) { this->build(args); };
        command_map["SIMPAN"] = [this](const std::string& args) { this->save(args); };
        command_map["MUAT"] = [this](const std::string& args) { this->load(args); };
        command_map["CETAK_LOG"] = [this](const std::string& args) { this->printLog(args); };
        command_map["GUNAKAN_KEMAMPUAN"] = [this](const std::string& args) { this->useAbility(args); };
}
void GameManager::startGame(){
    while (current_turn_count <= max_turns) {
        auto current_player = players[current_player_index];
        bool turn_finished = false;
        while (!turn_finished) {
            std::string raw_command = ViewGame::getUserCommand();
            std::stringstream ss(raw_command);
            std::string command;
            ss >> command;
            std::string args;
            std::getline(ss, args);
            if (!args.empty() && args[0] == ' ') {
                args.erase(0, 1);
            }
            auto it = command_map.find(command);
            if (it == command_map.end()) {
                ViewGame::displayMessage("Command tidak dikenal.");
                continue;
            }
            it->second(args);
            if (command == "LEMPAR_DADU" || command == "ATUR_DADU") {
                turn_finished = true;
            }
        }
        current_player->endTurn();
        current_player_index = (current_player_index + 1) % players.size();

        if (current_player_index == 0) {
            current_turn_count++;
        }
    }
}
void GameManager::printBoard(const std::string& args){
    ViewGame::displayBoard();
}

void GameManager::printCertificate(const std::string& args){
    std::cout<<"kode petak: ";
    std::string kode;
    std::cin>>kode;
    ViewGame::displayPropertyDeed(kode);
}
void GameManager::printProperty(const std::string& args){
    ViewGame::displayPlayerProperties(players[current_player_index]->getname());
}

void GameManager::setDice(const std::string& args){ //belum
    std::stringstream ss(args);
    int dice1;
    int dice2;
    if (!(ss >> dice1 >> dice2)) {
        ViewGame::displayMessage("Format ATUR_DADU harus: ATUR_DADU <dadu1> <dadu2>. Contoh: ATUR_DADU 6 7");
        return;
    }
    if (dice1 < 1 || dice1 > 6 || dice2 < 1 || dice2 > 6) {
        ViewGame::displayMessage("Nilai dadu harus di antara 1 sampai 6.");
        return;
    }
    auto player = players[current_player_index];
    player->movePlayer(dice1 + dice2);
    ViewGame::displayManualDiceRollResult(players[current_player_index]->getname(),dice1,dice2,PropertyManager::getBoard().getTile(player->getPosition()).getName());
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
void GameManager::redeem(const std::string& args){
    ViewGame::displayUnmortgageList(players[current_player_index]->owned_properties, players[current_player_index]->getBalance());
    int index=ViewGame::getInt(10);
    auto property=players[current_player_index]->owned_properties[index];
    if(property_manager->tryUnmortgage(players[current_player_index],property)){
        ViewGame::displayUnmortgageSuccess(property->getName(),property->getMortgageValue(),players[current_player_index]->getBalance());
    } else{
        ViewGame::displayUnmortgageFailure(property->getName(),property->getMortgageValue(),players[current_player_index]->getBalance());
    }
}
void GameManager::build(const std::string& args){ //ga yakin, dibantu teman ini
    auto player = players[current_player_index];
    auto& board = PropertyManager::getBoard();
    std::map<std::string, std::vector<StreetTile*>> buildable_groups;
    std::vector<std::string> all_groups = board.getAllGroups();
    for (const std::string& color : all_groups) {
        if (!board.checkMonopoly(*player, color)) {
            continue;
        }
        std::vector<StreetTile*> streets_in_group;
        for (Tile* tile : board.getPropertiesByGroup(color)) {
            StreetTile* street = dynamic_cast<StreetTile*>(tile);
            if (street != nullptr) {
                streets_in_group.push_back(street);
            }
        }
        if (!streets_in_group.empty()) {
            buildable_groups[color] = streets_in_group;
        }
    }
    if (buildable_groups.empty()) {
        ViewGame::displayNoBuildableError();
        return;
    }
    ViewGame::displayBuildableGroups(buildable_groups, static_cast<int>(player->getmoney()));
    std::cout << "Pilih color group (0 untuk batal): ";
    int group_index = ViewGame::getInt(static_cast<int>(buildable_groups.size()));
    if (group_index == 0) {
        return;
    }
    auto group_it = buildable_groups.begin();
    std::advance(group_it, group_index - 1);
    const std::string& selected_group = group_it->first;
    std::vector<StreetTile*>& group_streets = group_it->second;
    int min_level = 100;
    for (StreetTile* street : group_streets) {
        min_level = std::min(min_level, street->getBuildingLevel());
    }
    std::vector<StreetTile*> eligible_streets;
    for (StreetTile* street : group_streets) {
        int level = street->getBuildingLevel();
        if (level == min_level && level < 5) {
            eligible_streets.push_back(street);
        }
    }
    ViewGame::displayBuildableTilesInGroup(selected_group, group_streets, eligible_streets);
    if (eligible_streets.empty()) {
        ViewGame::displayMessage("Tidak ada properti yang dapat dibangun di group ini.");
        return;
    }
    for (size_t i = 0; i < eligible_streets.size(); ++i) {
        std::cout << i + 1 << ". " << eligible_streets[i]->getName() << " (" << eligible_streets[i]->getCode() << ")\n";
    }
    std::cout << "Pilih properti yang ingin dibangun (0 untuk batal): ";
    int street_index = ViewGame::getInt(static_cast<int>(eligible_streets.size()));
    if (street_index == 0) {
        return;
    }
    StreetTile* selected_street = eligible_streets[street_index - 1];
    int previous_level = selected_street->getBuildingLevel();
    int build_cost = (previous_level == 4)
        ? static_cast<int>(selected_street->getHotelPrice())
        : static_cast<int>(selected_street->getHousePrice());

    bool success = (previous_level == 4)
        ? property_manager->tryBuildHotel(player, selected_street)
        : property_manager->tryBuildHouse(player, selected_street);

    if (!success) {
        ViewGame::displayMessage("Pembangunan gagal. Pastikan aturan build terpenuhi dan uang mencukupi.");
        return;
    }

    if (previous_level == 4) {
        ViewGame::displayUpgradeHotelSuccess(selected_street->getName(), build_cost, player->getBalance());
    } else {
        ViewGame::displayBuildSuccess(selected_street->getName(), build_cost, player->getBalance());
    }
}

void GameManager::useAbility(const std::string& args){
    auto player = players[current_player_index];
    const auto& cards = player->getAllskillCard();
    if (cards.empty()) {
        ViewGame::displayMessage("Tidak ada kartu kemampuan yang bisa digunakan.");
        return;
    }
    ViewGame::displaySkillCardList(cards);
    int choice = ViewGame::getInt(cards.size());
    if (choice == 0) {
        return;
    }
    int index = choice - 1;
    SkillCard* selected_card = cards[index].get();
    std::string effect_desc = "Efek kartu spesial aktif.";
    if (selected_card->isMoveCard()) {
        MoveSkillCard* move = static_cast<MoveSkillCard*>(selected_card);
        effect_desc = "Maju " + std::to_string(move->getStep()) + " petak.";
    } else if (selected_card->isDiscountCard()) {
        DiscountSkillCard* discount = static_cast<DiscountSkillCard*>(selected_card);
        effect_desc = "Diskon sewa " + std::to_string(static_cast<int>(discount->getPercentage())) + "% aktif.";
    } else if (selected_card->isShieldCard()) {
        effect_desc = "Shield aktif untuk melindungi dari tagihan/sanksi.";
    } else if (selected_card->isTeleportCard()) {
        effect_desc = "Kamu dapat berpindah ke petak tujuan.";
    } else if (selected_card->isLassoCard()) {
        effect_desc = "Lawan terdekat ditarik ke petakmu.";
    } else if (selected_card->isDemolitionCard()) {
        effect_desc = "Bangunan lawan dapat dihancurkan.";
    }

    player->useSkillCard(index);
    GameManager::card_manager->takeSkillCardFromPlayer(*player,index);
    ViewGame::displaySkillCardActivated(selected_card->getName(), effect_desc);
}






 


//VisitTilesFunctions are below



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

void GameManager::visitTaxTile(TaxTile* tile, std::shared_ptr<Player> player) {
    int amount = 0;

    if (tile->getTaxType() == TaxType::PPH) {
        ViewGame::displayPPH(tile);
        int choice = ViewGame::getInt(2);
        if (choice == 1) {
            amount = tile->getTaxAmount();
        } else {
            amount = player->getTotalAssetValue()*0.1;
        }

    } else if (tile->getTaxType() == TaxType::PPH) {
        ViewGame::displayPBM(tile);
    }

    bool trying = economy_manager->processTax(player, tile->getTaxType(), amount);
    if (trying) {
        std::cout << "Kamu berhasil membayar\n";
    }
}

void GameManager::visitFestivalTile(FestivalTile* tile, Player& player) {
    if (player.owned_properties.empty()) {
        ViewGame::displayMessage("Anda tidak memiliki properti untuk merayakan festival!");
        return;
    }

    PropertyTile* selected_property = nullptr;
    bool finished = false;

    while (!finished) {
        ViewGame::displayFestivalStart(player.owned_properties);
        ViewGame::displayMessage("Masukkan kode properti untuk festival:");
        std::string property_code = ViewGame::getUserInput();

        for (PropertyTile* prop : player.owned_properties) {
            if (prop->getCode() == property_code) {
                selected_property = prop;
                finished = true;
                break;
            }
        }

        if (!finished) {
            ViewGame::displayMessage("Kode properti tidak valid atau bukan milik Anda!");
            ViewGame::displayMessage("Ketik 'CANCEL' untuk keluar atau coba lagi.");
            if (property_code == "CANCEL") return;
        }
    }

    if (selected_property) {
        property_manager->startFestival(selected_property); 
        ViewGame::displayMessage("Efek festival diaktifkan di " + selected_property->getName() + "!");
    }
}
void GameManager::visitGoTile(GoTile* tile, Player& player) {
    ViewGame::displayMessage("Kamu mengunjungi Petak Go\n");
    economy_manager->addMoney(player, tile->getReward());
}

void GameManager::visitGoToJailTile(GoToJailTile* tile, Player& player) {
    ViewGame::displayMessage("Kamu mengunjungi Petak Masuk ke Penjara, kamu DIPENJARA!\n");
    Board& board = property_manager->getBoard();
    int pen_index = 0;
    for (size_t i = 0; i < board.getSize(); ++i) {
        Tile& tile = board.getTile(i);
        if (tile.getCode() == "PEN") {
            pen_index = i;
            break; 
        }
    }
    player.setInJail();
    player.setPosition(pen_index);
}

void GameManager::visitFreeParkingTile(FreeParkingTile* tile, Player& player) {
    ViewGame::displayMessage("Kamu mengunjungi Petak FreeParking\n");
}

void GameManager::visitJailTile(JailTile* tile, Player& player) {
    ViewGame::displayMessage("Kamu mengunjungi petak Penjara\n");
}

void GameManager::visitStreetTile(StreetTile* tile, Player& player) {
    PropertyStatus status = tile->getPropertyStatus();
    if (status == PropertyStatus::BANK) {
        ViewGame::displayBuyPromptStreet(tile->getName(), tile->getCode(), tile->getColor(), tile->getBuyPrice(), tile->getRentList(), player.getBalance());
        ViewGame::displayMessage("Apakah kamu ingin membeli properti ini seharga M400? (y/n):");
        bool nak = ViewGame::getYesNo();
        if (player.canPay(tile->getBuyPrice()) && nak) {
            std::cout << tile->getName() << "kini menjadi milikmu!" << "\n" << "Uang tersisa: M1." << player.getBalance() << "\n"; 
            player.buyProperty(*tile);
        } else{
            ViewGame::displayMessage("Properti ini akan masuk ke sistem lelang...");
            economy_manager->startAuction(tile);
        }
    } else if (status == PropertyStatus::OWNED) {
        std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
        if(current_owner && current_owner.get() != &player){
            float rent = tile->calculateRent();
            bool success = economy_manager->transferMoney(player, *current_owner, rent);
        }
    } else if (status == PropertyStatus::MORTGAGED) {
        ViewGame::displayMortgagedRent(*tile, player);
    }
}

void GameManager::visitRailroadTile(RailroadTile* tile, Player& player) {
    PropertyStatus status = tile->getPropertyStatus();
    if (status == PropertyStatus::BANK) {
        ViewGame::displayBuyAutoSuccess("Stasiun", tile->getName());
        player.addProperty(tile);
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
        ViewGame::displayBuyAutoSuccess("Stasiun", tile->getName());
        player.addProperty(tile);
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

    //<STATE_PROPERTI>
    out << property_manager->toSaveFormat();
    //<STATE_DECK>
    out << card_manager->toSaveFormat();
    //<STATE_LOG>


    return out.str();
}
int GameManager::getCurrentTurn(){
    return current_player_index;
}