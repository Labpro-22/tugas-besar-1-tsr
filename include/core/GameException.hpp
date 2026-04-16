#pragma once
#include <exception>
#include <string>

class GameException : public std::exception {
private:
    std::string message;

public:
    GameException(const std::string& msg);
    virtual const char* what() const noexcept override;
};

class InputException : public GameException {
public:
    explicit InputException(const std::string& msg);
};

class ConfigException : public GameException {
public:
    explicit ConfigException(const std::string& msg);
};

class SaveLoadException : public GameException {
public:
    explicit SaveLoadException(const std::string& msg);
};

class GameplayException : public GameException {
public:
    explicit GameplayException(const std::string& msg);
};