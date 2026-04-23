#include "ActionCard.hpp"

class StationActionCard : public ActionCard {
    private:
        Player& owner;
        

    public:
        StationActionCard(Player& owner);
        void onDraw(Player& p, Board& b, std::vector<Player>& all) override;


};