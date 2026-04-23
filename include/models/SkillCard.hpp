#include "Card.hpp"
class SkillCard: public Card
{
private:
    /* data */
public:
    virtual void useEffect(Player& p, Board& b, std::vector<Player>& all)=0;
    /*Read my notes on Card.hpp on why I made this change -Billie*/
};


