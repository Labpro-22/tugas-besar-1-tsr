#include "../../include/models/SkillCard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Effect.hpp"
#include <random>
#include <limits>

// SkillCard::onDraw
void SkillCard::onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    p.addSkillCard(std::unique_ptr<SkillCard>(this));
}

// MoveSkillCard::onDraw
void MoveSkillCard::onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    p.addSkillCard(std::unique_ptr<SkillCard>(this));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);
    this->step = dist(gen);
}

// MoveSkillCard::useEffect
void MoveSkillCard::useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    p.movePlayer(this->step);
}

// DiscountSkillCard::onDraw
void DiscountSkillCard::onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    p.addSkillCard(std::unique_ptr<SkillCard>(this));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1, 100);
    this->percentage = dist(gen);
}

// DiscountSkillCard::useEffect
void DiscountSkillCard::useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    p.addEffect(std::unique_ptr<Effect>(new DiscountEffect(this->percentage)));
}

// ShieldSkillCard::useEffect
void ShieldSkillCard::useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    p.addEffect(std::unique_ptr<Effect>(new ShieldEffect()));
}

// TeleportSkillCard::useEffect
void TeleportSkillCard::useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    int steps;
    // std::cin>>steps;
    p.movePlayer(steps);
}

// LassoSkillCard::useEffect
void LassoSkillCard::useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    Player* moved = nullptr;
    int distance = (int)std::numeric_limits<float>::infinity();
    for (auto& pl : all) {
        int temp = pl.getPosition() - p.getPosition();
        if (temp < distance && temp >= 0 && &p != &pl) {
            distance = temp;
            moved = &pl;
        }
    }
    if (moved) {
        moved->movePlayer(-distance);
    }
}

// DemolitionSkillCard::useEffect
void DemolitionSkillCard::useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) {
    // TODO: Implement demolition skill card effect
}
