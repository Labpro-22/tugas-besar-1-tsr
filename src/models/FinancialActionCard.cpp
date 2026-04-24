#include "../../include/models/FinancialActionCard.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/PropertyTile.hpp"


FinancialActionCard::FinancialActionCard(std::string name):ActionCard(name){
    std::uniform_int_distribution<> amount_dist(1, 100); 
    std::uniform_int_distribution<> type_dist(0, 3); // SUBJECT TO CHANGE
    amount = amount_dist(gen);
    t = static_cast<transaction_type>(type_dist(gen));
}

void FinancialActionCard::onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    switch(t){
        case transaction_type::PLAYER_TO_ALL:
            for(Player& pl:all){
                if(&pl==&p)continue;
                else p.transferTo(pl,amount);
            }
            break;
        case transaction_type::ALL_TO_PLAYER:
            for(Player& pl:all){
                if(&pl==&p)continue;
                pl.transferTo(p,amount);
            }
            break;
        case transaction_type::PLAYER_TO_BANK:
            p.pay(amount);
            break;
        case transaction_type::BANK_TO_PLAYER:
            p.receive(amount);
            break;
    }
}