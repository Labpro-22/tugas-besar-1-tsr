#pragma once

#include <string>
#include <vector>
#include <iostream>

class Player;
class Board;
class PropertyTile;
class LogEntry;
enum class GameState;

class ViewGame {
public:
    ViewGame() = default;
    ~ViewGame() = default;

    // Render stubs (no-op dulu supaya compile)
    void renderBoard(const Board& board) const {
        (void)board;
    }

    void renderPlayerStats(const Player& player) const {
        (void)player;
    }

    void renderPropertyList(const std::vector<PropertyTile*>& properties) const {
        (void)properties;
    }

    void renderMenu(GameState currentState, const std::string& playerName) const {
        (void)currentState;
        (void)playerName;
    }

    void showMessage(const std::string& message) const {
        std::cout << message << "\n";
    }

    void renderLogs(const std::vector<LogEntry>& logs) const {
        (void)logs;
    }

    // Input helpers
    std::string getInput() const {
        std::string s;
        std::getline(std::cin >> std::ws, s);
        return s;
    }

    bool getConfirmation() const {
        return getYesNo();
    }

    int getNumericInput() const {
        int x = 0;
        std::cin >> x;
        return x;
    }

    // Dipanggil sebagai ViewGame::getYesNo()
    static bool getYesNo() {
        std::string s;
        std::getline(std::cin >> std::ws, s);
        if (s.empty()) return false;
        char c = static_cast<char>(std::tolower(static_cast<unsigned char>(s[0])));
        return c == 'y';
    }
};