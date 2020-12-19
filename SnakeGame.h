#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "coor.h"
#include "snake.h"
#include "wall.h"
#include "foods.h"
#include "boardwidget.h"
#include "startwidget.h"
#include <qtimer.h>
#include <QKeyEvent>
#include "Toolwidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

//enum State{welcome,inMenu,running,pause,over};
enum Mode{single,multiplayer,AI};

extern const int BLOCK_SIZE; //单位方块边长
extern const int AREA_ROW; //场景行数
extern const int AREA_COL; //场景列数

const int timeInterval=500;    //时间间隔

class SnakeGame : public QMainWindow
{
    Q_OBJECT

    friend class BoardWidget;
    friend class Foods;
    friend class Snake;
    friend class AISnake;
    friend class wall;
    friend class ToolWidget;
    friend class StartWidget;

private:

    BoardWidget *boardWidget;
    StartWidget *startWidget;
    ToolWidget *toolWidget;

    QList<Snake> snakes;  //可能有多条蛇
    AISnake *AI;         //AI蛇
    Wall wall;      //墙
    Foods foods;   //可能有多个果实，Foods为所有果实
    State state;  //游戏状态
    int snakesNum;  //蛇的数量
    Coor cursor;    //地图编辑所用光标

    QTimer *timer1,*timer2;    //计时器

    void initialize();   //程序初始化
    void initialBeforePlay();     //play前的初始化
    void initialBeforeAI();     //AI模式的初始化
    void gameUpdate();   //更新状态
    void changeState(State s);  //改变游戏状态
    void updateS1();        //调用第一条蛇的update
    void updateS2();        //调用第二条蛇的update
    void updateAI();        //调用AI蛇的update
    void keyPressEvent(QKeyEvent *event) override;
    void clearInformation();        //清除蛇和食物，解开定时器信号槽
    void restart();     //重新开始（以当前模式）
    void save();        //存档
    void resume();      //读档
    bool isEmpty(Coor coor);       //工具函数，如果地图上该坐标地方什么东西也没有则返回true
    void BackToMenu();          //回到标题页面


public:
    SnakeGame(QWidget *parent = 0);
    ~SnakeGame();

    void singlePlay();
    void multiPlay();
    void AIPlay();
};

#endif // MAINWINDOW_H
