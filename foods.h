#ifndef FOOD_H
#define FOOD_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "coor.h"

extern const int BLOCK_SIZE; //单位方块边长
extern const int AREA_ROW; //场景行数
extern const int AREA_COL; //场景列数

enum Type{normal,heart,incitant,cut};

class Food:public Coor
{
    friend class SnakeGame;
private:
    int type;
public:
    Food(int xx=0, int yy=0, Type tp=normal);
    int retType() const {return type;}
};


class Foods:public QList<Food>
{
    friend class SnakeGame;
private:
    class SnakeGame *game;

    int count;

public:
    Foods(SnakeGame *g);
    bool posLegal(const Coor &tmp);
    void addFood(Type tp);
    void generate();     //生成各种食物，push_back到最后
    void reGenerate(int i);    //重新生成第i个食物
    int contain(Coor obj);    //返回Foods中含有此坐标的下标值，如果不含有则返回-1

};
#endif // FOOD_H
