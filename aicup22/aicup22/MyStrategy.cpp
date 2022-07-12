#include "MyStrategy.hpp"
#include <exception>
#include "debugging/Color.hpp"
#include <utility>
#include <vector>
#include <Stream.hpp>
#include "Utils.hpp"

MyStrategy::MyStrategy(const model::Constants &constants) {}


model::Order MyStrategy::getOrder(const model::Game &game, DebugInterface *debugInterface)
{
    auto units = getUnits(game);
    std::unordered_map<int, model::UnitOrder> actions;
    if (units.first == nullptr)
        return model::Order(actions);

    highlightUnits(units.second, debugInterface);

    const auto& my_unit = units.first;
    if (units.second.empty())
    {
        auto currentCenterVec = getNextZoneCenter(game, *my_unit);
        auto currentDirection = model::Vec2(-my_unit->direction.y, my_unit->direction.x);
        std::optional<std::shared_ptr<model::ActionOrder>> action;
        model::UnitOrder order(currentCenterVec, currentDirection, action);
        actions.insert({my_unit->id, order});
    }
    else
    {
        auto currentCenterVec = getNextZoneCenter(game, *my_unit);
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
