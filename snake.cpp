#include "snake.h"
#include "SnakeGame.h"

Snake::Snake(SnakeGame *g):game(g)
{    
    direction=RIGHT;
    life=1;
    speed=1;
    score=0;
}

Snake::Snake(const Snake &obj):game(obj.game)
{
    direction=obj.direction;
    life=obj.life;
    speed=obj.speed;
    score=obj.score;
}

void Snake::update()
{
    move();
    int i=game->foods.contain(front());
    if(i>=0)
    {
        eatFoods(game->foods.at(i).retType());
        if(game->foods.at(i).retType()==normal)
            game->foods.reGenerate(i);
        else game->foods.removeAt(i);
    }
    else pop_back();

    for (int i=1;i<size();++i)                //判断有没有吃到自己
    {
        if(at(i)==front())
            die();
    }

    for (int i=0;i<game->snakes.size();++i)               //判断有没有撞到其他蛇
    {
        if(game->snakes.indexOf(*this)==i)                                //判断是否撞其他蛇，所以排除自己
            continue;
        if(game->snakes[i].contains(front()))
            die();
    }

    if(game->wall.contains(front()))                //判断是否撞到墙
        die();

    updateImfDisplay();
}

void Snake::move()
{
    switch (direction) {
    case UP:
        if(this->front().y()>0)
            push_front(Coor(this->front().x(),this->front().y()-1));
        else push_front(Coor(this->front().x(),AREA_ROW));
        break;
    case DOWN:
        if(this->front().y()<AREA_ROW)
            push_front(Coor(this->front().x(),this->front().y()+1));
        else push_front(Coor(this->front().x(),0));
        break;
    case RIGHT:
        if(this->front().x()<AREA_COL)
            push_front(Coor(this->front().x()+1,this->front().y()));
        else push_front(Coor(0,this->front().y()));
        break;
    case LEFT:
        if(this->front().x()>0)
            push_front(Coor(this->front().x()-1,this->front().y()));
        else push_front(Coor(AREA_COL,this->front().y()));
        break;
    }
}

void Snake::turnDirect(int cmd)
{
    if(cmd==-1)
        return;
    switch (cmd)
    {
        case UP:
            if(direction!=DOWN)
                direction=UP;
            break;
        case DOWN:
            if(direction!=UP)
                direction=DOWN;
            break;
        case LEFT:
            if(direction!=RIGHT)
                direction=LEFT;
            break;
        case RIGHT:
            if(direction!=LEFT)
                direction=RIGHT;
            break;
    }
}

void Snake::eatFoods(int tp)
{
    switch (tp){
    case normal:
        score+=1;
        break;
    case heart:
        score+=1;
        life+=1;
        break;
    case incitant:
        score+=1;
        speedUp();
        break;
    case cut:
        score+=1;
        pop_back();
        pop_back();
       break;
    }

}


void Snake::die()
{
    life-=1;
    if(isDead())
        game->changeState(over);
}

bool Snake::isDead()
{
    return life<1;
}

void Snake::speedUp()
{
    speed+=1;
    if(game->snakes.indexOf(*this)==0)
        game->timer1->setInterval(timeInterval/speed);
    else game->timer2->setInterval(timeInterval/speed);
}

void Snake::updateImfDisplay()
{
    int i=game->snakes.indexOf(*this);
    if (i==0)
        game->toolWidget->score1->setText("score of snake1: "+QString::number(score)+"\n           life: "+QString::number(life));
    else game->toolWidget->score2->setText("score of snake2: "+QString::number(score)+"\n           life: "+QString::number(life));
}

//AI相关---------------------------------------------------------

AISnake::AISnake(SnakeGame *g):Snake (g)
{
    direction=RIGHT;
    life=1;
    speed=25;
    score=0;
}

AISnake::AISnake(const AISnake &obj):Snake (obj.game)
{
    for (int i=0;i<obj.size();++i)
    {
        push_back(obj[i]);
    }
}

void AISnake::update()
{
    turnDirect(getNextCmd());
    move();
    int i=game->foods.contain(front());
    if(i>=0)
    {
        eatFoods(game->foods.at(i).retType());
        if(game->foods.at(i).retType()==normal)
            game->foods.reGenerate(i);
        else game->foods.removeAt(i);
    }
    else pop_back();

    for (int i=1;i<size();++i)                //判断有没有吃到自己
    {
        if(at(i)==front())
            die();
    }

    if(game->wall.contains(front()))                //判断是否撞到墙
        die();

    updateImfDisplay();
}

void AISnake::updateImfDisplay()
{
    game->toolWidget->score1->setText("score of snake1: "+QString::number(score)+"\n           life: "+QString::number(life));
}

int AISnake::canFindPath(const Coor _start, const Coor _end)
{
    if(_start==_end)
        return 0;
    const int next[4][2]={{1,0},{0,1},{-1,0},{0,-1}};

    QQueue<Coor> bfs_que;
    QList<Coor> snake_list;
    Coor tcoor;
    bfs_que.enqueue(_start);
    for(int i=0;i<size();++i)
        snake_list.push_front(this->at(i));

    if(size()>=4&&_end==this->back())
        snake_list.removeAll(_end);
    while (!bfs_que.empty())
    {
        for (int k=0;k<4;++k)
        {
            tcoor.rx()=bfs_que.front().x()+next[k][0];
            tcoor.ry()=bfs_que.front().y()+next[k][1];
            if(!inBorder(tcoor)||game->wall.contains(tcoor))
                continue;
            if(!snake_list.contains(tcoor))
            {
                tcoor.step=bfs_que.front().step+1;
                snake_list.push_front(tcoor);
                bfs_que.enqueue(tcoor);
            }
            if(tcoor==_end)   //到达目标
                return tcoor.step;
        }
        bfs_que.dequeue();
    }
    return -1;
}

bool AISnake::canFindTail(Coor _start)
{
    return canFindPath(_start,this->back())>=0?true:false;
}

bool AISnake::canFindFood(Coor _start)
{
    return canFindPath(_start,game->foods.back())>=0?true:false;
}

int AISnake::NextCmdToFood()
{
    const int next[4][2]={{1,0},{0,1},{-1,0},{0,-1}};

    Coor aroundPoint[4];
    for(int i=0;i<4;++i)
    {
        aroundPoint[i].rx()=front().x()+next[i][0];
        aroundPoint[i].ry()=front().y()+next[i][1];
        if(!inBorder(aroundPoint[i])||contains(aroundPoint[i])||game->wall.contains(aroundPoint[i]))
            aroundPoint[i].step=-1;
        else
            aroundPoint[i].step=canFindPath(aroundPoint[i],game->foods.front());
    }

    int minstep_index=0;
    for(int i=1;i<4;++i)
    {
        if(aroundPoint[minstep_index].step==-1)
            minstep_index=i;
        if(aroundPoint[i].step!=-1)
        {
            if(aroundPoint[i].step<aroundPoint[minstep_index].step)
                minstep_index=i;
        }
    }

    int cmd=-1;
    if(aroundPoint[minstep_index].step!=-1)
        cmd=RetCmd(aroundPoint[minstep_index]);
    return cmd;
}

int AISnake::NextCmdToTail()
{
    const int next[4][2]={{1,0},{0,1},{-1,0},{0,-1}};

    Coor aroundPoint[4];
    for(int i=0;i<4;++i)
    {
        aroundPoint[i].rx()=front().x()+next[i][0];
        aroundPoint[i].ry()=front().y()+next[i][1];
        if (inBorder(aroundPoint[i]) && !contains(aroundPoint[i])&&!game->wall.contains(aroundPoint[i]))
            aroundPoint[i].step = canFindPath(aroundPoint[i],back());
        else
            aroundPoint[i].step = -1;
    }

    int maxstep_index=0;
    for(int i=1;i<4;++i)
    {
        if(aroundPoint[maxstep_index].step==-1)
            maxstep_index=i;
        if(aroundPoint[i].step!=-1)
        {
            if(aroundPoint[i].step>aroundPoint[maxstep_index].step)
                maxstep_index=i;
        }
    }

    int cmd=-1;
    if(aroundPoint[maxstep_index].step!=-1)
        cmd=RetCmd(aroundPoint[maxstep_index]);
    return cmd;
}

int AISnake::NextCmdToFarAway()
{
    const int next[4][2]={{1,0},{0,1},{-1,0},{0,-1}};

    Coor aroundPoint[4];
    for(int i=0;i<4;++i)
    {
        aroundPoint[i].rx()=front().x()+next[i][0];
        aroundPoint[i].ry()=front().y()+next[i][1];
        if (inBorder(aroundPoint[i]) && !contains(aroundPoint[i])&&!game->wall.contains(aroundPoint[i]))
            aroundPoint[i].step = getManhattanDistance(aroundPoint[i],back());
        else
            aroundPoint[i].step = -1;
    }

    int maxstep_index=0;
    for(int i=1;i<4;++i)
    {
        if(aroundPoint[maxstep_index].step==-1)
            maxstep_index=i;
        if(aroundPoint[i].step!=-1)
        {
            if(aroundPoint[i].step>aroundPoint[maxstep_index].step)
                maxstep_index=i;
        }
    }

    int cmd=-1;
    if(aroundPoint[maxstep_index].step!=-1)
        cmd=this->RetCmd(aroundPoint[maxstep_index]);
    return cmd;
}

int AISnake::RetCmd(Coor nextPoint)
{
    int cmd=-1;
    int dx=nextPoint.x()-front().x();
    int dy=nextPoint.y()-front().y();
    if (dx == 0 && dy < 0)
        cmd = UP;
    if (dx == 0 && dy > 0)
        cmd = DOWN;
    if (dx < 0 && dy == 0)
        cmd = LEFT;
    if (dx > 0 && dy == 0)
        cmd = RIGHT;
    return cmd;
}

int AISnake::getManhattanDistance(Coor _a, Coor _b)
{
    return (abs(_a.x()-_b.x())+abs(_a.y()-_b.y()));
}

bool AISnake::inBorder(Coor coor)
{
    return (coor.x()>=0&&coor.x()<=AREA_COL&&coor.y()>=0&&coor.y()<=AREA_ROW);
}

int AISnake::getNextCmd()
{
    int cmd=-1;
    if(canFindFood(front()))
    {
        simulate=new AISnake(*this);
        while (simulate->front()!=game->foods.front())
        {
            simulate->turnDirect(simulate->NextCmdToFood());
            simulate->move();
            if(simulate->front()!=game->foods.front()) simulate->pop_back();
        }

        if((simulate->size()<4&&simulate->canFindPath(simulate->front(),simulate->back())>0)||(simulate->size()>=4&&simulate->canFindPath(simulate->front(),simulate->back())>1))
            cmd=NextCmdToFood();
        else
            if(canFindTail(front()))
                cmd=NextCmdToTail();
            else
                cmd=NextCmdToFarAway();
        delete simulate;
    }
    else
        if(canFindTail(front()))
            cmd=NextCmdToTail();
        else
            cmd=NextCmdToFarAway();

    return cmd;
}















