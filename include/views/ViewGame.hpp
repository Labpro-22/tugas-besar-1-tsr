#pragma once

#include <string>
#include <vector>
#include <iostream>

class GameManager; 

class ViewGame {
public:
    ViewGame() = default;
    ~ViewGame() = default;

    // ------ INPUT --------

    static std::string getUserCommand();

    static std::string getPlayerName();

    static std::string getPropertyName();

    static int gettInt(int max_int);

    static bool getYesNo();

    static void displayMessage(const std::string& message);

    static std::string getUserInput();

    // ------ OUTPUT --------

    static void displayBoard(const GameManager& gm);
    
    static void displayDiceRollResult(const std::string& playerName, int dice1, int dice2, const std::string& destTile);
    static void displayManualDiceRollResult(const std::string& playerName, int dice1, int dice2, const std::string& destTile);

    static void displayPropertyDeed(const GameManager& gm, const std::string& propertyCode);

    static void displayPlayerProperties(const GameManager& gm, const std::string& playerName);

    // Buy
    static void displayBuyPrompt(const GameManager& gm);
    static void displayBuyResult(bool success, int remainingMoney);

    // Rent
    static void displayRentPayment(const PropertyTile& property, const Player& victim, const Player& owner, int rentAmount);
    static void displayMortgagedRent(const PropertyTile& property, const Player& owner);
    static void displayRentBankruptcyWarning(int rentAmount, int currentMoney);

    // Tax
    static void displayPphPrompt();
    static void displayPphPercentageResult(const Player& player, int propValue, int buildValue, int taxAmount);
    static void displayTaxSuccess(const std::string& taxName, const Player& player, int taxAmount);
    static void displayTaxBankruptcyWarning(const std::string& taxName, int taxAmount, int currentMoney);

    // Mortgage
    static void displayMortgageList(const std::vector<PropertyTile*>& properties);
    static void displayBuildingSellList(const std::string& color_group, const std::vector<StreetTile*>& streets);
    static void displayBuildingSellSuccess(const std::string& prop_name, int sell_gain);
    static void displayMortgageSuccess(const std::string& prop_name, int gain_amount, int current_balance);
    static void displayNoMortgageableError();

    // Unmortgage
    static void displayUnmortgageList(const std::vector<PropertyTile*>& properties, int currentBalance);
    static void displayUnmortgageSuccess(const std::string& propName, int cost, int currentBalance);
    static void displayUnmortgageFailure(const std::string& propName, int cost, int currentBalance);
    static void displayNoMortgagedProperties();

    // Build
    class BuildableGroup {
    public:
        std::string color_group;
        std::vector<StreetTile*> streets;
    };
    
    static void displayBuildableGroups(const std::vector<BuildableGroup>& groups, int current_money);
    static void displayBuildableTilesInGroup(const std::string& color_group, const std::vector<StreetTile*>& group_streets, const std::vector<StreetTile*>& eligible_streets);
    
    static void displayBuildSuccess(const std::string& prop_name, int cost, int remaining_money);
    static void displayUpgradeHotelPrompt(const std::string& color_group, const std::vector<StreetTile*>& group_streets);
    static void displayUpgradeHotelSuccess(const std::string& prop_name, int cost, int remaining_money);
    static void displayHotelMaxError(const std::string& prop_name);
    static void displayNoBuildableError();

    // Auction
    static void displayAuctionStart(const std::string& prop_name, const std::string& next_player_name);
    static void displayAuctionTurn(const std::string& player_name, int highest_bid, const std::string& highest_bidder);
    static void displayAuctionEnd(const std::string& prop_name, const std::string& winner_name, int final_price);

    // Festival
    static void displayFestivalStart(const std::vector<PropertyTile*>& owned_props);
    static void displayFestivalEffect(int old_rent, int new_rent, int duration);
    static void displayFestivalEnhanced(int old_rent, int new_rent, int duration);
    static void displayFestivalMaxed(int duration);
    static void displayFestivalErrorInvalidCode();
    static void displayFestivalErrorNotOwned();

    // Liquidation
    static void displayLiquidationWarning(int current_money, int total_debt, int deficit, int max_potential, const std::string& creditor_name);
    
    class LiquidationItem {
    public:
        std::string name;
        std::string code;
        std::string color;
        int value; 
        std::string detail; 
    };
    
    static void displayLiquidationPanel(int current_money, int debt, const std::vector<LiquidationItem>& sellables, const std::vector<LiquidationItem>& mortgageables);
    static void displayLiquidationSellSuccess(const std::string& prop_name, int gain, int current_money);
    static void displayLiquidationDebtCleared(int debt, const std::string& creditor_name, int remaining_money, int creditor_before, int creditor_after);
    
    // Bankrupt
    static void displayBankruptcyToPlayer(const std::string& bankrupt_player, const std::string& creditor, int remaining_money, int total_assets, int debt);
    static void displayBankruptcyToBank(const std::string& bankrupt_player, int remaining_money, int total_assets, int debt);

    // Save
    static void displaySaveSuccess(const std::string& filename);
    static void displaySaveOverwritePrompt(const std::string& filename);
    static void displaySaveError();
    
    // Load
    static void displayLoadSuccess(const std::string& current_player_turn);
    static void displayLoadErrorNotFound(const std::string& filename);
    static void displayLoadErrorCorrupted();

    // Log
    static void displayLogHeader(int count = 0);
    static void displayLogEntry(const std::vector<LogEntry> logs);

    // Winner
    class PlayerRecap {
    public:
        std::string name;
        int money;
        int property_count;
        int card_count;
    };
    
    static void displayWinMaxTurn(const std::vector<PlayerRecap>& recaps, const std::vector<std::string>& winners);
    static void displayWinBankruptcy(const std::string& winner, const std::vector<std::string>& remaining_players);

    // Use Action Card
    static void displayCardDraw(const std::string& petak_type, const std::string& card_text);
    static void displayCardEffectMoney(int amount, int remaining_money, bool is_paying);
    static void displayCardEffectMove(const std::string& target_petak);
    static void displayCardBankruptcyWarning(int amount, int current_money);

    // Use SKill Card
    class CardDesc {
    public:
        std::string name;
        std::string description;
    };
    
    static void displaySkillCardList(const std::vector<SkillCard>& cards);
    static void displaySkillCardActivated(const std::string& card_name, const std::string& effect_desc);
    static void displaySkillCardShieldActive(int canceled_amount, int current_money);
    static void displaySkillCardErrorAlreadyUsed();
    static void displaySkillCardErrorAfterDice();

    // Drop Skill Card
    static void displayDropCardWarning(const std::string& new_card_name);
    static void displayDropCardList(const std::vector<SkillCard>& cards);
    static void displayDropCardSuccess(const std::string& dropped_card_name);
};