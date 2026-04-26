#include "../../include/core/SaveLoadManager.hpp"
#include "../../include/core/GameManager.hpp" 
#include <fstream>
#include <iostream>
#include "../../include/core/GameException.hpp"

void SaveLoadManager::save(const GameManager& g, const std::string& filepath) {
    std::ofstream outFile(filepath);


    if (!outFile.is_open()) {
        throw SaveLoadException("Could not open or create file at " + filepath + "\n");
        return;
    }
    std::string saveData = g.toSaveFormat();

    outFile << saveData;
    outFile.close();

    std::cout << "Game successfully saved to " << filepath << "!\n";
}