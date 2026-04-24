#include "../../include/core/CardManager.hpp"

// Membaca file konfigurasi dan mengisi ketiga deck di atas
void CardManager::initializeDecks(){}
// Ketika pemain mendarat di petak Kesempatan
void CardManager::drawKesempatan(Player& player){}
// Ketika pemain mendarat di petak Dana Umum
void CardManager::drawDanaUmum(Player& player){}
// Memberikan Skill Card ke pemain
void CardManager::giveSkillCardToPlayer(Player& player){}
// Fungsi ini dipanggil oleh GameManager ketika pemain selesai menggunakan SkillCard.
void CardManager::discardSkillCard(SkillCard* usedCard){}
std::string CardManager::toSaveFormat() const{}