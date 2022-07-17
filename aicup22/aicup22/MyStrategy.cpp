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
    for (auto my_unit : my_units)
    {
        if (!myFirstUnit.ID.has_value() || myFirstUnit.ID == my_unit.id)
        {
            myFirstUnit.ClearActions();
            myFirstUnit.setGame(&game, my_unit, other_units, debugInterface);
            myFirstUnit.AddFightClosestAction();
            myFirstUnit.AddSoundAction();
            myFirstUnit.AddUseShieldAction();
            myFirstUnit.AddGetShieldAction();
            myFirstUnit.AddGetAmmoAction();
            myFirstUnit.AddGetWeaponAction();
            myFirstUnit.AddNoVisibleUnitsAction();
            mySecondUnit.setSecondUnit(my_unit);
            actions.insert({myFirstUnit.ID.value(), myFirstUnit.GetOrder().value()});
        }
        else if (!mySecondUnit.ID.has_value() || mySecondUnit.ID == my_unit.id)
        {
            mySecondUnit.ClearActions();
            mySecondUnit.setGame(&game, my_unit, other_units, debugInterface);
            mySecondUnit.AddFightClosestAction();
            mySecondUnit.AddSoundAction();
            mySecondUnit.AddUseShieldAction();
            mySecondUnit.AddGetShieldAction();
            mySecondUnit.AddGetAmmoAction();
            mySecondUnit.AddGetWeaponAction();
            mySecondUnit.AddNoVisibleUnitsAction();
            mySecondUnit.setSecondUnit(my_unit);
            actions.insert({mySecondUnit.ID.value(), mySecondUnit.GetOrder().value()});
        }
    }

    return model::Order(actions); 
}

void MyStrategy::debugUpdate(int displayedTicks, DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
