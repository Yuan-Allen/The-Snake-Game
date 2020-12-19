#ifndef COOR_H
#define COOR_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class Coor:public QPoint
{
public:
    int step;         //用于AI蛇
    Coor();
    Coor(int _x, int _y, int _step = 0);
    bool operator==(const Coor &t)const {
        return (x() == t.x()) && (y() == t.y());
    }
    bool operator!=(const Coor &t)const {
        return (x() != t.x()) || (y() != t.y());
    }
    bool operator<(const Coor &t)const {
        return x() == t.x() ? y() < t.y() : x() < t.x();
    }

};

#endif // COOR_H
