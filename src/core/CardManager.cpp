#include "../../include/core/CardManager.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include <sstream>

// Membaca file konfigurasi dan mengisi ketiga deck di atas
void CardManager::initializeDecks(){}


// Ketika pemain mendarat di petak Kesempatan
void CardManager::drawKesempatan(Player& player){
    std::unique_ptr<ActionCard> card = chance_deck.drawCard();
    card->onDraw(player);
    discardCard(std::move(card), chance_deck);
}
// Ketika pemain mendarat di petak Dana Umum
void CardManager::drawDanaUmum(Player& player){
    std::unique_ptr<ActionCard> card = community_chest_deck.drawCard();
    card->onDraw(player);
    discardCard(std::move(card), community_chest_deck);
}
// Memberikan Skill Card ke pemain
void CardManager::giveSkillCardToPlayer(Player& player){
    std::unique_ptr<SkillCard> card = std::move(skill_deck.drawCard());
    card->onDraw(player);
    player.addSkillCard(std::move(card));
}

void CardManager::takeSkillCardFromPlayer(Player& player, int index){
    skill_deck.addToDiscard(std::move(player.removeSkillCard(index)));
}

void CardManager::discardCard(std::unique_ptr<ActionCard> usedCard, Deck<std::unique_ptr<ActionCard>>& deck){
       deck.addToDiscard(std::move(usedCard));
}
std::string CardManager::toSaveFormat() const{
    std::ostringstream out;
    out << chance_deck.toSaveFormat();
    out << community_chest_deck.toSaveFormat();
    out << skill_deck.toSaveFormat();
    return out.str();
}
