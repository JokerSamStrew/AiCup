#include "Utils.hpp"
#include <limits>
#include <cmath>

std::pair<const model::Unit*, std::vector<const model::Unit*>> getUnits(const model::Game &game)
{
    std::vector<const model::Unit*> other_units;
    const model::Unit* my_unit = nullptr;
    for (auto &unit : game.units)
    {
        if (unit.playerId == game.myId)
        {
            my_unit = &unit;
            continue;
        }
        
        other_units.push_back(&unit);
    }

    return std::pair(my_unit, other_units);
}

double countRange(const model::Vec2& a, const model::Vec2& b)
{
   return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

const model::Unit* closestUnit(const model::Vec2& point, const std::vector<const model::Unit*>& units)
{
   double minDistance = std::numeric_limits<double>::max(); 
   const model::Unit* closestUnit = nullptr;
   for (auto unit : units){
       double distance = (point.x - unit->position.x)*(point.x - unit->position.x) + (point.x - unit->position.x)*(point.x - unit->position.x);
       if (distance < minDistance)
       {
          minDistance = distance;
          closestUnit = unit;
       }
   }

   return closestUnit;
}


void highlightUnits(const std::vector<const model::Unit*>& units, DebugInterface *debugInterface)
{
    if (debugInterface == nullptr)
        return;

    for (const auto &unit : units)
    {
        debugInterface->addRing(unit->position, 2, 0.5, debugging::Color(0.0, 0.5, 0.0, 0.5));
    }
}

void drawDirectionArc(const model::Unit& unit, double lineSize, DebugInterface *debugInterface)
{
   if (debugInterface == nullptr)
       return;

   auto end_position = model::Vec2(
           unit.position.x + lineSize * unit.direction.x,
           unit.position.y + lineSize * unit.direction.y );
   debugInterface->addPolyLine({unit.position, end_position}, 0.5, debugging::Color(0.0, 0.0, 1.0, 0.5));
}

model::Vec2 getNextZoneCenter(const model::Game &game, const model::Unit& unit)
{
    return model::Vec2( game.zone.nextCenter.x - unit.position.x, game.zone.nextCenter.y - unit.position.y);
}

model::Vec2 vecDiff(const model::Vec2& a, const model::Vec2& b)
{
    return {a.x - b.x, a.y - b.y};
}
