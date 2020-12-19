#include "SnakeGame.h"

const int BLOCK_SIZE=40; //单位方块边长
const int AREA_ROW=20; //场景行数
const int AREA_COL=20; //场景列数

SnakeGame::SnakeGame(QWidget *parent)
    : QMainWindow(parent),
      foods(this)
{
    setWindowTitle("The Snake Game");
    resize(1200, 840);

    srand(time(nullptr));
    startWidget=new StartWidget(this);
    boardWidget=new BoardWidget(this,this);
    toolWidget=new ToolWidget(this,this);

    setFocusPolicy(Qt::StrongFocus);

    timer1=new QTimer(this);
    timer2=new QTimer(this);

    initialize();
}

SnakeGame::~SnakeGame()
{
    delete timer1;
    delete timer2;
    delete boardWidget;
    delete startWidget;
    delete toolWidget;
    if(AI!=nullptr) delete AI;
}

void SnakeGame::initialize()
{
    state=welcome;
    snakesNum=0;
    AI=nullptr;

    boardWidget->close();
    startWidget->show();
    toolWidget->close();
}

void SnakeGame::initialBeforePlay()
{

    clearInformation();

    for (int i=0;i<snakesNum;++i)
    {
        snakes.push_back(Snake(this));
        snakes.back().push_front(Coor(AREA_COL/2,AREA_ROW/2+2*i));
    }
    timer1->start(timeInterval/snakes[0].speed);
    if(snakesNum>1)
        timer2->start(timeInterval/snakes[1].speed);
    foods.generate();
}

void SnakeGame::initialBeforeAI()
{
    clearInformation();

    AI=new AISnake(this);
    AI->push_back(Coor(AREA_COL/2,AREA_ROW/2));
    AI->push_back(Coor(AI->front().x()-1,AI->front().y()));

    timer1->start(timeInterval/AI->speed);

    foods.generate();
}

void SnakeGame::changeState(State s)        //游戏状态转换器
{
    switch (s){
    case welcome:
        startWidget->show();
        boardWidget->close();
        toolWidget->close();
        state=welcome;
        break;
    case running:
        initialBeforePlay();
        boardWidget->show();
        toolWidget->show();
        startWidget->close();
        connect(timer1,&QTimer::timeout,this,&SnakeGame::updateS1);
        if(snakesNum>1) connect(timer2,&QTimer::timeout,this,&SnakeGame::updateS2);
        state=running;
        break;
    case AIRunning:
        initialBeforeAI();
        boardWidget->show();
        toolWidget->show();
        startWidget->close();
        connect(timer1,&QTimer::timeout,this,&SnakeGame::updateAI);
        state=AIRunning;
        break;
    case pause:
        if(state==running||state==AIRunning)
        {
            timer1->stop();
            timer2->stop();
            state=pause;
            break;
        }
        if(state==pause)
        {
            if(AI)
            {
                state=AIRunning;
                timer1->start(timeInterval/AI->speed);
            }
            else
            {
                state=running;
                timer1->start(timeInterval/snakes[0].speed);
                if(snakesNum>1) timer2->start(timeInterval/snakes[1].speed);
            }
            break;
        }
        if(state==edit) state=pause;
        break;
    case over:
        state=over;
        timer1->stop();
        timer2->stop();
        break;
    case edit:
        if(state==pause)
        {
            state=edit;
            cursor.setX(0);
            cursor.setY(0);
        }
        break;
    }
}



void SnakeGame::updateS1()
{
    snakes[0].update();
}

void SnakeGame::updateS2()
{
    snakes[1].update();
}

void SnakeGame::updateAI()
{
    AI->update();
}

void SnakeGame::keyPressEvent(QKeyEvent *event)
{
    if(state==running)
    {
        if(snakes.size()>0)
        {
            switch (event->key()) {
            case Qt::Key_Up:
                snakes[0].turnDirect(UP);
                break;
            case Qt::Key_Down:
                snakes[0].turnDirect(DOWN);
                break;
            case Qt::Key_Right:
                snakes[0].turnDirect(RIGHT);
                break;
            case Qt::Key_Left:
                snakes[0].turnDirect(LEFT);
                break;
            default:
                break;
            }
        }
        if(snakes.size()>1)
        {
            switch (event->key()) {
            case Qt::Key_W:
                snakes[1].turnDirect(UP);
                break;
            case Qt::Key_S:
                snakes[1].turnDirect(DOWN);
                break;
            case Qt::Key_D:
                snakes[1].turnDirect(RIGHT);
                break;
            case Qt::Key_A:
                snakes[1].turnDirect(LEFT);
                break;
            default:
                break;
            }
        }
    }

    if(state==edit)
    {
        switch (event->key()) {
        case Qt::Key_Up:
            if(cursor.y()>0)    cursor.ry()--;
            else cursor.setY(AREA_ROW);
            break;
        case Qt::Key_Down:
            if(cursor.y()<AREA_ROW)    cursor.ry()++;
            else cursor.setY(0);
            break;
        case Qt::Key_Right:
            if(cursor.x()<AREA_COL)    cursor.rx()++;
            else cursor.setX(0);
            break;
        case Qt::Key_Left:
            if(cursor.x()>0)    cursor.rx()--;
            else cursor.setX(AREA_COL);
            break;
        case Qt::Key_Z:
            if(isEmpty(cursor)) wall.push_back(cursor);
            else if(wall.contains(cursor)) wall.removeOne(cursor);
            break;
        case Qt::Key_X:
            if(isEmpty(cursor)) foods.push_back(Food(cursor.x(),cursor.y(),normal));
            else if(foods.contain(cursor)>=0) foods.removeAt(foods.contain(cursor));
            break;
        }
    }


    switch (event->key()) {
    case Qt::Key_P:
        if(state==running||state==pause||state==AIRunning)
            changeState(pause);
        break;
    case Qt::Key_E:
        if(state==pause)
            changeState(edit);
        else if(state==edit) changeState(pause);
        break;
    default:
        break;
    }
}

void SnakeGame::singlePlay()
{
    snakesNum=1;
    changeState(running);
}

void SnakeGame::multiPlay()
{
    snakesNum=2;
    changeState(running);
}

void SnakeGame::AIPlay()
{
    snakesNum=1;
    changeState(AIRunning);
}

void SnakeGame::clearInformation()
{
    if(!snakes.empty()) snakes.clear();
    if(AI!=nullptr) {delete AI; AI=nullptr;}
    if(!foods.empty()) foods.clear();
    if(!wall.empty()) wall.clear();
    foods.count=0;
    disconnect(timer1,nullptr,this,nullptr);
    disconnect(timer2,nullptr,this,nullptr);
    if(timer1->isActive()) timer1->stop();
    if(timer2->isActive()) timer2->stop();
}

void SnakeGame::restart()
{
    if(AI!=nullptr)
        changeState(AIRunning);
    else changeState(running);
}

void SnakeGame::save()
{
    QFile file("data.txt");
    if(file.open(QFile::WriteOnly|QIODevice::Truncate))
    {
        QTextStream out(&file);
        out<<snakesNum<<endl;
        for (int i=0;i<snakesNum;++i)
        {
            out<<snakes[i].life<<' '<<snakes[i].direction<<' '<<snakes[i].speed<<' '<<snakes[i].score<<endl;
            out<<snakes[i].size()<<endl;
            for (int j=0;j<snakes[i].size();++j)
            {
                out<<snakes[i][j].x()<<' '<<snakes[i][j].y()<<endl;
            }
        }

        out<<foods.size()<<endl;
        for (int i=0;i<foods.size();++i)
        {
            out<<foods[i].x()<<' '<<foods[i].y()<<' '<<foods[i].retType()<<endl;
        }

        out<<wall.size()<<endl;
        for(int i=0;i<wall.size();++i)
        {
            out<<wall[i].x()<<' '<<wall[i].y()<<endl;
        }
    }
    else qDebug()<<"Open file failed."<<endl;
}

void SnakeGame::resume()
{
    QFile file("data.txt");
    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        int size;
        in>>snakesNum;
        for (int i=0;i<snakesNum;++i)
        {
            snakes.push_back(Snake(this));
            in>>snakes[i].life>>snakes[i].direction>>snakes[i].speed>>snakes[i].score;
            in>>size;
            for (int j=0;j<size;++j)
            {
                snakes[i].push_back(Coor());
                in>>snakes[i][j].rx()>>snakes[i][j].ry();
            }
        }

        in>>size;
        for (int i=0;i<size;++i)
        {
            foods.push_back(Food());
            in>>foods[i].rx()>>foods[i].ry()>>foods[i].type;
        }

        in>>size;
        for (int i=0;i<size;++i)
        {
            wall.push_back(Coor());
            in>>wall[i].rx()>>wall[i].ry();
        }
    }
    else qDebug()<<"Open file failed."<<endl;

    boardWidget->show();
    toolWidget->show();
    startWidget->close();
    timer1->start(timeInterval/snakes[0].speed);
    if(snakesNum>1)
        timer2->start(timeInterval/snakes[1].speed);
    connect(timer1,&QTimer::timeout,this,&SnakeGame::updateS1);
    if(snakesNum>1) connect(timer2,&QTimer::timeout,this,&SnakeGame::updateS2);
    state=running;
}

void SnakeGame::BackToMenu()
{
    if(state!=over&&state!=AIRunning&&AI==nullptr)
        save();
    clearInformation();
    changeState(welcome);
}

bool SnakeGame::isEmpty(Coor coor)
{
    for(int i=0;i<snakes.size();++i)
        if (snakes[i].contains(coor))
            return false;

    if(AI!=nullptr&&AI->contains(coor)) return false;

    if(foods.contain(coor)>=0) return false;

    if(wall.contains(coor)) return  false;

    return true;
}





