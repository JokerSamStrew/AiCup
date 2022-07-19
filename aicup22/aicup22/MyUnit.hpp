#ifndef _MY_UNIT_HPP_
#define _MY_UNIT_HPP_

#include "model/Unit.hpp"
#include "model/Game.hpp"
#include "model/Order.hpp"
#include "model/UnitOrder.hpp"
#include "DebugInterface.hpp"
#include "model/Constants.hpp"
#include <unordered_map>

#define NEAR_OBS 1.8 
#define MOVE_RANGE 20.0
#define PICKUP_RANGE 10.0
#define SOUND_RANGE 20.0
#define EDGE_COEF 0.85
#define WEAPON_COEF 0.65 
#define MOVE_COEF 100000.0 
#define CLOSE_TO_REACH 0.4 
#define MIN_EDGE_RAD 5.0

class MyUnit
{
    private:
        bool _follow_second = false;
        std::vector<model::Loot> _shield_potions;
        std::vector<model::Loot> _weapons;
        std::vector<model::Loot> _ammo;

        std::optional<model::Obstacle> _current_obs;
        std::optional<model::Obstacle> _prev_obs;
        std::optional<model::Obstacle> _prev_prev_obs;
        std::optional<model::Unit> _current_target;
        int _prev_shield_level;
        int _prev_hp_level;
        model::Constants _constants;
        model::Vec2 currentMoveVec();
        std::optional<model::Unit> _my_unit;
        std::optional<model::Unit> _second_unit;
        std::optional<model::Obstacle> _second_unit_current_obs;
        std::vector<model::Sound> _sounds;
        std::vector<model::Unit> _other_units;
        std::vector<model::Obstacle> _available_obs;
        const model::Game* _game;
        std::vector<std::pair<double, model::UnitOrder>> _actions;

        DebugInterface* _debugInterface;
        double countWeaponRange();
        void setLoot();
        void setSound();
        void AddNoVisibleUnitsAction();
        void AddFightClosestAction();
        void AddGetShieldAction();
        void AddUseShieldAction();
        void AddGetAmmoAction();
        void AddGetWeaponAction();
        void ClearActions();

    public:
        std::optional<int> ID;
        MyUnit(const model::Constants& constants, bool follow_second);
        void setSecondUnit(const std::optional<model::Unit>& unit, const std::optional<model::Obstacle>& obs);
        void setGame(const model::Game* game, 
            const model::Unit& my_unit, 
            const std::vector<model::Unit>& other_units, 
            const std::vector<model::Obstacle>& obstacles, 
            DebugInterface* debugInterface);
        std::optional<model::UnitOrder> GetOrder();
        std::optional<model::Obstacle> GetCurrentObstacle();
};

#endif
