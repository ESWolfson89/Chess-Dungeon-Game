#ifndef BEING_H
#define BEING_H

#include  "globals.h"
#include  "point.h"

using namespace point1;

namespace being1
{
class being
{
public:
    being();
    being(entity_tile, point);
    void setl(point);
    point getl();
    entity_tile getType();
    bool getAggroFlag();
    void setAggroFlag(bool);
private:
    point loc;
    entity_tile type_obj;
    bool aggroed;
};
}

#endif
