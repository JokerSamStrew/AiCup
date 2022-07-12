#include "MyUnit.hpp"

MyUnit::MyUnit()
{
}

void MyUnit::setUnit(const model::Unit& unit)
{
    my_unit = &unit;
}
