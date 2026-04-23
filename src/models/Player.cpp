#include "Player.hpp"
#include <sstream>

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