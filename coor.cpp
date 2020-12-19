#include "coor.h"

Coor::Coor()
{
    step=0;
}

Coor::Coor(int _x, int _y, int _step):QPoint (_x,_y),step(_step)
{

}
