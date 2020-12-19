#include "foods.h"
#include "SnakeGame.h"

Food::Food(int x, int y, Type tp):Coor(x,y)
{
    type=tp;
}

Foods::Foods(SnakeGame *g)
{
    game=g;
    count=0;
}

bool Foods::posLegal(const Coor &tmp)
{
    if(game->AI!=nullptr&&game->AI->contains(tmp))
        return false;

    if(game->wall.contains(tmp))    return false;

    if(game->foods.contain(tmp)>=0) return false;

    for (int i=0;i<game->snakes.size();++i)
    {
        if(game->snakes[i].contains(tmp))
            return false;
    }
    return true;
}

void Foods::addFood(Type tp)
{
    Food tmp((rand()%AREA_COL),(rand()%AREA_ROW),tp);

    while (!posLegal(Coor(tmp)))
    {
        tmp.setX(rand()%AREA_COL);
        tmp.setY(rand()%AREA_ROW);
    }
    push_back(tmp);
    if(tp==normal) count++;
}

void Foods::generate()
{
    addFood(normal);

    if(game->state==AIRunning) return;          //AI模式不生成特殊食物，直接返回

    if(count%7==0) addFood(cut);
    if(count%4==0) addFood(incitant);
    if(count%10==0) addFood(heart);
}

int Foods::contain(Coor obj)
{
    for (int i=0;i<this->size();++i)
    {
        if(Coor(this->at(i))==obj)
            return i;
    }
    return -1;
}

void Foods::reGenerate(int i)
{
    this->removeAt(i);
    generate();
}

/*
void Foods::reGenerate()
{
    reGenerate(this->size()-1);
}
*/
