#include "boardwidget.h"
#include "SnakeGame.h"

BoardWidget::BoardWidget(SnakeGame *g, QWidget *parent) : QWidget(parent)
{
    game=g;
    resize(840,840);
    timer=new QTimer;
    timer->start(10);
    connect(timer,&QTimer::timeout,this,static_cast<void (QWidget::*)()>(&QWidget::update));
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    int side = qMin(width(), height());
    p.scale(side / 840, side / 840 );
    p.fillRect(0, 0, width(), height(), QGradient:: JungleDay);

    paintSnake(&p);

    paintFoods(&p);

    paintWall(&p);

    paintCursor(&p);
}

void BoardWidget::paintSnake(QPainter *p)
{
    p->save();
    //画蛇
    p->setPen(Qt::NoPen);

    if(game->snakes.size()>0)
    {
        p->setBrush(Qt::darkBlue);
        p->drawRect(game->snakes[0].front().x()*BLOCK_SIZE,game->snakes[0].front().y()*BLOCK_SIZE,BLOCK_SIZE-2,BLOCK_SIZE-2);
        p->setBrush(Qt::blue);
        for (int i=1;i<game->snakes[0].size();++i)
        {
         p->drawRect(game->snakes[0][i].x()*BLOCK_SIZE,game->snakes[0][i].y()*BLOCK_SIZE,BLOCK_SIZE-2,BLOCK_SIZE-2);
        }

        if(game->snakesNum>1)
        {
            p->setBrush(Qt::darkGreen);
            p->drawRect(game->snakes[1].front().x()*BLOCK_SIZE,game->snakes[1].front().y()*BLOCK_SIZE,BLOCK_SIZE-2,BLOCK_SIZE-2);
            p->setBrush(Qt::green);
            for (int i=1;i<game->snakes[1].size();++i)
            {
             p->drawRect(game->snakes[1][i].x()*BLOCK_SIZE,game->snakes[1][i].y()*BLOCK_SIZE,BLOCK_SIZE-2,BLOCK_SIZE-2);
            }
        }

    }

    if(game->AI!=nullptr)
    {
        p->setBrush(Qt::darkBlue);
        p->drawRect(game->AI->front().x()*BLOCK_SIZE,game->AI->front().y()*BLOCK_SIZE,BLOCK_SIZE-2,BLOCK_SIZE-2);
        p->setBrush(Qt::blue);
        for (int i=1;i<game->AI->size();++i)
        {
         p->drawRect(game->AI->at(i).x()*BLOCK_SIZE,game->AI->at(i).y()*BLOCK_SIZE,BLOCK_SIZE-2,BLOCK_SIZE-2);
        }
    }


    p->restore();
}

void BoardWidget::paintFoods(QPainter *p)
{
    p->save();
    //画果实
    p->setPen(Qt::NoPen);
    for (int i=0;i<game->foods.size();++i)
    {
        switch (game->foods[i].retType()){
        case normal:
            p->setBrush(Qt::yellow);
            p->drawEllipse(game->foods[i].x()*BLOCK_SIZE,game->foods[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            break;
        case heart:
            p->setBrush(Qt::red);
            p->drawEllipse(game->foods[i].x()*BLOCK_SIZE,game->foods[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            break;
        case incitant:
            p->setBrush(Qt::cyan);
            p->drawEllipse(game->foods[i].x()*BLOCK_SIZE,game->foods[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            break;
        case cut:
            p->setBrush(QColor(255,165,0));
            p->drawEllipse(game->foods[i].x()*BLOCK_SIZE,game->foods[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            break;
        }
    }
    p->restore();
}

void BoardWidget::paintWall(QPainter *p)
{
    p->save();
    p->setPen(Qt::NoPen);
    p->setBrush(Qt::black);
    for (int i=0;i<game->wall.size();++i) {
        p->drawRect(game->wall[i].x()*BLOCK_SIZE,game->wall[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    }
    p->restore();
}

void BoardWidget::paintCursor(QPainter *p)
{
    if(game->state!=edit) return;
    p->save();
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::white);
    p->setPen(pen);
    p->drawRect(game->cursor.x()*BLOCK_SIZE,game->cursor.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    p->restore();
}

