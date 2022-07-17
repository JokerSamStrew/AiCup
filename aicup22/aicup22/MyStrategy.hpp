#ifndef _MY_STRATEGY_HPP_
#define _MY_STRATEGY_HPP_

#include "DebugInterface.hpp"
#include "model/Game.hpp"
#include "model/Order.hpp"
#include "model/Constants.hpp"
#include "MyUnit.hpp"

class MyStrategy {
private:
    model::Constants _constants;
    bool canSetUnit(const MyUnit& unit, int unit_id);
    void getUnitOrder(MyUnit& unit);
public:
    MyStrategy(const model::Constants& constants);
    model::Order getOrder(const model::Game& game, DebugInterface* debugInterface);
    MyUnit _my_first_unit;
    MyUnit _my_second_unit;
    std::unordered_map<int, model::UnitOrder> actions;
    void debugUpdate(int displayedTick, DebugInterface& debugInterface);
    void finish();
};

#endif
