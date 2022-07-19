#include "MyUnit.hpp"
#include "Utils.hpp"

MyUnit::MyUnit(const model::Constants& constants)
    : _constants(constants)
{
}

void MyUnit::setLoot()
{
    _shield_potions.clear();
    _weapons.clear();
    _ammo.clear();

    for (auto l : _game->loot)
    {

        if (!isVecInsideCircle(_my_unit->position, PICKUP_RANGE, l.position))
            continue;

        if (!isVecInsideCircle(_game->zone.currentCenter, _game->zone.currentRadius * EDGE_COEF, l.position))
            continue;

        if (_second_unit.has_value() && isVecInsideCircle(_second_unit->position, PICKUP_RANGE, l.position))
            continue;

        if (_second_unit.has_value() && isVecInsideCircle(_game->zone.nextCenter, _game->zone.nextRadius * CLOSE_TO_REACH, l.position))
            continue;

        if (const auto* item = std::get_if<model::ShieldPotions>(&l.item))
        {
            _shield_potions.push_back(l);
        }
        else if (const auto* item = std::get_if<model::Weapon>(&l.item))
        {
            _weapons.push_back(l);
        }
        else if (const auto* item = std::get_if<model::Ammo>(&l.item))
        {
            _ammo.push_back(l);
        }
    }

    highlightLoot(_shield_potions, _debugInterface, 1);
    highlightLoot(_ammo, _debugInterface, 2);
    highlightLoot(_weapons, _debugInterface, 3);
}

void MyUnit::setSound()
{
    auto _current_sounds = _sounds;
    _sounds.clear();
    int max_size = 5;

    for (const auto& s : _game->sounds)
    {
        if (isVecInsideCircle(_my_unit->position, SOUND_RANGE, s.position))
        {
            _sounds.push_back(s);
        }
    }

    for (const auto& s : _current_sounds)
    {
        if (isVecInsideCircle(_my_unit->position, SOUND_RANGE, s.position))
        {
            max_size--;
            _sounds.push_back(s);
        }

        if (max_size == 0)
            break;
    }


    highlightSounds(_sounds, _debugInterface);
}

void MyUnit::setGame(const model::Game* game, 
        const model::Unit& my_unit, 
        const std::vector<model::Unit>& other_units, 
        const std::vector<model::Obstacle>& obstacles, 
        DebugInterface* debugInterface)
{
    _available_obs = obstacles;
    _game = game;
    _my_unit = my_unit;
    _prev_hp_level = _my_unit->health;
    _prev_shield_level = _my_unit->shield;
    ID = my_unit.id;
    
    _other_units = other_units;

    _debugInterface = debugInterface;

    setLoot();
    setSound();
    highlightUnits(_other_units, _debugInterface);
}

void MyUnit::AddNoVisibleUnitsAction()
{
    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    std::optional<model::ActionOrder> action;
    model::UnitOrder order(currentMoveVec(), currentDirection, action);
    _actions.push_back({0.0, order});
}

model::Vec2 MyUnit::currentMoveVec()
{
    auto obs = getObstaclesInsideCircle(_available_obs, _my_unit->position, MOVE_RANGE);
    auto center_point_radius = _game->zone.nextRadius * CLOSE_TO_REACH;
    auto center = _game->zone.nextCenter;

    if (_second_unit.has_value())
        obs = removeObstaclesInsideCircle(obs, _second_unit->position, MOVE_RANGE);

    if (!isVecInsideCircle(center, center_point_radius, _my_unit->position))
    {
        obs = removeObstaclesInsideCircle(obs, center, center_point_radius);
        if (obs.empty())
            return getNextZoneCenter(*_game, *_my_unit);
    }

    if (_prev_prev_obs.has_value())
        obs = removeObstaclesInsideCircle(obs, _prev_prev_obs->position, _prev_prev_obs->radius);
    if (obs.empty())
        return vecDiff(_prev_prev_obs->position, _my_unit->position);

    if (_prev_obs.has_value())
        obs = removeObstaclesInsideCircle(obs, _prev_obs->position, _prev_obs->radius);
    if (obs.empty())
        return vecDiff(_prev_obs->position, _my_unit->position);


    highlightObstacles(obs, _debugInterface);

    if (_current_obs.has_value() 
        && !isVecInsideCircle(_current_obs->position, _current_obs->radius + NEAR_OBS, _my_unit->position)) 
    {
        _prev_obs = _current_obs;
        highlightObstacle(*_current_obs, _debugInterface);
        return vecDiff(_current_obs->position, _my_unit->position);
    }

    auto closest_obs = closestObstacle(center, obs).value(); 
    _prev_prev_obs = _prev_obs;
    _current_obs = closest_obs;

    return vecDiff(_current_obs->position, _my_unit->position);
}


double MyUnit::countWeaponRange()
{
    if (!_my_unit->weapon.has_value())
        return 0.0;

    auto weapon_properties = _constants.weapons[_my_unit->weapon.value()];
    auto speed = weapon_properties.projectileSpeed;
    auto time = weapon_properties.projectileLifeTime;
    return speed * time * WEAPON_COEF;
}

void MyUnit::AddFightClosestAction()
{
    if (_other_units.empty())
        return;
    
    auto weapon_range = countWeaponRange();
    if (weapon_range == 0.0)
        return;

    if (_my_unit->ammo[_my_unit->weapon.value()] == 0)
        return;

    auto other_unit = closestUnit(_my_unit->position, _other_units);

    auto unit_range = countRange(_my_unit->position, other_unit->position);
    if (unit_range > weapon_range)
        return;

    auto weapon_properties = _constants.weapons[_my_unit->weapon.value()];
    auto speed = weapon_properties.projectileSpeed;
    auto time = unit_range / speed; 
    if (_my_unit->aim < 1)
        time += weapon_properties.aimTime;
    time *= WEAPON_COEF;

    auto direction = vecDiff(other_unit->position, _my_unit->position);
    direction = vecSum(direction, {other_unit->velocity.x * time, other_unit->velocity.y * time});

    drawDirectionArc(*_my_unit, weapon_range, _debugInterface); 

    // auto priority = 1 - ( other_unit->shield / _constants.maxShield + other_unit->health / _constants.unitHealth ) / 2 + 0.5;
    auto priority = 1.0;
    auto aim = model::Aim(true);
    model::UnitOrder order (currentMoveVec(), direction, std::make_optional<model::Aim>(aim));
    auto aim_pos = vecSum(_my_unit->position, model::Vec2( _my_unit->direction.x * unit_range, _my_unit->direction.y * unit_range ));
    auto obs = getObstaclesInsideCircle(_available_obs, _my_unit->position, unit_range + 0.6);
    if (isAimInObs(_my_unit->position, aim_pos, obs))
    {
        priority = 1 - ( other_unit->shield / _constants.maxShield + other_unit->health / _constants.unitHealth ) / 2;
        order.action = model::Aim(false);
    }
    else
        drawLine(_my_unit->position, aim_pos, _debugInterface);

    _actions.push_back({priority, order});
}

std::optional<model::UnitOrder> MyUnit::GetOrder()
{
    ClearActions();
    auto remainingSpawnTime =  _my_unit->remainingSpawnTime.value_or(-1);

    if (remainingSpawnTime != -1) {
        drawText(vecSum(_my_unit->position, {0.5, 0.5}), std::to_string(remainingSpawnTime), _debugInterface);
        AddFightClosestAction();
        AddUseShieldAction();
    }

    AddGetShieldAction();
    AddGetAmmoAction();
    AddGetWeaponAction();
    AddNoVisibleUnitsAction();

    if (_actions.empty())
        return std::optional<model::UnitOrder>();

    auto max_priority = _actions[0].first;
    std::optional<model::UnitOrder> action = _actions[0].second;
    for (const auto& a : _actions)
    {
        if (max_priority < a.first)
        {
            max_priority = a.first;
            action = a.second;
        }
    }
    
    drawText(vecSum(_my_unit->position, { 0.5, 1.5 }), std::to_string(max_priority), _debugInterface);
    drawText(vecSum(_my_unit->position, { 0.5, 1 }), action->toString(), _debugInterface);
    return action;
}

void MyUnit::AddGetShieldAction()
{
    if (_my_unit->shieldPotions == _constants.maxShieldPotionsInInventory)
        return;

    auto closest_shield = closestLoot(_my_unit->position, _shield_potions);
    if (!closest_shield.has_value())
        return;

    auto priority = 1.0 - (double) _my_unit->shieldPotions / _constants.maxShieldPotionsInInventory;
    auto loot_pos = closest_shield.value().position;
    auto diff_vec = vecDiff(loot_pos, _my_unit->position);
    model::UnitOrder order ({diff_vec.x * MOVE_COEF, diff_vec.y * MOVE_COEF}, diff_vec, std::make_optional<model::Pickup>(model::Pickup(closest_shield->id)));
    _actions.push_back({priority, order});
}

void MyUnit::AddGetWeaponAction()
{
    std::vector<model::Loot> prior_weapons;
    drawText(_my_unit->position, std::to_string(_my_unit->weapon.value_or(-1)), _debugInterface);

    for (auto wp : _weapons){
        auto weapon = std::get<model::Weapon>(wp.item);
        drawText(wp.position, std::to_string(weapon.typeIndex), _debugInterface);

        if (_my_unit->ammo[weapon.typeIndex] == 0)
            continue;

        if (weapon.typeIndex > _my_unit->weapon.value_or(-1))
            prior_weapons.push_back(wp);
    }

    auto closest_weapon = closestLoot(_my_unit->position, prior_weapons);
    if (!closest_weapon.has_value())
        return;
    
    auto weapon = std::get<model::Weapon>(closest_weapon->item);
    auto priority = 1.0;

    if (_my_unit->weapon.has_value())
        priority = (double) _my_unit->ammo[weapon.typeIndex] / _constants.weapons[weapon.typeIndex].maxInventoryAmmo;

    auto loot_pos = closest_weapon.value().position;
    auto diff_vec = vecDiff(loot_pos, _my_unit->position);
    model::UnitOrder order (diff_vec, diff_vec, std::make_optional<model::Pickup>(model::Pickup(closest_weapon->id)));
    _actions.push_back({priority, order});
}

void MyUnit::AddUseShieldAction()
{
    if (_my_unit->shieldPotions == 0)
        return;

    if (_my_unit->shield == _constants.maxShield )
        return;

    auto priority = 1.0 - ( _my_unit->shield / _constants.maxShield + _my_unit->health / _constants.unitHealth ) / 2;
    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    model::UnitOrder order (currentMoveVec(), currentDirection, std::make_optional<model::UseShieldPotion>(model::UseShieldPotion()));
    _actions.push_back({priority, order});
}

void MyUnit::AddGetAmmoAction()
{
    drawText(_my_unit->position, std::to_string(_my_unit->weapon.value_or(-1)), _debugInterface);
    std::vector<model::Loot> prior_ammo;

    for (auto a : _ammo){
        auto ammo = std::get<model::Ammo>(a.item);
        drawText(a.position, std::to_string(ammo.weaponTypeIndex), _debugInterface);
        if (_my_unit->ammo[ammo.weaponTypeIndex] == _constants.weapons[ammo.weaponTypeIndex].maxInventoryAmmo )
            continue;

        prior_ammo.push_back(a);
    }

    auto closest_ammo = closestLoot(_my_unit->position, prior_ammo);
    if (!closest_ammo.has_value())
        return;

    auto ammo = std::get<model::Ammo>(closest_ammo->item);
    auto priority = 1.0 - (double) _my_unit->ammo[ammo.weaponTypeIndex] / _constants.weapons[ammo.weaponTypeIndex].maxInventoryAmmo;

    auto loot_pos = closest_ammo.value().position;
    auto diff_vec = vecDiff(loot_pos, _my_unit->position);
    model::UnitOrder order (diff_vec, diff_vec, std::make_optional<model::Pickup>(model::Pickup(closest_ammo->id)));
    _actions.push_back({priority, order});
}


void MyUnit::setSecondUnit(const std::optional<model::Unit>& unit)
{
    if (unit.has_value())
        _second_unit = unit.value();
}

void MyUnit::ClearActions() 
{ 
    _actions.clear();
}

