#include "Toolwidget.h"
#include "SnakeGame.h"

ToolWidget::ToolWidget(SnakeGame *g, QWidget *parent) : QWidget(parent)
{
    game=g;
    setGeometry(840,0,360,840);

    score1=new QLabel(this);
    score2=new QLabel(this);
    hint=new QLabel(this);
    restart=new QPushButton("restart",this);
    backToWelcome=new QPushButton("back",this);

    score1->setGeometry(20,40,320,100);
    score2->setGeometry(20,140,320,100);
    hint->setGeometry(20,240,320,300);
    restart->setGeometry(80,600,200,80);
    backToWelcome->setGeometry(80,700,200,80);

    connect(restart,&QPushButton::clicked,game,&SnakeGame::restart);
    connect(backToWelcome,&QPushButton::clicked,game,&SnakeGame::BackToMenu);

    restart->setFocusPolicy(Qt::NoFocus);

    score1->setText("score of snake1: 0\n           life: 1");
    score2->setText("score of snake2: 0\n           life: 1");
    hint->setText("黄色：普通食物\n红色：加一条命\n蓝绿色：加速\n橘色：长度减1\nP键暂停\n暂停时按E进入地图编辑\n地图编辑：Z设置墙\n          X设置食物");
}
