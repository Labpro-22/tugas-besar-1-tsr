#pragma once

#include "ActionCard.hpp"
#include <random>

class MoveActionCard:public ActionCard{
    private:
        static std::random_device rd;
        static std::mt19937 gen;
        int walkDistance;
    public:
        MoveActionCard(std::string name);
        void onDraw(Player& p) override;
};