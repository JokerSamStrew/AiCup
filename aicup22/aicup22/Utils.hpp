#include <utility>
#include <model/Unit.hpp>
#include <model/Game.hpp>
#include "DebugInterface.hpp"

std::pair<const model::Unit*, std::vector<const model::Unit*>> getUnits(const model::Game &game);

const model::Unit* closestUnit(const model::Vec2& point, const std::vector<const model::Unit*>& units);

double countRange(const model::Vec2& a, const model::Vec2& b);

void highlightUnits(const std::vector<const model::Unit*>& units, DebugInterface *debugInterface);

model::Vec2 getNextZoneCenter(const model::Game &game, const model::Unit& unit);
