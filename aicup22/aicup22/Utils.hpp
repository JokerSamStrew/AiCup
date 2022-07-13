#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <utility>
#include <model/Unit.hpp>
#include <model/Game.hpp>
#include "DebugInterface.hpp"

std::pair<const model::Unit*, std::vector<const model::Unit*>> getUnits(const model::Game &game);

const model::Unit* closestUnit(const model::Vec2& point, const std::vector<const model::Unit*>& units);

double countRange(const model::Vec2& a, const model::Vec2& b);

void highlightUnits(const std::vector<const model::Unit*>& units, DebugInterface *debugInterface);

model::Vec2 getNextZoneCenter(const model::Game &game, const model::Unit& unit);

model::Vec2 vecDiff(const model::Vec2& a, const model::Vec2& b);

void drawDirectionArc(const model::Unit& unit, double lineSize, DebugInterface *debugInterface);

model::Vec2 vecSum(const model::Vec2& a, const model::Vec2& b);

void drawText(const model::Vec2& pos, const std::string& text, DebugInterface *debugInterface);

#endif
