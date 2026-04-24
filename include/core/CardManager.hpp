#pragma once

#include <memory>
#include "../../include/models/Deck.hpp"
#include "../../include/models/Card.hpp"
#include "../../include/models/Player.hpp" 
#include "../../include/models/Saveable.hpp"

class CardManager : public Saveable {
private:
    Deck<std::unique_ptr<Card>> chance_deck;
    Deck<std::unique_ptr<Card>> community_chest_deck;
    Deck<std::unique_ptr<Card>> skill_deck;

public:
    CardManager();
    ~CardManager() = default;

    void initializeDecks();
    void drawKesempatan(Player& player);
    void drawDanaUmum(Player& player);
    void giveSkillCardToPlayer(Player& player);
    

    void discardSkillCard(SkillCard* usedCard); 
    
    std::string toSaveFormat() const override;
};