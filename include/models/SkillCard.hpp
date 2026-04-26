#pragma once
#include "../../include/models/Card.hpp"
#include "../../include/models/Effect.hpp"
#include <memory>
#include <string>


class SkillCard: public Card
{
protected:
    std::string name;
public:
    SkillCard(std::string name);
    void onDraw(Player& p) override;
    virtual void useEffect(Player& p) = 0;

    virtual std::string toSaveFormat() const;
};

class MoveSkillCard: public SkillCard
{
private:
    int step;
public:
    MoveSkillCard(std::string name);
    void onDraw(Player& p) override;
    void useEffect(Player& p) override;
    std::string toSaveFormat() const override;
};

class DiscountSkillCard: public SkillCard
{
private:
    float percentage;
public:
    DiscountSkillCard(std::string name);
    void onDraw(Player& p) override;
    void useEffect(Player& p) override;
    std::string toSaveFormat() const override;
};

class ShieldSkillCard: public SkillCard
{
private:
    /* data */
public:
    ShieldSkillCard(std::string name);
    void useEffect(Player& p) override;
};

class TeleportSkillCard: public SkillCard
{
private:
    /* data */
public:
    TeleportSkillCard(std::string name);
    void useEffect(Player& p) override;
};

class LassoSkillCard: public SkillCard
{
private:
    /* data */
public:
    LassoSkillCard(std::string name);
    void useEffect(Player& p) override;
};

class DemolitionSkillCard: public SkillCard
{
private:
    /* data */
public:
    DemolitionSkillCard(std::string name);
    void useEffect(Player& p) override;
};