#pragma once
#include "Saveable.hpp"
#include "GameException.hpp" // Ensure this is included here now
#include <stack>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <sstream>

template <class T>
class Deck : public Saveable {
private:
    std::vector<T> cards;
    std::vector<T> discard;

public:
    T drawCard() {
        if (cards.empty()) {
            throw GameplayException("Deck: Cannot draw from an empty deck!");
        }
        
        T temp = std::move(cards.back()); 
        cards.pop_back();
        return temp;
    }

    void shuffle() {
        if (discard.empty()) {
            throw GameplayException("Deck: Nothing to shuffle!");
        }
        
        std::random_device rd;
        std::mt19937 g(rd());
        
        std::shuffle(discard.begin(), discard.end(), g);
        
        for (auto& card : discard) {
            cards.push(std::move(card));
        }
        
        discard.clear();
    }
    void addToDiscard(T card) {
        discard.push_back(std::move(card));
    }
    
    std::string toSaveFormat() const override {
        std::ostringstream out;
        out << cards.size()+discard.size() << "\n";
        for(const auto& card: cards){
            out << card->getName() << "\n";
        }
        for(const auto& card: discard){
            out << "~" <<card->getName() << "\n";
        }
        return out.str();
    }
};