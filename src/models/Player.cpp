#include <algorithm>
#include <memory>
#include "../models/Player.hpp"
#include "../models/PropertyTile.hpp"
#include "../models/SkillCard.hpp"

Player::Player(std::string name, int balance, int position):name(name),balance(balance),position(position){}
Player& Player::operator+=(int amount){
    balance += amount;
    return *this;
}
Player& Player::operator-=(int amount){
    balance -= amount;
    return *this;
}
Player Player::operator+(int amount) const{
    Player result = *this;
    result.balance += amount;
    return result;
}
Player Player::operator-(int amount) const{
    Player result = *this;
    result.balance -= amount;
    return result;
}

void Player::transferTo(Player& player, int amount){
    player+=amount;
} 
void Player::movePlayer(int steps){
    position+=steps;
}
void Player::addProperty(PropertyTile* property){
    this->owned_properties.push_back(property);
}
void Player::removeProperty(PropertyTile* property){
    owned_properties.erase(
        std::remove(owned_properties.begin(), owned_properties.end(), property),
        owned_properties.end()
    );
}
void Player::addSkillCard(std::unique_ptr<SkillCard> card){
    this->saved_cards.push_back(card);
}
void Player::useSkillCard(int index){
    this->saved_cards[index]->useEffect();
}
void Player::addEffect(std::unique_ptr<Effect> effect){
    this->active_effects.push_back(std::move(effect));
}
int Player::getTotalAssetValue(){
    int sum=this->balance;
    for (auto& asset:this->owned_properties){
        sum+=asset->getSellValue();
    }
    return sum;
}
bool Player::canPay(int amount){
    return this->balance>=amount;
}
void Player::buyProperty(PropertyTile &property){
    float dihcount=0;
    if(!this->canPay(property.getSellValue())){
        throw "uang tak cukup";
    }
    *this-=property.getSellValue();
    this->addProperty(&property);
    property.setPropertyOwner(std::make_shared<Player>(this));
}
void Player::sellProperty(PropertyTile &property, Player& other){
    this->removeProperty(&property);
    *this+=property.getSellValue();
    other.buyProperty(property);
}
int Player::liquidateAsset(int required){
    std::vector<PropertyTile*> properties_to_sell = owned_properties;
    std::sort(
        properties_to_sell.begin(),
        properties_to_sell.end(),
        [](PropertyTile* a, PropertyTile* b) {
            return a->getSellValue() < b->getSellValue();
        }
    );

    int liquidated = 0;
    for (PropertyTile* property : properties_to_sell) {
        if (liquidated >= required) {
            break;
        }

        int sell_value = property->getSellValue();
        liquidated += sell_value;
        balance += sell_value;
        removeProperty(property);
        property->setPropertyStatus(BANK);
        property->setPropertyOwner(std::shared_ptr<Player>());
    }

    return liquidated;
}
