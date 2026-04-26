#pragma once

#include "../../include/models/Saveable.hpp"
#include <string>
class GameManager;
class SaveLoadManager{
     public:
        void save(const GameManager& g, const std::string& filepath);
};