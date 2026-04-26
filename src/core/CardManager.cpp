#include "../../include/core/CardManager.hpp"
#include "../../include/core/GameManager.hpp"
#include "../../include/models/ActionCard.hpp"
#include "../../include/models/SkillCard.hpp"
#include <algorithm>
#include <sstream>

std::unique_ptr<SkillCard> CardManager::createSkillCardFromSave(const CardSaveData& cardData) const {
    if (cardData.card_type == "move") {
        auto card = std::make_unique<MoveSkillCard>(cardData.card_type);
        card->loadFromSave(cardData.value, cardData.remaining_duration);
        return card;
    }
    if (cardData.card_type == "discount") {
        auto card = std::make_unique<DiscountSkillCard>(cardData.card_type);
        card->loadFromSave(cardData.value, cardData.remaining_duration);
        return card;
    }
    if (cardData.card_type == "shield") {
        auto card = std::make_unique<ShieldSkillCard>(cardData.card_type);
        card->loadFromSave(cardData.value, cardData.remaining_duration);
        return card;
    }
    if (cardData.card_type == "teleport") {
        auto card = std::make_unique<TeleportSkillCard>(cardData.card_type);
        card->loadFromSave(cardData.value, cardData.remaining_duration);
        return card;
    }
    if (cardData.card_type == "lasso") {
        auto card = std::make_unique<LassoSkillCard>(cardData.card_type);
        card->loadFromSave(cardData.value, cardData.remaining_duration);
        return card;
    }
    if (cardData.card_type == "demolition") {
        auto card = std::make_unique<DemolitionSkillCard>(cardData.card_type);
        card->loadFromSave(cardData.value, cardData.remaining_duration);
        return card;
    }
    return nullptr;
}

std::unique_ptr<Card> CardManager::createSkillDeckCard(const std::string& cardName) const {
    if (cardName == "move") {
        return std::make_unique<MoveSkillCard>(cardName);
    }
    if (cardName == "discount") {
        return std::make_unique<DiscountSkillCard>(cardName);
    }
    if (cardName == "shield") {
        return std::make_unique<ShieldSkillCard>(cardName);
    }
    if (cardName == "teleport") {
        return std::make_unique<TeleportSkillCard>(cardName);
    }
    if (cardName == "lasso") {
        return std::make_unique<LassoSkillCard>(cardName);
    }
    if (cardName == "demolition") {
        return std::make_unique<DemolitionSkillCard>(cardName);
    }
    return nullptr;
}

// Membaca file konfigurasi dan mengisi ketiga deck di atas
#include "../../include/core/CardManager.hpp"
// Include all specific card headers
#include "../../include/models/FinancialActionCard.hpp"
#include "../../include/models/JailActionCard.hpp"
#include "../../include/models/MoveActionCard.hpp"
#include "../../include/models/StationActionCard.hpp"
#include "../../include/models/SkillCard.hpp" 
#include <iostream>

void CardManager::initializeDecks() {
    std::cout << "No config provided. Initializing decks randomly...\n";
    for (int i = 0; i < 3; i++) {
        skill_deck.addToDiscard(std::make_unique<MoveSkillCard>("move"));
        skill_deck.addToDiscard(std::make_unique<DiscountSkillCard>("discount"));
        skill_deck.addToDiscard(std::make_unique<ShieldSkillCard>("shield"));
        skill_deck.addToDiscard(std::make_unique<TeleportSkillCard>("teleport"));
        skill_deck.addToDiscard(std::make_unique<LassoSkillCard>("lasso"));
        skill_deck.addToDiscard(std::make_unique<DemolitionSkillCard>("demolition"));
    }
    for (int i = 0; i < 4; i++) {
        chance_deck.addToDiscard(std::make_unique<FinancialActionCard>("Pajak Mendadak"));
        chance_deck.addToDiscard(std::make_unique<JailActionCard>("Tertangkap Tilang"));
        chance_deck.addToDiscard(std::make_unique<MoveActionCard>("Maju 3 Langkah"));
        chance_deck.addToDiscard(std::make_unique<StationActionCard>("Tiket Kereta Api"));
    }

    for (int i = 0; i < 4; i++) {
        community_chest_deck.addToDiscard(std::make_unique<FinancialActionCard>("Bonus Deviden"));
        community_chest_deck.addToDiscard(std::make_unique<JailActionCard>("Salah Tangkap"));
        community_chest_deck.addToDiscard(std::make_unique<MoveActionCard>("Jalan Mundur"));
        community_chest_deck.addToDiscard(std::make_unique<StationActionCard>("Kereta Cepat"));
    }

    skill_deck.shuffle();
    chance_deck.shuffle();
    community_chest_deck.shuffle();

    std::cout << "Random decks successfully generated and shuffled!\n";
}


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

void CardManager::loadCardState(const GameSaveData& data){
    std::vector<std::unique_ptr<SkillCard>> skillCards;
    skillCards.reserve(data.skill_card_deck.size());
    
    for (const auto& cardName : data.skill_card_deck) {
        std::unique_ptr<Card> base_card = createSkillDeckCard(cardName);
        
        if (base_card) {
            if (SkillCard* raw_skill_card = dynamic_cast<SkillCard*>(base_card.get())) {
                base_card.release();
                std::unique_ptr<SkillCard> new_skill_card(raw_skill_card);
                skillCards.push_back(std::move(new_skill_card));
            } else {
            }
        }
    }
    
    skill_deck.setCards(std::move(skillCards));
    skill_deck.clearDiscard();

    for (const auto& savedPlayer : data.players) {
        auto playerIt = std::find_if(GameManager::players.begin(), GameManager::players.end(), [&savedPlayer](const std::shared_ptr<Player>& player) {
            return player && player->getname() == savedPlayer.username;
        });

        if (playerIt == GameManager::players.end()) {
            continue;
        }

        for (const auto& savedCard : savedPlayer.hand_cards) {
            auto card = createSkillCardFromSave(savedCard);
            if (card) {
                (*playerIt)->addSkillCard(std::move(card));
            }
        }
    }
}

std::string CardManager::toSaveFormat() const{
    std::ostringstream out;
    out << chance_deck.toSaveFormat();
    out << community_chest_deck.toSaveFormat();
    out << skill_deck.toSaveFormat();
    return out.str();
}
