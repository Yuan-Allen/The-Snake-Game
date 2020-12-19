#ifndef BORADWIDGET_H
#define BORADWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include "foods.h"
#include "wall.h"
#include "snake.h"
#include "startwidget.h"
#include <QTimer>


class BoardWidget : public QWidget
{
    Q_OBJECT
private:
    class SnakeGame *game;
    QTimer *timer;    //计时器

    void initialize();
public:
    explicit BoardWidget(SnakeGame *g, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void paintSnake(QPainter *p);
    void paintFoods(QPainter *p);
    void paintWall(QPainter *p);
    void paintCursor(QPainter *p);
signals:

public slots:
};

#endif // BORADWIDGET_H
