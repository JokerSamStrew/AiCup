#include "MyStrategy.hpp"
#include <exception>
#include "debugging/Color.hpp"
#include <utility>
#include <vector>
#include <Stream.hpp>
#include "Utils.hpp"

MyStrategy::MyStrategy(const model::Constants &constants) : _constants(constants), _my_first_unit(constants), _my_second_unit(constants) {}

bool MyStrategy::canSetUnit(const MyUnit& unit, int unit_id)
{
    return !unit.ID.has_value() || unit.ID == unit_id;
}

void MyStrategy::getUnitOrder(MyUnit& unit)
{
    actions.insert({unit.ID.value(), unit.GetOrder().value()});
}

model::Order MyStrategy::getOrder(const model::Game &game, DebugInterface *debugInterface)
{
    auto units = getUnits(game);
    auto other_units = units.second;
    auto my_units = units.first;

    actions.clear();
    for (auto my_unit : my_units)
    {
        if (canSetUnit(_my_first_unit, my_unit.id))
        {
            _my_first_unit.setGame(&game, my_unit, other_units, debugInterface);
            _my_second_unit.setSecondUnit(my_unit);
            getUnitOrder(_my_first_unit);
        }
        else if (canSetUnit(_my_second_unit, my_unit.id))
        {
            _my_second_unit.setGame(&game, my_unit, other_units, debugInterface);
            _my_second_unit.setSecondUnit(my_unit);
            getUnitOrder(_my_second_unit);
        }
    }

    return model::Order(actions); 
}

void MyStrategy::debugUpdate(int displayedTicks, DebugInterface &debugInterface) 
{
    debugInterface.flush();
}

void MyStrategy::finish() {}
