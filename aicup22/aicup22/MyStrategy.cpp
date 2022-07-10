#include "MyStrategy.hpp"
#include <exception>

MyStrategy::MyStrategy(const model::Constants &constants) {}

model::Vec2 getNextZoneCenterVec(const model::Game &game, const model::Unit& unit)
{
    return model::Vec2( game.zone.nextCenter.x - unit.position.x, game.zone.nextCenter.y - unit.position.y);
}

model::Order MyStrategy::getOrder(const model::Game &game, DebugInterface *debugInterface)
{
    std::unordered_map<int, model::UnitOrder> actions;
    for (auto &unit : game.units)
    {
        if (unit.playerId != game.myId)
            continue;

        std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
        std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);
        auto currentCenterVec = getNextZoneCenterVec(game, unit);

        model::UnitOrder order(currentCenterVec, {-unit.direction.y, unit.direction.x}, action);
        actions.insert({unit.id, order});
    }
    return model::Order(actions);
}

void MyStrategy::debugUpdate(DebugInterface &debugInterface) {}

void MyStrategy::finish() {}
