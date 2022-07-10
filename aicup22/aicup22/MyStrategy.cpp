#include "MyStrategy.hpp"
#include <exception>
#include "debugging/Color.hpp"
#include <utility>
#include <vector>
#include <limits>

MyStrategy::MyStrategy(const model::Constants &constants) {}

model::Vec2 getNextZoneCenterVec(const model::Game &game, const model::Unit& unit)
{
    return model::Vec2( game.zone.nextCenter.x - unit.position.x, game.zone.nextCenter.y - unit.position.y);
}

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

model::Order MyStrategy::getOrder(const model::Game &game, DebugInterface *debugInterface)
{
    auto units = getUnits(game);
    std::unordered_map<int, model::UnitOrder> actions;
    if (units.first == nullptr)
        return model::Order(actions);

    if (debugInterface != nullptr)
    {
        for (auto &unit : units.second)
        {
            debugInterface->addCircle(unit->position, 2, debugging::Color(0.0, 0.5, 0.0, 0.5));
        }
    }

    const auto& my_unit = units.first;
    if (units.second.empty())
    {
        auto currentCenterVec = getNextZoneCenterVec(game, *my_unit);
        auto currentDirection = model::Vec2(-my_unit->direction.y, my_unit->direction.x);
        std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
        std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);
        model::UnitOrder order(currentCenterVec, currentDirection, action);
        actions.insert({my_unit->id, order});
    }
    else
    {
        auto currentCenterVec = getNextZoneCenterVec(game, *my_unit);
        auto currentDirection = model::Vec2(-my_unit->direction.y, my_unit->direction.x);
        std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
        std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);
        const auto& other_unit = closestUnit(units.first->position, units.second);
        model::UnitOrder order (currentCenterVec, {other_unit->position.x - my_unit->position.x, other_unit->position.y - my_unit->position.y}, action);
        actions.insert({my_unit->id, order});
    }

    return model::Order(actions);
}

void MyStrategy::debugUpdate(int displayedTicks, DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
