#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ToolWidget : public QWidget
{
    Q_OBJECT
    friend class SnakeGame;
    friend class Snake;
    friend class AISnake;
private:
    class SnakeGame *game;
    QLabel *score1,*score2,*hint;
    QPushButton *restart,*backToWelcome;
public:
    explicit ToolWidget(SnakeGame *g, QWidget *parent = nullptr);

signals:

public slots:
};

#endif // TOOLWIDGET_H
