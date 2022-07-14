#include "MyStrategy.hpp"
#include <exception>
#include "debugging/Color.hpp"
#include <utility>
#include <vector>
#include <Stream.hpp>
#include "Utils.hpp"

MyStrategy::MyStrategy(const model::Constants &constants) : _constants(constants), myUnit(constants) {}

model::Order MyStrategy::getOrder(const model::Game &game, DebugInterface *debugInterface)
{
    myUnit.ClearActions();
    myUnit.setGame(&game, debugInterface);
    myUnit.AddFightClosestAction();
    myUnit.AddUseShieldAction();
    myUnit.AddGetShieldAction();
    myUnit.AddNoVisibleUnitsAction();
    return myUnit.CreateOrder();
}

void MyStrategy::debugUpdate(int displayedTicks, DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
