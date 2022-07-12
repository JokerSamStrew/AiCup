#ifndef _MY_UNIT_HPP_
#define _MY_UNIT_HPP_

#include "model/Unit.hpp"
#include "model/Game.hpp"
#include "model/Order.hpp"
#include "model/UnitOrder.hpp"
#include "DebugInterface.hpp"
#include <unordered_map>

class MyUnit
{
    private:
        const model::Unit* _my_unit;
        std::vector<const model::Unit*> _other_units;
        const model::Game* _game;
        std::unordered_map<int, model::UnitOrder> actions;
    public:
        MyUnit();
        void setGame(const model::Game* game, DebugInterface* debugInterface);
        void AddNoVisibleUnitsAction();
        void AddFightClosestAction();
        model::Order CreateOrder();
        void ClearActions();
};

#endif
