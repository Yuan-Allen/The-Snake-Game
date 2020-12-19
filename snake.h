#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "coor.h"
#include "foods.h"
#include <QTimer>
#include <QQueue>

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

class Snake: public QList<Coor>
{
    friend SnakeGame;
protected:
    int life;
    int direction;
    int speed;
    int score;

    class SnakeGame *game;
public:
    Snake(SnakeGame *g);
    Snake(const Snake &obj);
    virtual void update();              //蛇的更新（外部只需调用的函数）
    void move();                //蛇的移动
    void turnDirect(int cmd);       //蛇的转向
    void eatFoods(int tp);             //吃食物
    void die();             //死亡（失去一条生命）
    bool isDead();              //已死亡（life<1）
    void speedUp();             //加速（speed增加，时间间隔减小）
    void pause();               //暂停或继续
    virtual void updateImfDisplay();       //更新toolWidget分数和生命的显示
};


class AISnake: public Snake
{
private:
    AISnake *simulate;         //为AI蛇探路的模拟蛇

public:
    AISnake(SnakeGame *g);
    AISnake(const AISnake &obj);

    void update() override;
    void updateImfDisplay() override;
    int canFindPath(const Coor _start, const Coor _end);    //能找到从_start到_end的路径
    bool canFindTail(Coor _start);      //能找到尾巴
    bool canFindFood(Coor _start);      //能找到食物
    int NextCmdToFood(); //最短距离吃食物
    int NextCmdToTail();  //最远距离吃尾巴
    int NextCmdToFarAway();  //最远曼哈顿距离吃尾巴（既不能吃到食物又不能吃到尾巴时调用）
    int RetCmd(Coor nextPoint);
    int getNextCmd(); //得到下一步指令
    static int getManhattanDistance(Coor _a, Coor _b);      //获得曼哈顿距离
    static bool inBorder(Coor coor);
};











#endif // SNAKE_H
