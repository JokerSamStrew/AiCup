#ifndef _MY_UNIT_HPP_
#define _MY_UNIT_HPP_

#include "model/Unit.hpp"
#include "model/Game.hpp"
#include "model/Order.hpp"
#include "model/UnitOrder.hpp"
#include "DebugInterface.hpp"
#include "model/Constants.hpp"
#include <unordered_map>

#define NEAR_OBS 1.5 
#define MOVE_RANGE 30.0
#define PICKUP_RANGE 10.0
#define SOUND_RANGE 10.0
#define EDGE_COEF 0.85
#define WEAPON_COEF 0.65 
#define MOVE_COEF 100000.0 
#define CLOSE_TO_REACH 0.5 
#define MIN_EDGE_RAD 5.0

class MyUnit
{
    private:
        std::vector<model::Loot> _shield_potions;
        std::vector<model::Loot> _weapons;
        std::vector<model::Loot> _ammo;

        std::optional<model::Obstacle> _current_obs;
        std::optional<model::Obstacle> _prev_obs;
        std::optional<model::Obstacle> _prev_prev_obs;
        model::Constants _constants;
        model::Vec2 currentMoveVec();
        std::optional<model::Unit> _my_unit;
        std::vector<model::Unit> _other_units;
        std::vector<model::Obstacle> _available_obs;
        const model::Game* _game;
        std::unordered_map<int, model::UnitOrder> actions;
        DebugInterface* _debugInterface;
        double countWeaponRange();
        void setLoot();
    public:
        MyUnit(const model::Constants& constants);
        void setGame(const model::Game* game, DebugInterface* debugInterface);
        void AddNoVisibleUnitsAction();
        void AddFightClosestAction();
        void AddGetShieldAction();
        void AddSoundAction();
        void AddUseShieldAction();
        void AddGetAmmoAction();
        void AddGetWeaponAction();
        model::Order CreateOrder();
        void ClearActions();
};

#endif
