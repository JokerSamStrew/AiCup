#include "MyUnit.hpp"
#include "Utils.hpp"

MyUnit::MyUnit(const model::Constants& constants)
    : _constants(constants)
{
}

void MyUnit::setGame(const model::Game* game, DebugInterface* debugInterface)
{
    _game = game;
    auto units = getUnits(*_game);
    _my_unit = units.first;
    _other_units = units.second;
    _debugInterface = debugInterface;
    highlightUnits(_other_units, _debugInterface);
}

void MyUnit::AddNoVisibleUnitsAction()
{
    if (!actions.empty())
        return;

    auto currentCenterVec = getNextZoneCenter(*_game, *_my_unit);
    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    std::optional<std::shared_ptr<model::ActionOrder>> action;
    model::UnitOrder order(currentCenterVec, currentDirection, action);
    actions.insert({_my_unit->id, order});
}

double MyUnit::countWeaponRange()
{
    if (_my_unit->weapon.has_value())
    {
        auto speed = _constants.weapons[_my_unit->weapon.value()].projectileSpeed;
        auto time = _constants.weapons[_my_unit->weapon.value()].projectileLifeTime;
        return speed * time;
    }

    return 0.0;
}

void MyUnit::AddFightClosestAction()
{
    if (_other_units.empty())
        return;
    
    auto weapon_range = countWeaponRange();
    if (weapon_range == 0.0)
        return;


    auto other_unit = closestUnit(_my_unit->position, _other_units);

    //if (countRange(_my_unit->position, other_unit->position) > weapon_range)
        //return;

    std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
    std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);
    auto currentCenterVec = getNextZoneCenter(*_game, *_my_unit);
    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
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
