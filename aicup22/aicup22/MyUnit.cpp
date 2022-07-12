#include "MyUnit.hpp"
#include "Utils.hpp"

MyUnit::MyUnit()
{
}

void MyUnit::setGame(const model::Game* game, DebugInterface* debugInterface)
{
    _game = game;
    auto units = getUnits(*_game);
    _my_unit = units.first;
    _other_units = units.second;
    if (!_other_units.empty())
        highlightUnits(_other_units, debugInterface);
}

void MyUnit::AddNoVisibleUnitsAction()
{
    if (!_other_units.empty())
        return;

    auto currentCenterVec = getNextZoneCenter(*_game, *_my_unit);
    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    std::optional<std::shared_ptr<model::ActionOrder>> action;
    model::UnitOrder order(currentCenterVec, currentDirection, action);
    actions.insert({_my_unit->id, order});
}

void MyUnit::AddFightClosestAction()
{
    if (_other_units.empty())
        return;

    auto currentCenterVec = getNextZoneCenter(*_game, *_my_unit);
    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
    std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);

    const auto& other_unit = closestUnit(_my_unit->position, _other_units);
    model::UnitOrder order (currentCenterVec, vecDiff(other_unit->position, _my_unit->position), action);
    actions.insert({_my_unit->id, order});
}

model::Order MyUnit::CreateOrder()
{
    return model::Order(actions);
}


void MyUnit::ClearActions() 
{ 
    actions.clear();
}
