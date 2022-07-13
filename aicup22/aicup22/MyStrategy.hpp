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

public:
    MyStrategy(const model::Constants& constants);
    model::Order getOrder(const model::Game& game, DebugInterface* debugInterface);
    MyUnit myUnit;
    void debugUpdate(int displayedTick, DebugInterface& debugInterface);
    void finish();
};

#endif
