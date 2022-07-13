#include "MyUnit.hpp"
#include "Utils.hpp"

MyUnit::MyUnit(const model::Constants& constants)
    : _constants(constants)
{
    for (auto obs : _constants.obstacles){
        if (!obs.canShootThrough)
            _available_obs.push_back(obs);
    }
}

void MyUnit::setGame(const model::Game* game, DebugInterface* debugInterface)
{
    _game = game;

    auto rad = _game->zone.currentRadius;
    auto center = _game->zone.currentCenter;
    _available_obs = getObstaclesInsideCircle(_available_obs, center, rad - 3.0);

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

    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    std::optional<std::shared_ptr<model::ActionOrder>> action;
    model::UnitOrder order(currentMoveVec(), currentDirection, action);
    actions.insert({_my_unit->id, order});
}

model::Vec2 MyUnit::currentMoveVec()
{
    auto obs = getObstaclesInsideCircle(_available_obs, _my_unit->position, MOVE_RANGE);
    highlightObstacles(obs, _debugInterface);
    if (obs.empty())
        return getNextZoneCenter(*_game, *_my_unit);

    auto center = _game->zone.nextCenter;
    auto closest_obs = closestObstacle(center, obs).value(); 
    highlightObstacle(closest_obs, _debugInterface);

    return vecDiff(closest_obs.position, _my_unit->position);
}


double MyUnit::countWeaponRange()
{
    if (_my_unit->weapon.has_value())
    {
        auto weapon_properties = _constants.weapons[_my_unit->weapon.value()];
        auto speed = weapon_properties.projectileSpeed;
        auto time = weapon_properties.projectileLifeTime;
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

    if (countRange(_my_unit->position, other_unit->position) > weapon_range)
        return;

    std::shared_ptr<model::ActionOrder::Aim> aim = std::make_shared<model::ActionOrder::Aim>(true);
    std::optional<std::shared_ptr<model::ActionOrder>> action = std::make_optional(aim);
    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);

    drawDirectionArc(*_my_unit, weapon_range, _debugInterface); 
    model::UnitOrder order (currentMoveVec(), vecDiff(other_unit->position, _my_unit->position), action);
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
