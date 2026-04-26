// #pragma once

#include "../all.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <map>

class ViewGame {
public:
    ViewGame() = default;
    ~ViewGame() = default;

    static void displayException(const GameException& e);

    // ------ INPUT --------

    static std::string getUserCommand();

    static std::string getPlayerName();

    static std::string getPropertyName();

    static float getInt(float max_int);

    static bool getYesNo();

    static std::string getUserInput();

    // ------ OUTPUT --------

    static void displayMessage(const std::string& message);

    // CETAK_PAPAN
    static void displayBoard();
    
    // DADU
    static void displayDiceRollResult(const std::string& player_name, float dice1, float dice2, const std::string& dest_tile);
    static void displayManualDiceRollResult(const std::string& player_name, float dice1, float dice2, const std::string& dest_tile);

    // CETAK_AKTA
    static void displayPropertyDeed(const std::string& property_code);

    // CETAK_PROPERTI
    static void displayPlayerProperties(const std::string& player_name);

    // Buy
    static void displayBuyPromptStreet(const std::string& prop_name, const std::string& prop_code, const std::string& color, float price, const std::vector<float>& rent, float current_money);
    static void displayBuySuccess(const std::string& prop_name, float remaining_money);
    static void displayBuyAuctionWarning();
    static void displayBuyAutoSuccess(const std::string& type, const std::string& prop_name);

    // Rent
    static void displayRentPayment(const PropertyTile& property, const Player& victim, const Player& owner, float rent_amount);
    static void displayMortgagedRent(const PropertyTile& property, const Player& owner);
    static void displayRentBankruptcyWarning(float rent_amount, float current_money);

    // Tax
    static void displayPPH(TaxTile* tile);
    static void displayPBM(TaxTile* tile);

    // Mortgage
    static void displayMortgageList(const std::vector<PropertyTile*>& properties);
    static void displayBuildingSellList(const std::string& color_group, const std::vector<StreetTile*>& streets);
    static void displayBuildingSellSuccess(const std::string& prop_name, float sell_gain);
    static void displayMortgageSuccess(const std::string& prop_name, float gain_amount, float current_balance);
    static void displayNoMortgageableError();

    // Unmortgage
    static void displayUnmortgageList(const std::vector<PropertyTile*>& properties, float current_balance);
    static void displayUnmortgageSuccess(const std::string& prop_name, float cost, float current_balance);
    static void displayUnmortgageFailure(const std::string& prop_name, float cost, float current_balance);
    static void displayNoMortgagedProperties();

    // Build
    static void displayBuildableGroups(const std::map<std::string, std::vector<StreetTile*>>& groups, float current_money);
    static void displayBuildableTilesInGroup(const std::string& color_group, const std::vector<StreetTile*>& group_streets, const std::vector<StreetTile*>& eligible_streets);
    static void displayBuildSuccess(const std::string& prop_name, float cost, float remaining_money);
    static void displayUpgradeHotelPrompt(const std::string& color_group, const std::vector<StreetTile*>& group_streets);
    static void displayUpgradeHotelSuccess(const std::string& prop_name, float cost, float remaining_money);
    static void displayHotelMaxError(const std::string& prop_name);
    static void displayNoBuildableError();

    // Auction
    static void displayAuctionStart(const std::string& prop_name, const std::string& next_player_name);
    static void displayAuctionTurn(const std::string& player_name, float highest_bid, const std::string& highest_bidder);
    static void displayAuctionEnd(const std::string& prop_name, const std::string& winner_name, float final_price);

    // Festival
    static void displayFestivalStart(const std::vector<PropertyTile*>& owned_props);
    static void displayFestivalEffect(float old_rent, float new_rent, float duration);
    static void displayFestivalEnhanced(float old_rent, float new_rent, float duration);
    static void displayFestivalMaxed(float duration);
    static void displayFestivalErrorInvalidCode();
    static void displayFestivalErrorNotOwned();

    // Liquidation and Bankruptcy
    static void displayLiquidationWarning(float current_money, float total_debt, float deficit, float max_potential, const std::string& creditor_name);
    static void displayLiquidationPanel(float current_money, float debt, const std::vector<StreetTile*>& sellable_buildings, const std::vector<PropertyTile*>& mortgageables);
    static void displayLiquidationSellSuccess(const std::string& prop_name, float gain, float current_money);
    static void displayLiquidationDebtCleared(float debt, const std::string& creditor_name, float remaining_money, float creditor_before, float creditor_after);
    static void displayBankruptcyToPlayer(const std::string& bankrupt_player, const std::string& creditor, float remaining_money, float total_assets, float debt);
    static void displayBankruptcyToBank(const std::string& bankrupt_player, float remaining_money, float total_assets, float debt);

    // Save and Load
    static void displaySaveSuccess(const std::string& filename);
    static void displaySaveOverwritePrompt(const std::string& filename);
    static void displaySaveError();
    static void displayLoadSuccess(const std::string& current_player_turn);
    static void displayLoadErrorNotFound(const std::string& filename);
    static void displayLoadErrorCorrupted();

    // Log
    static void displayLogHeader(float count = 0.0f);
    static void displayLogEntry(const std::vector<LogEntry>& logs);

    // Win
    static void displayWinMaxTurn(const std::vector<Player*>& players);
    static void displayWinBankruptcy(const std::vector<Player*>& players);

    // Chance and Community Chest
    static void displayCardDraw(const std::string& petak_type, const std::string& card_text);
    static void displayCardEffectMoney(float amount, float remaining_money, bool is_paying);
    static void displayCardEffectMove(const std::string& target_petak);
    static void displayCardBankruptcyWarning(float amount, float current_money);

    // Skill Card
    static void displaySkillCardList(const std::vector<std::unique_ptr<SkillCard>>& cards);
    static void displaySkillCardActivated(const std::string& card_name, const std::string& effect_desc);
    static void displaySkillCardShieldActive(float canceled_amount, float current_money);
    static void displaySkillCardErrorAlreadyUsed();
    static void displaySkillCardErrorAfterDice();

    // Drop Card
    static void displayDropCardWarning(const std::string& new_card_name);
    static void displayDropCardList(const std::vector<SkillCard*>& cards);
    static void displayDropCardSuccess(const std::string& dropped_card_name);
};