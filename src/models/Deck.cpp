#include "../../include/models/Deck.hpp"
template<>
std::string Deck<ActionCard>::toSaveFormat() const{

}
template<>  
std::string Deck<SkillCard>::toSaveFormat() const{

}
template<>  
std::string Deck<Card*>::toSaveFormat() const{

}

// template <>
// ActionCard Deck<ActionCard>::drawCard(){
//     if(cards.empty()){
//         throw "placeholder";
//     }
//     ActionCard temp= cards.top();
//     cards.pop();
//     this->AddToDiscard(temp);
//     return temp;
// }