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
    void assignOwnership(PropertyTile& tile, Player& newOwner);

    // Mengecek: 1. Apakah Monopoli? 2. Apakah pemerataan bangunan (even build) terpenuhi? Jika valid, panggil tile->buildHouse()
    bool tryBuildHouse(Player& player, StreetTile& tile);
    
    // Mengecek: 1. Apakah sudah 4 rumah? 2. Apakah pemerataan hotel terpenuhi?
    bool tryBuildHotel(Player& player, StreetTile& tile);

    // Mengecek: Tidak boleh digadai jika masih ada bangunan di color group tersebut
    bool tryMortgage(Player& player, PropertyTile& tile);
    bool tryUnmortgage(Player& player, PropertyTile& tile);

    // Memulai festival di suatu PropertyTile
    void startFestival(PropertyTile& tile);
};