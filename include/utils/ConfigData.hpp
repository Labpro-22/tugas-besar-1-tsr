#pragma once

#include <string>
#include <vector>
#include <map>

class StreetConfigData {
public:
    int id;
    std::string code;
    std::string name;
    std::string color;
    int buy_price;
    int mortgage_price;
    int house_price;
    int hotel_price;
    std::vector<int> rent_prices;
};

class ActionTileConfigData {
public:
    int id;
    std::string code;
    std::string name;
    std::string type;
    std::string color;
};

class FullConfigData {
public:
    std::vector<StreetConfigData> streets;
    std::vector<ActionTileConfigData> action_tiles;
    std::map<int, int> railroad_rent_map;
    std::map<int, int> utility_multiplier_map; 

    int pph_flat;
    int pph_percent;
    int pbm_flat;

    int go_salary;
    int jail_fine;

    int max_turn;
    int initial_balance;

    FullConfigData() : pph_flat(0), pph_percent(0), pbm_flat(0), go_salary(0), jail_fine(0), max_turn(0), initial_balance(0) {}
};