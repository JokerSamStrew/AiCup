#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <utility>
#include <model/Unit.hpp>
#include <model/Game.hpp>
#include <model/Obstacle.hpp>
#include <DebugInterface.hpp>

std::pair<std::optional<model::Unit>, std::vector<model::Unit>> getUnits(const model::Game &game);

std::optional<model::Unit> closestUnit(const model::Vec2& point, const std::vector<model::Unit>& units);

std::optional<model::Unit> closestUnit(const model::Vec2& point, const std::vector<model::Unit>& units);

std::optional<model::Obstacle> closestObstacle(const model::Vec2& point, const std::vector<model::Obstacle>& obstacles);

double countRange(const model::Vec2& a, const model::Vec2& b);

void highlightUnits(std::vector<model::Unit>& units, DebugInterface *debugInterface);

void highlightLoot(const std::vector<model::Loot>& loot, DebugInterface *debugInterface, int type);

model::Vec2 getNextZoneCenter(const model::Game &game, const model::Unit& unit);

model::Vec2 getCurrentZoneCenter(const model::Game &game, const model::Unit& unit);

model::Vec2 vecDiff(const model::Vec2& a, const model::Vec2& b);

void drawDirectionArc(const model::Unit& unit, double lineSize, DebugInterface *debugInterface);

model::Vec2 vecSum(const model::Vec2& a, const model::Vec2& b);

void drawText(const model::Vec2& pos, const std::string& text, DebugInterface *debugInterface);

void highlightObstacles(const std::vector<model::Obstacle>& obstacles, DebugInterface *debugInterface);

bool isVecInsideCircle(const model::Vec2& circle, double rad, const model::Vec2& pos);

std::vector<model::Obstacle> getObstaclesInsideCircle(const std::vector<model::Obstacle>& obstacles, const model::Vec2& circle_pos, double rad);

std::vector<model::Obstacle> removeObstaclesInsideCircle(const std::vector<model::Obstacle>& obstacles, const model::Vec2& circle_pos, double rad);

void highlightObstacle(const model::Obstacle& obstacle, DebugInterface *debugInterface);

std::optional<model::Loot> closestLoot(const model::Vec2& point, const std::vector<model::Loot>& loot);

#endif
