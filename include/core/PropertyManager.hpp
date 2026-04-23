#pragma once

#include <memory>
#include <string>
#include "Board.hpp"
#include "PropertyTile.hpp"
#include "StreetTile.hpp"
#include "Player.hpp"

class PropertyManager {
private:
    std::unique_ptr<Board> board;

public:
    PropertyManager(std::unique_ptr<Board> gameBoard);
    ~PropertyManager() = default;
    Board& getBoard() const;
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
    int getFinalRentPrice(PropertyTile* tile, int diceRoll = 0) const;

    // Memulai festival di suatu PropertyTile
    void startFestival(PropertyTile* tile);
};