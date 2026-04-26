#include "../../include/models/SkillCard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Effect.hpp"
#include "../../include/core/GameManager.hpp"
#include <random>
#include <limits>
#include <sstream>

// SkillCard
SkillCard::SkillCard(std::string name):name(name){};
void SkillCard::onDraw(Player& p) {
    p.addSkillCard(std::unique_ptr<SkillCard>(this));
}
std::string SkillCard::toSaveFormat() const{
    std::ostringstream out;
    out << name << "\n";
    return out.str();
}

// MoveSkillCard
MoveSkillCard::MoveSkillCard(std::string name):SkillCard(name){}
void MoveSkillCard::onDraw(Player& p) {
    p.addSkillCard(std::unique_ptr<SkillCard>(this));
    std::uniform_int_distribution<> dist(1, 100);
    this->step = dist(gen);
}
void MoveSkillCard::useEffect(Player& p) {
    p.movePlayer(this->step);
}
std::string MoveSkillCard::toSaveFormat() const{
    std::ostringstream out;
    out << name << " " << step << "\n";
    return out.str();
}

// DiscountSkillCard
DiscountSkillCard::DiscountSkillCard(std::string name):SkillCard(name){}
void DiscountSkillCard::onDraw(Player& p) {
    p.addSkillCard(std::unique_ptr<SkillCard>(this));
    std::uniform_real_distribution<> dist(1, 100);
    this->percentage = dist(gen);
}

void DiscountSkillCard::useEffect(Player& p) {
    p.addEffect(std::unique_ptr<Effect>(new DiscountEffect(this->percentage)));
}
std::string DiscountSkillCard::toSaveFormat() const{
    std::ostringstream out;
    out << name << " " << percentage << "\n";
    return out.str();
}


// ShieldSkillCard
ShieldSkillCard::ShieldSkillCard(std::string name):SkillCard(name){}
void ShieldSkillCard::useEffect(Player& p) {
    p.addEffect(std::unique_ptr<Effect>(new ShieldEffect()));
}

// TeleportSkillCard
TeleportSkillCard::TeleportSkillCard(std::string name):SkillCard(name){}
void TeleportSkillCard::useEffect(Player& p) {
    int steps;
    p.movePlayer(steps);
}

// LassoSkillCard
LassoSkillCard::LassoSkillCard(std::string name):SkillCard(name){}
void LassoSkillCard::useEffect(Player& p) {
    auto &all=GameManager::players;
    Player* moved = nullptr;
    int distance = (int)std::numeric_limits<float>::infinity();
    for (auto& pl : all) {
        int temp = pl->getPosition() - p.getPosition();
        if (temp < distance && temp >= 0 && &p != pl.get()) {
            distance = temp;
            moved = pl.get();
        }
    }
    if (moved) {
        moved->movePlayer(-distance);
    }
}


// DemolitionSkillCard
DemolitionSkillCard::DemolitionSkillCard(std::string name):SkillCard(name){}
void DemolitionSkillCard::useEffect(Player& p) {
    // TODO: Implement demolition skill card effect
}
