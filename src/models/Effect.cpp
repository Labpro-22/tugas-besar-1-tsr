#include "../../include/models/Effect.hpp"
#include "../../include/models/Player.hpp"

// Effect base class
void Effect::onTurnStart(Player& p) {}
void Effect::onTurnEnd(Player& p) {}

bool Effect::blocksPayment() const {
    
}

int Effect::modifyPayment(int amount) const {
    
}

// ShieldEffect
ShieldEffect::ShieldEffect(int duration) : turns_left(duration) {}

void ShieldEffect::onTurnEnd(Player& p) {
    
}

bool ShieldEffect::isExpired() const {
    
}

bool ShieldEffect::blocksPayment() const {
    
}

// DiscountEffect
DiscountEffect::DiscountEffect(int percent, int duration) 
    : percent(percent), turns_left(duration) {}

void DiscountEffect::onTurnEnd(Player& p) {
    
}

bool DiscountEffect::isExpired() const {
    
}

int DiscountEffect::modifyPayment(int amount) const {
    
}