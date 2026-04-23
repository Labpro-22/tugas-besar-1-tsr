#pragma once

#include <string>
#include "SaveData.hpp" 
#include "ConfigData.hpp"

class IOManager {
public:
    IOManager() = default;
    ~IOManager() = default;

    GameSaveData loadGameData(const std::string& filepath);
    void saveGameData(const GameSaveData& data, const std::string& filepath);
    FullConfigData loadAllConfigs(const std::string& configDirectoryPath);

private:
    void parseMiscConfig(const std::string& filepath, FullConfigData& config);
    void parseTaxConfig(const std::string& filepath, FullConfigData& config);
    void parseSpecialConfig(const std::string& filepath, FullConfigData& config);
    void parseRailroadConfig(const std::string& filepath, FullConfigData& config);
    void parseUtilityConfig(const std::string& filepath, FullConfigData& config);
    void parsePropertyConfig(const std::string& filepath, FullConfigData& config);
    void parseActionConfig(const std::string& filepath, FullConfigData& config);
};