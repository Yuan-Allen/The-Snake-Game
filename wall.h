#ifndef WALL_H
#define WALL_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "coor.h"

class Wall:public QList<Coor>
{
public:
    Wall();
};

#endif // WALL_H
