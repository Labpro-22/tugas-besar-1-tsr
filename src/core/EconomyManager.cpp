#include "../../include/core/EconomyManager.hpp"

void EconomyManager::addMoney(Player& player, int amount){}
bool EconomyManager::deductMoney(Player& player, int amount){}
bool EconomyManager::transferMoney(Player& payer, Player& receiver, int amount){}
// Pemrosesan pajak
bool EconomyManager::processTax(Player& player, TaxType type, int baseTaxAmount, TransactionLog& logger){}
// Proses lelang
void EconomyManager::startAuction(PropertyTile* property, const std::vector<std::shared_ptr<Player>>& players){}
bool EconomyManager::placeBid(int amount){}
void EconomyManager::foldBid(){}
bool EconomyManager::isAuctionOver() const{}
void EconomyManager::resolveAuction(PropertyManager& propMgr, TransactionLog& logger){}
std::shared_ptr<Player> EconomyManager::getCurrentBidder() const{}
int EconomyManager::getHighestBid() const{}
// Proses ketika pemain tidak bisa membayar kewajiban
bool EconomyManager::isBankruptcyInevitable(const Player& player, int debtAmount) const{}
void EconomyManager::executeBankruptcy(Player& bankruptPlayer, 
                       std::shared_ptr<Player> creditor, 
                       PropertyManager& propMgr, 
                       TransactionLog& logger){}