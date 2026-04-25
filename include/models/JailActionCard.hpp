#pragma once

#include "ActionCard.hpp"
#include <random>
class JailActionCard : public ActionCard { 
    public:
        void onDraw(Player& p) override;
};