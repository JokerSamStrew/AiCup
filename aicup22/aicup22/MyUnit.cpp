#include "MyUnit.hpp"
#include "Utils.hpp"
#include <memory>

MyUnit::MyUnit(const model::Constants& constants)
    : _constants(constants)
{
    for (auto obs : _constants.obstacles){
        if (!obs.canShootThrough)
            _available_obs.push_back(obs);
    }
}

void MyUnit::setLoot()
{

    _shield_potions.clear();
    _weapons.clear();
    _ammo.clear();

    for (auto l : _game->loot)
    {

        if (!isVecInsideCircle(_my_unit->position, MOVE_RANGE, l.position))
            continue;

        if (!isVecInsideCircle(_game->zone.currentCenter, _game->zone.currentRadius * EDGE_COEF, l.position))
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

void MyUnit::setGame(const model::Game* game, DebugInterface* debugInterface)
{
    _game = game;

    auto rad = _game->zone.currentRadius * EDGE_COEF;
    auto center = _game->zone.currentCenter;
    _available_obs = getObstaclesInsideCircle(_available_obs, center, rad);

    auto units = getUnits(*_game);

    _my_unit = units.first;
    _other_units = units.second;
    _debugInterface = debugInterface;

    setLoot();
    highlightUnits(_other_units, _debugInterface);
}

void MyUnit::AddNoVisibleUnitsAction()
{
    if (!actions.empty())
        return;

    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    std::optional<model::ActionOrder> action;
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

    if (_my_unit->ammo[_my_unit->weapon.value()] == 0)
        return;

    auto other_unit = closestUnit(_my_unit->position, _other_units);

    if (countRange(_my_unit->position, other_unit->position) > weapon_range)
        return;

    auto aim = model::Aim(true);

    drawDirectionArc(*_my_unit, weapon_range, _debugInterface); 
    model::UnitOrder order (currentMoveVec(), vecDiff(other_unit->position, _my_unit->position), std::make_optional<model::Aim>(aim));
    actions.insert({_my_unit->id, order});
}

model::Order MyUnit::CreateOrder()
{
    return model::Order(actions);
}

void MyUnit::AddGetShieldAction()
{
    if (!actions.empty())
        return;

    if (_my_unit->shieldPotions == _constants.maxShieldPotionsInInventory)
        return;

    auto closest_shield = closestLoot(_my_unit->position, _shield_potions);
    if (!closest_shield.has_value())
        return;

    auto loot_pos = closest_shield.value().position;
    auto diff_vec = vecDiff(loot_pos, _my_unit->position);
    model::UnitOrder order (diff_vec, diff_vec, std::make_optional<model::Pickup>(model::Pickup(closest_shield->id)));
    actions.insert({_my_unit->id, order});
}

void MyUnit::AddGetWeaponAction()
{
    if (!actions.empty())
        return;

    std::vector<model::Loot> prior_weapons;

    for (auto wp : _weapons){
        auto weapon = std::get<model::Weapon>(wp.item);
        if (weapon.typeIndex <= _my_unit->weapon.value_or(9999999999))
            continue;

        if (_my_unit->ammo[weapon.typeIndex] == 0)
            continue;

        prior_weapons.push_back(wp);
    }

    auto closest_weapon = closestLoot(_my_unit->position, prior_weapons);
    if (!closest_weapon.has_value())
        return;

    auto loot_pos = closest_weapon.value().position;
    auto diff_vec = vecDiff(loot_pos, _my_unit->position);
    model::UnitOrder order (diff_vec, diff_vec, std::make_optional<model::Pickup>(model::Pickup(closest_weapon->id)));
    actions.insert({_my_unit->id, order});
}

void MyUnit::AddUseShieldAction()
{
    if (!actions.empty())
        return;

    if (_my_unit->shieldPotions == 0)
        return;

    if (_my_unit->shield == _constants.maxShield )
        return;

    auto currentDirection = model::Vec2(-_my_unit->direction.y, _my_unit->direction.x);
    model::UnitOrder order (currentMoveVec(), currentDirection, std::make_optional<model::UseShieldPotion>(model::UseShieldPotion()));
    actions.insert({_my_unit->id, order});
}

void MyUnit::AddGetAmmoAction()
{
    if (!actions.empty())
        return;

    std::vector<model::Loot> prior_ammo;

    for (auto a : _ammo){
        auto ammo = std::get<model::Ammo>(a.item);
        if (_my_unit->ammo[ammo.weaponTypeIndex] == _constants.weapons[ammo.weaponTypeIndex].maxInventoryAmmo )
            continue;

        prior_ammo.push_back(a);
    }

    auto closest_weapon = closestLoot(_my_unit->position, prior_ammo);
    if (!closest_weapon.has_value())
        return;

    auto loot_pos = closest_weapon.value().position;
    auto diff_vec = vecDiff(loot_pos, _my_unit->position);
    model::UnitOrder order (diff_vec, diff_vec, std::make_optional<model::Pickup>(model::Pickup(closest_weapon->id)));
    actions.insert({_my_unit->id, order});
}


void MyUnit::ClearActions() 
{ 
    actions.clear();
}

