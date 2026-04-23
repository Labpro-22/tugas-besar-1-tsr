#pragma once
#include "PropertyTile.hpp"
#include <vector>

class StreetTile : public PropertyTile {
private:
    int building_level;
    std::vector<int> rent_list;
    int house_price;
    int hotel_price;

public:
    StreetTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status, int building_level, const std::vector<int>& rent_list, int house_price, int hotel_price);
               
    int calculateRent() const override;
    void onLand(Player& p, TileVisitor& visitor) override;
    PropertyType getPropertyType() const override;

    int getBuildingLevel() const;
    bool canBuildHouse() const;
    bool canBuildHotel() const;
    
    bool canSellBuilding() const;

    void buildHouse();
    void buildHotel();
    void sellBuilding();

    int getHousePrice() const;
    int getHotelPrice() const;
};