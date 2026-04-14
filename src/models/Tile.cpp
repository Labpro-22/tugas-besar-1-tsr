#include "Tile.hpp"

// Tile
Tile::Tile(int index, std::string name, std::string code) 
    : index(index), name(name), code(code) {}

int Tile::getIndex() const {
    return index;
}

std::string Tile::getName() const {
    return name;
}

std::string Tile::getCode() const {
    return code;
}

// CardTile 
CardTile::CardTile(int index, std::string name, std::string code, CardType card) 
    : Tile(index, name, code), card(card) {}

void CardTile::onLand(Player& p) {

}

// TaxTile
TaxTile::TaxTile(int index, std::string name, std::string code, TaxType tax_type, int tax_amount)
    :  Tile(index, name, code), tax_type(tax_type), tax_amount(tax_amount) {}

void TaxTile::onLand(Player& p) {

}

// FestivalTile
FestivalTile::FestivalTile(int index, std::string name, std::string code)
    : Tile(index, name, code) {}

void FestivalTile::onLand(Player& p){

}

// GoTile
GoTile::GoTile(int index, std::string name, std::string code, int go_reward) 
    : Tile(index, name, code), go_reward(go_reward) {}

void GoTile::onLand(Player& p) {

}

// GoToJailTile
GoToJailTile::GoToJailTile(int index, std::string name, std::string code)
    : Tile(index, name, code) {}

void GoToJailTile::onLand(Player& p) {

}

// FreeParkingTile
FreeParkingTile::FreeParkingTile(int index, std::string name, std::string code)
    : Tile(index, name, code) {}

void FreeParkingTile::onLand(Player& p) {

}

// JailTile
JailTile::JailTile(int index, std::string name, std::string code, int jail_fine)
    : Tile(index, name, code), jail_fine(jail_fine) {}

void JailTile::onLand(Player& p) {
    
}