#include "../../include/utils/IOManager.hpp"
#include <algorithm>

GameSaveData IOManager::loadGameData(const std::string& filepath){
    GameSaveData res;
    
    std::ifstream file("./data/"+filepath);
    std::string line;

    // header
    std::getline(file,line);
    std::istringstream header1(line);
    header1 >> res.current_turn >> res.max_turn;
    std::getline(file,line);
    std::istringstream header2(line);
    header2 >> res.player_count;
    
    // players
    std::string username, pos, status;
    int balance;
    for(int i=0;i<res.player_count;i++){
        std::getline(file,line);
        std::istringstream playerLine(line);
        PlayerSaveData player;
        int cardCount;
        playerLine >> player.username >> player.balance >> player.position_code >> player.status;
        std::getline(file,line);
        std::istringstream playerCardsCount(line);
        if(playerCardsCount >> cardCount){
            if(cardCount>0){
                for(int j=0;j<cardCount;j++){
                    std::getline(file,line);
                    std::istringstream playerCards(line);
                    CardSaveData card;
                    if(playerCards >> card.card_type >> card.value >> card.remaining_duration){
                        player.hand_cards.push_back(card);
                    }
                }
            }
        }
        res.players.push_back(player);
    }

    // turns
    std::getline(file,line);
    std::istringstream turnOrder(line);
    std::string name;
    while(turnOrder>>name){
        res.turn_order.push_back(name);
    }
    std::getline(file,line);
    std::istringstream curr_player(line);
    curr_player >> res.current_active_player;

    // property
    std::getline(file,line);
    std::istringstream propertyCount(line);
    int property_count;
    propertyCount >> property_count;
    for(int i=0;i<property_count;i++){
        std::getline(file,line);
        std::istringstream propertyLine(line);
        PropertySaveData property;
        propertyLine >> property.tile_code >> property.type >> property.owner_name >> property.status >> property.festival_multiplier >> property.festival_duration >> property.build_level;
        res.properties.push_back(property);
    }

    // deck
    std::getline(file,line);
    std::istringstream cardDeckCount(line);
    int card_deck_count;
    cardDeckCount >> card_deck_count;
    for(int i=0;i<card_deck_count;i++){
        std::getline(file,line);
        std::istringstream deckLine(line);
        std::string card;
        deckLine >> card;
        res.skill_card_deck.push_back(card);        
    }

    // logs
    std::getline(file,line);
    std::istringstream logsCount(line);
    int logs_count;
    logsCount >> logs_count;
    for(int i=0;i<logs_count;i++){
        std::getline(file,line);
        std::istringstream logsLine(line);
        LogSaveData log;
        logsLine >> log.turn >> log.username >> log.action_type;
        std::getline(logsLine >> std::ws, log.detail);
        res.logs.push_back(log);
    }

    return res;
}

FullConfigData IOManager::loadAllConfigs(const std::string& configDirectoryPath){
    IOManager manager;
    std::vector<std::unique_ptr<Tile>> allTiles;
    std::vector<std::unique_ptr<Tile>> Properties = manager.IOparsePropertyTileConfig(configDirectoryPath+"/property.txt");
    std::vector<int> taxes = manager.IOparseTaxConfig(configDirectoryPath+"/tax.txt");
    std::vector<int> special = manager.IOparseSpecialConfig(configDirectoryPath+"/special.txt");
    std::vector<std::unique_ptr<Tile>> ActionTile = manager.IOparseActionTileConfig(configDirectoryPath+"/aksi.txt",taxes,special);
    std::vector<int> misc = manager.IOparseMiscConfig(configDirectoryPath+"/misc.txt");
    std::map<int,int> RailroadSewa = manager.IOparseRailroadConfig(configDirectoryPath+"/railroad.txt");
    std::map<int,int> UtilityMult = manager.IOparseUtilityConfig(configDirectoryPath+"/utility.txt");

    return FullConfigData(std::move(Properties),std::move(ActionTile),RailroadSewa,UtilityMult,taxes.at(0),taxes.at(1),taxes.at(2),special.at(0),special.at(1),misc.at(0),misc.at(1));
}

std::vector<std::unique_ptr<Tile>> IOManager::IOparsePropertyTileConfig(const std::string& filepath){
    std::ifstream file(filepath);
    std::string line;

    int ID, HARGA_LAHAN, NILAI_GADAI, UPG_RUMAH, UPG_HT, RENT_L0, RENT_L1, RENT_L2, RENT_L3, RENT_L4, RENT_L5;
    std::string KODE, NAMA, JENIS, WARNA;
    std::vector<std::unique_ptr<Tile>> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> ID >> KODE >> NAMA >> JENIS){
            if(JENIS=="STREET"){
                if(iss >> WARNA >> HARGA_LAHAN >> NILAI_GADAI >> UPG_RUMAH >> UPG_HT >> RENT_L0 >> RENT_L1 >> RENT_L2 >> RENT_L3 >> RENT_L4 >> RENT_L5){
                    std::vector<int> rent;
                    rent.push_back(RENT_L0);
                    rent.push_back(RENT_L1);
                    rent.push_back(RENT_L2);
                    rent.push_back(RENT_L3);
                    rent.push_back(RENT_L4);
                    rent.push_back(RENT_L5);
                    temp.push_back(std::make_unique<StreetTile>(ID, NAMA, KODE, WARNA, HARGA_LAHAN, NILAI_GADAI, nullptr, 0, 0, BANK, 0, rent, UPG_RUMAH, UPG_HT));
                }
            }
            else if(JENIS=="RAILROAD"){
                if(iss >> WARNA >> HARGA_LAHAN >> NILAI_GADAI){
                    temp.push_back(std::make_unique<RailroadTile>(ID, NAMA, KODE, WARNA, HARGA_LAHAN, NILAI_GADAI, nullptr, 0, 0, BANK));
                }
            }
            else if(JENIS=="UTILITY"){
                if(iss >> WARNA >> HARGA_LAHAN >> NILAI_GADAI){
                    temp.push_back(std::make_unique<UtilityTile>(ID, NAMA, KODE, WARNA, HARGA_LAHAN, NILAI_GADAI, nullptr, 0, 0, BANK));
                }
            }
        }
    }
    return temp;
}
std::vector<int> IOManager::IOparseTaxConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int PPHFLAT, PPHPERCENT, PBM;
    std::vector<int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> PPHFLAT >> PPHPERCENT >> PBM){
            temp.push_back(PPHFLAT);
            temp.push_back(PPHPERCENT);
            temp.push_back(PBM);
        }
    }
    return temp;
}
std::vector<int> IOManager::IOparseSpecialConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int GO, JAIL;
    std::vector<int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> GO >> JAIL){
            temp.push_back(GO);
            temp.push_back(JAIL);
        }
    }
    return temp;
}
std::vector<std::unique_ptr<Tile>> IOManager::IOparseActionTileConfig(const std::string& configDirectoryPath, std::vector<int>& tax, std::vector<int>& special){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int ID;
    std::string KODE, NAMA, JENIS, WARNA;
    std::vector<std::unique_ptr<Tile>> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> ID >> KODE >> NAMA >> JENIS >> WARNA){
            if(JENIS=="SPESIAL"){
                if(NAMA=="Petak_Mulai"){
                    temp.push_back(std::make_unique<GoTile>(ID, NAMA, KODE, WARNA, special.at(0)));
                }
                else if(NAMA=="Penjara"){
                    temp.push_back(std::make_unique<JailTile>(ID, NAMA, KODE, WARNA, special.at(1)));
                }
                else if(NAMA=="Petak_Pergi_ke_Penjara"){
                    temp.push_back(std::make_unique<GoToJailTile>(ID, NAMA, KODE, WARNA));
                }
                else if(NAMA=="Bebas_Parkir"){
                    temp.push_back(std::make_unique<FreeParkingTile>(ID, NAMA, KODE, WARNA));
                }
            }
            else if(JENIS=="KARTU"){
                if(NAMA=="Kesempatan"){
                    temp.push_back(std::make_unique<CardTile>(ID, NAMA, KODE, WARNA, CHANCE));
                }
                else if(NAMA=="Dana_Umum"){
                    temp.push_back(std::make_unique<CardTile>(ID, NAMA, KODE, WARNA, COMMUNITY_CHEST));
                }
            }
            else if(JENIS=="PAJAK"){
                if(NAMA=="Pajak_Penghasilan"){
                    temp.push_back(std::make_unique<TaxTile>(ID, NAMA, KODE, WARNA, PPH, tax.at(0)));
                }
                else if(NAMA=="Pajak_Barang_Mewah"){
                    temp.push_back(std::make_unique<TaxTile>(ID, NAMA, KODE, WARNA, PBM, tax.at(2)));
                }
            }
            else if(JENIS=="FESTIVAL"){
                temp.push_back(std::make_unique<FestivalTile>(ID, NAMA, KODE, WARNA));
            }
        }
    }
    return temp;
}

std::vector<int> IOManager::IOparseMiscConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int MAX_TURN, SALDO_AWAL;
    std::vector<int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> MAX_TURN >> SALDO_AWAL){
            temp.push_back(MAX_TURN);
            temp.push_back(SALDO_AWAL);
        }
    }
    return temp;
}
std::map<int,int> IOManager::IOparseRailroadConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int JUMLAH_RAILROAD, BIAYA_SEWA;
    std::map<int,int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> JUMLAH_RAILROAD >> BIAYA_SEWA){
            temp.insert({JUMLAH_RAILROAD,BIAYA_SEWA});
        }
    }
    return temp;
}
std::map<int,int> IOManager::IOparseUtilityConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int JUMLAH_UTILITY, FAKTOR_PENGALI;
    std::map<int,int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> JUMLAH_UTILITY >> FAKTOR_PENGALI){
            temp.insert({JUMLAH_UTILITY,FAKTOR_PENGALI});
        }
    }
    return temp;
}