#pragma once
#include "../../include/models/Card.hpp"
#include "../../include/models/Effect.hpp"
#include <memory>
#include <string>

class SkillCard : public Card {
protected:
    std::string name;

public:
    SkillCard(std::string name);
    virtual ~SkillCard() = default;
    
    std::string getName() const;

    virtual bool isMoveCard() const { return false; }
    virtual bool isDiscountCard() const { return false; }
    virtual bool isShieldCard() const { return false; }
    virtual bool isTeleportCard() const { return false; }
    virtual bool isLassoCard() const { return false; }
    virtual bool isDemolitionCard() const { return false; }

    void onDraw(Player& p) override;
    virtual void useEffect(Player& p) = 0;
    virtual void loadFromSave(float value, int remainingDuration);

    virtual std::string toSaveFormat() const;
};

class MoveSkillCard : public SkillCard {
private:
    int step;
public:
    MoveSkillCard(std::string name);
    
    bool isMoveCard() const override { return true; }
    
    int getStep() const;

    void onDraw(Player& p) override;
    void useEffect(Player& p) override;
    void loadFromSave(float value, int remainingDuration) override;
    std::string toSaveFormat() const override;
};

class DiscountSkillCard : public SkillCard {
private:
    float percentage;
public:
    DiscountSkillCard(std::string name);
    
    bool isDiscountCard() const override { return true; }
    
    float getPercentage() const;

    void onDraw(Player& p) override;
    void useEffect(Player& p) override;
    void loadFromSave(float value, int remainingDuration) override;
    std::string toSaveFormat() const override;
};

class ShieldSkillCard : public SkillCard {
public:
    ShieldSkillCard(std::string name);
    
    bool isShieldCard() const override { return true; }
    void useEffect(Player& p) override;
};

class TeleportSkillCard : public SkillCard {
public:
    TeleportSkillCard(std::string name);
    
    bool isTeleportCard() const override { return true; }
    void useEffect(Player& p) override;
};

class LassoSkillCard : public SkillCard {
public:
    LassoSkillCard(std::string name);
    
    bool isLassoCard() const override { return true; }
    void useEffect(Player& p) override;
};

class DemolitionSkillCard : public SkillCard {
public:
    DemolitionSkillCard(std::string name);
    
    bool isDemolitionCard() const override { return true; }
    void useEffect(Player& p) override;
};