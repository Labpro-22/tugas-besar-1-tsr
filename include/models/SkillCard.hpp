#pragma once
#include "../../include/models/Card.hpp"
#include "../../include/models/Effect.hpp"
#include <memory>

class Player;
class Board;
class TileVisitor;

class SkillCard: public Card
{
private:
    /* data */
public:
    void onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
    virtual void useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) = 0;
};

class MoveSkillCard: public SkillCard
{
private:
    int step;
public:
    void onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
    void useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
};

class DiscountSkillCard: public SkillCard
{
private:
    float percentage;
public:
    void onDraw(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
    void useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
};

class ShieldSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
};

class TeleportSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
};

class LassoSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
};

class DemolitionSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all, TileVisitor& visitor) override;
};
