#pragma once
#include <string>

class Player;

class Tile {
protected:
    int index;
    std::string name;
    std::string code;
    
public:
    Tile(int index, std::string name, std::string code);
    virtual ~Tile() = default;
    int getIndex() const;
    std::string getName() const;
    std::string getCode() const;
    virtual void onLand(Player& p) = 0;
};

enum CardType {
    CHANCE,
    COMMUNITY_CHEST
};

class CardTile : public Tile {
private:
    CardType card;

public:
    CardTile(int index, std::string name, std::string code, CardType card);
    void onLand(Player& p) override;
};

enum TaxType {
    PPH,
    PBM
};

class TaxTile : public Tile {
private:
    TaxType tax_type;
    int tax_amount;

public:
    TaxTile(int index, std::string name, std::string code, TaxType tax_type, int tax_amount);
    void onLand(Player& p) override;
};

class FestivalTile : public Tile {
public:
    FestivalTile(int index, std::string name, std::string code);
    void onLand(Player& p) override;
};

class GoTile : public Tile {
private:
    int go_reward;

public:
    GoTile(int index, std::string name, std::string code, int go_reward);
    void onLand(Player& p) override;
};

class GoToJailTile : public Tile {
public:
    GoToJailTile(int index, std::string name, std::string code);
    void onLand(Player& p) override;
};

class FreeParkingTile : public Tile {
public:
    FreeParkingTile(int index, std::string name, std::string code);
    void onLand(Player& p) override;
};

class JailTile : public Tile {
private:
    int jail_fine;

public:
    JailTile(int index, std::string name, std::string code, int jail_fine);
    void onLand(Player& p) override;
};