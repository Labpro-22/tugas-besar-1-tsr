#pragma once

#include <memory>
#include <map>
#include <string>
#include <vector>
#include "../../include/models/Board.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/models/StreetTile.hpp"
#include "../../include/models/Player.hpp"
#include "../utils/ConfigData.hpp"
#include "../utils/SaveData.hpp"

class PropertyManager:public Saveable {
private:
    static std::unique_ptr<Board> board;
    std::map<int, int> railroad_rent_map;
    std::map<int, int> utility_multiplier_map;
    bool sellAllBuildingsOnColorGroup(std::shared_ptr<Player> player, const std::string& color);

public:
    std::string toSaveFormat() const override;
    PropertyManager(std::unique_ptr<Board> gameBoard);
    ~PropertyManager() = default;
    static Board& getBoard();
    Tile& getTileAt(int position) const;

    // Mengurus pergantian status dan pointer owner saat dibeli/dilelang
    void assignOwnership(PropertyTile* tile, std::shared_ptr<Player> newOwner);

    // Mengecek: 1. Apakah Monopoli? 2. Apakah pemerataan bangunan (even build) terpenuhi? Jika valid, panggil tile->buildHouse()
    bool tryBuildHouse(std::shared_ptr<Player> player, StreetTile* tile);
    
    // Mengecek: 1. Apakah sudah 4 rumah? 2. Apakah pemerataan hotel terpenuhi?
    bool tryBuildHotel(std::shared_ptr<Player> player, StreetTile* tile);

    // Mengecek: Tidak boleh digadai jika masih ada bangunan di color group tersebut
    bool tryMortgage(std::shared_ptr<Player> player, PropertyTile* tile);
    bool tryUnmortgage(std::shared_ptr<Player> player, PropertyTile* tile);
    // Membungkus panggilan tile->calculateRent(), misal tambahan untuk kasus khusus UtilityTile yang membutuhkan nilai dadu
    float getFinalRentPrice(PropertyTile* tile, int diceRoll = 0) const;

    // Memulai festival di suatu PropertyTile
    void startFestival(PropertyTile* tile);

    std::vector<PropertyTile*> findPropertiesOwnedByPlayer(Player* player) const;
    void doMortgage(std::shared_ptr<Player> player);

    void doUnmortgage(std::shared_ptr<Player> player);

    void InitializeBoard(FullConfigData&& config);
    void loadBoardState(std::vector<PropertySaveData> data);
};