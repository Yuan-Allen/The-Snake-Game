#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QPainter>

enum State{welcome,running,pause,over,AIRunning,edit};

class StartWidget : public QWidget
{
    Q_OBJECT
public:
    QPushButton *startBotton;
    QPushButton *multiPlayBotton;
    QPushButton *AIBotton;
    QPushButton *resumeBotton;
    QPushButton *exitBotton;


    QTimer *timer;
    explicit StartWidget(QWidget *parent = nullptr);

signals:

public :
    //void paintEvent(QPaintEvent *event);
};

#endif // STARTWIDGET_H
