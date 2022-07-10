#include "MyStrategy.hpp"
#include <exception>
#include "debugging/Color.hpp"

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
        auto currentDirection = model::Vec2(0, 0);
        if (unit.playerId != game.myId)
        {
            debugInterface->addCircle(unit.position, 2, debugging::Color(0.0, 0.5, 0.0, 0.5));
            currentDirection = model::Vec2(unit.position.x, unit.position.y);
            continue;
        } 
        else
        {
            currentDirection = model::Vec2(-unit.direction.y, unit.direction.x);
        }
            

        std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
        std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);
        auto currentCenterVec = getNextZoneCenterVec(game, unit);

        model::UnitOrder order(currentCenterVec, currentDirection, action);
        actions.insert({unit.id, order});
    }
    return model::Order(actions);
}

void MyStrategy::debugUpdate(DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
