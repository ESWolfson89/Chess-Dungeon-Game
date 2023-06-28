#include "being.h"

namespace being1
{

being::being()
{
}

being::being(entity_tile type, point p)
{
    loc.set(p.y(),p.x());
    type_obj = type;
    aggroed = false;
}

point being::getl()
{
    return loc;
}

void being::setl(point p)
{
    loc.set(p.y(),p.x());
}

entity_tile being::getType()
{
    return type_obj;
}

void being::setAggroFlag(bool new_flag)
{
    aggroed = new_flag;
}

bool being::getAggroFlag()
{
    return aggroed;
}
};
