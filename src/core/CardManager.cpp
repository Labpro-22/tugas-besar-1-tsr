#include "../../include/core/CardManager.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include <sstream>

// Membaca file konfigurasi dan mengisi ketiga deck di atas
void CardManager::initializeDecks(){}


// Ketika pemain mendarat di petak Kesempatan
void CardManager::drawKesempatan(Player& player, Board& b, std::vector<std::shared_ptr<Player>>& all, TileVisitor& visitor){
    std::unique_ptr<Card> card = chance_deck.drawCard();
    card->onDraw(player);
    discardCard(std::move(card),chance_deck);
}
// Ketika pemain mendarat di petak Dana Umum
void CardManager::drawDanaUmum(Player& player, Board& b, std::vector<std::shared_ptr<Player>>& all, TileVisitor& visitor){
    std::unique_ptr<Card> card = chance_deck.drawCard();
    card->onDraw(player);
    discardCard(std::move(card),chance_deck);
}
// Memberikan Skill Card ke pemain
void CardManager::giveSkillCardToPlayer(Player& player, Board& b, std::vector<std::shared_ptr<Player>>& all, TileVisitor& visitor){
    std::unique_ptr<Card> card = chance_deck.drawCard();
    card->onDraw(player);
}

void CardManager::discardCard(std::unique_ptr<Card> usedCard, Deck<std::unique_ptr<Card>>& deck){
       deck.addToDiscard(std::move(usedCard));
}
std::string CardManager::toSaveFormat() const{
    std::ostringstream out;
    out << chance_deck.toSaveFormat();
    out << community_chest_deck.toSaveFormat();
    out << skill_deck.toSaveFormat();
    return out.str();
}