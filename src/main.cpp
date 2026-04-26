#include "../include/utils/IOManager.hpp"
#include "../include/utils/ConfigData.hpp"
#include "../include/core/GameManager.hpp"
#include <memory>
#include <vector>
// untuk test doang 
int main(){
    auto game = std::make_unique<GameManager>(100, 0);  
    game->startGame();
    return 0;
}
