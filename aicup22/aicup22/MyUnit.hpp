#ifndef _MY_UNIT_HPP_
#define _MY_UNIT_HPP_

#include "model/Unit.hpp"

class MyUnit
{
    private:
        const model::Unit* my_unit;
    public:
        MyUnit();
        void setUnit(const model::Unit& unit);
};

#endif
