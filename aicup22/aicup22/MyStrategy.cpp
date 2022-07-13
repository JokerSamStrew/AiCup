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
    auto rad = game.zone.currentRadius;
    auto center = game.zone.currentCenter;
    auto obs_in_zone = getObstaclesInsideCircle(_constants.obstacles, center, rad - 3.0);

    highlightObstacles(obs_in_zone, debugInterface);
    myUnit.ClearActions();
    myUnit.setGame(&game, debugInterface);
    myUnit.AddFightClosestAction();
    myUnit.AddNoVisibleUnitsAction();
    return myUnit.CreateOrder();
}

void MyStrategy::debugUpdate(int displayedTicks, DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
