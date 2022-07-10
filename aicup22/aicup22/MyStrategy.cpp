#include "MyStrategy.hpp"
#include <exception>
#include "debugging/Color.hpp"
#include <utility>
#include <vector>

MyStrategy::MyStrategy(const model::Constants &constants) {}

model::Vec2 getNextZoneCenterVec(const model::Game &game, const model::Unit& unit)
{
    return model::Vec2( game.zone.nextCenter.x - unit.position.x, game.zone.nextCenter.y - unit.position.y);
}

std::pair<const model::Unit*, std::vector<const model::Unit*>> getUnits(const model::Game &game)
{
    std::vector<const model::Unit*> other_units;
    const model::Unit* my_unit;
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

model::Order MyStrategy::getOrder(const model::Game &game, DebugInterface *debugInterface)
{
    std::unordered_map<int, model::UnitOrder> actions;
    const auto& units = getUnits(game);
    for (auto &unit : units.second)
    {
        debugInterface->addCircle(unit->position, 2, debugging::Color(0.0, 0.5, 0.0, 0.5));
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
        std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
        std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);
        auto currentCenterVec = getNextZoneCenterVec(game, *my_unit);
        auto currentDirection = model::Vec2(-my_unit->direction.y, my_unit->direction.x);
        const auto& other_unit = units.second[0];
        model::UnitOrder order (currentCenterVec, {other_unit->position.x - my_unit->position.x, other_unit->position.y - my_unit->position.y}, action);
        actions.insert({my_unit->id, order});
    }

    return model::Order(actions);
}

void MyStrategy::debugUpdate(DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
