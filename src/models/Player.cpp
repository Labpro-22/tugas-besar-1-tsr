#include "../../include/models/Player.hpp"
#include <sstream>

Player::Player(std::string name, int balance, int position, PlayerState player_state){}

Player& Player::operator+=(int amount){}
Player& Player::operator-=(int amount){}
Player Player::operator+(int amount) const{}
Player Player::operator-(int amount) const{}

int Player::processPayment(int amount) const{}
int Player::pay(int amount){}
void Player::receive(int amount){}       
void Player::transferTo(Player&, int){} 

void Player::movePlayer(int steps){}
void Player::addProperty(PropertyTile* property){}
void Player::removeProperty(PropertyTile* property){}
int Player::countOwnedRailroad() const{}
int Player::countOwnedUtility() const{}

void Player::addSkillCard(std::unique_ptr<SkillCard> card){}
void Player::useSkillCard(int index){}
void Player::addEffect(std::unique_ptr<Effect> effect){}

int Player::getTotalAssetValue(){}
bool Player::canPay(int amount){}
int Player::liquidateAsset(int required){}
void Player::declareBankruptcy(){}

bool Player::inJail(){}
void Player::setInJail(){}
void Player::setFree(){}
void Player::startTurn(){}
void Player::endTurn(){}
int Player::getPosition(){}

std::string Player::toSaveFormat() const {
    std::ostringstream out;
    out << name << " " << balance << " " << position;
    switch (player_state)
    {
    case PlayerState::FREE:
        out<<"FREE\n";
        break;
    case PlayerState::INJAIL:
        out<<"INJAIL\n";
        break;
    }

    out << saved_cards.size() << "\n";

    
    // for(const std::unique_ptr<SkillCard>& card : saved_cards){
    //     card->
    // }


    return out.str();
}