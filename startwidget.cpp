#include "startwidget.h"
#include "SnakeGame.h"

StartWidget::StartWidget(QWidget *parent) : QWidget(parent)
{
    resize(1200,840);

    startBotton=new QPushButton("Play",this);
    resumeBotton=new QPushButton("resume",this);
    multiPlayBotton=new QPushButton("2 Players",this);
    AIBotton= new QPushButton("AI Mode",this);
    exitBotton=new QPushButton("exit",this);

    startBotton->setGeometry(300,300,300,300);
    resumeBotton->setGeometry(800,300,200,80);
    multiPlayBotton->setGeometry(800,400,200,80);
    AIBotton->setGeometry(800,500,200,80);
    exitBotton->setGeometry(800,600,200,80);

    connect(startBotton,&QPushButton::clicked,static_cast<SnakeGame*>(parent),&SnakeGame::singlePlay);
    connect(multiPlayBotton,&QPushButton::clicked,static_cast<SnakeGame*>(parent),&SnakeGame::multiPlay);
    connect(resumeBotton,&QPushButton::clicked,static_cast<SnakeGame*>(parent),&SnakeGame::resume);
    connect(AIBotton,&QPushButton::clicked,static_cast<SnakeGame*>(parent),&SnakeGame::AIPlay);
    connect(exitBotton,&QPushButton::clicked,parent,&QWidget::close);

    startBotton->setFocusPolicy(Qt::NoFocus);
    multiPlayBotton->setFocusPolicy(Qt::NoFocus);
    resumeBotton->setFocusPolicy(Qt::NoFocus);
    AIBotton->setFocusPolicy(Qt::NoFocus);
}
