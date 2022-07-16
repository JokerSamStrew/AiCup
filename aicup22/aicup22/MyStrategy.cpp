#include "MyStrategy.hpp"
#include <exception>
#include "debugging/Color.hpp"
#include <utility>
#include <vector>
#include <Stream.hpp>
#include "Utils.hpp"

MyStrategy::MyStrategy(const model::Constants &constants) : _constants(constants), myFirstUnit(constants), mySecondUnit(constants) {}

model::Order MyStrategy::getOrder(const model::Game &game, DebugInterface *debugInterface)
{
    auto units = getUnits(game);
    auto other_units = units.second;
    auto my_units = units.first;

    actions.clear();
    if (my_units.empty())
        return model::Order(actions); 

    myFirstUnit.ClearActions();
    myFirstUnit.setGame(&game,  my_units[0], other_units, debugInterface);
    myFirstUnit.AddFightClosestAction();
    myFirstUnit.AddSoundAction();
    myFirstUnit.AddUseShieldAction();
    myFirstUnit.AddGetShieldAction();
    myFirstUnit.AddGetAmmoAction();
    myFirstUnit.AddGetWeaponAction();
    myFirstUnit.AddNoVisibleUnitsAction();
    for (auto action : myFirstUnit.GetActions())
        actions.insert(action);

    if (my_units.size() < 2)
        return model::Order(actions); 

    mySecondUnit.ClearActions();
    mySecondUnit.setGame(&game,  my_units[1], other_units, debugInterface);
    mySecondUnit.AddFightClosestAction();
    mySecondUnit.AddSoundAction();
    mySecondUnit.AddUseShieldAction();
    mySecondUnit.AddGetShieldAction();
    mySecondUnit.AddGetAmmoAction();
    mySecondUnit.AddGetWeaponAction();
    mySecondUnit.AddNoVisibleUnitsAction();
    for (auto action : mySecondUnit.GetActions())
        actions.insert(action);

    return model::Order(actions); 
}

void MyStrategy::debugUpdate(int displayedTicks, DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
