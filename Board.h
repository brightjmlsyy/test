#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMap>
#include <QStack>
#include <algorithm>
#include <QPoint>
#include <QTimer>
#include "Stone.h"
#include "Step.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    int r;
    Stone s[32];
    QMap<QPoint,int> stonePos;
    QStack<Step*> steps;
    bool beRedTurn=true;
    int selectId=-1;
    QPoint getClickCenter(QPoint pt);
    bool canSelect(int id);
    void selectStone(int id);
    int checkIsStone(QPoint pt);
    void moveStone(int id,QPoint pt);
    bool checkPosIsValid(QPoint pt);
    bool canMove(int id,QPoint pt);
    bool canMovePao(int id,QPoint pt);
    bool canMoveChe(int id,QPoint pt);
    bool canMoveMa(int id,QPoint pt);
    bool canMoveXiang(int id,QPoint pt);
    bool canMoveShi(int id,QPoint pt);
    bool canMoveJiang(int id,QPoint pt);
    bool canMoveBing(int id,QPoint pt);
    int stonesInPath(int id,QPoint pt);
    bool checkPosBeRed(QPoint pt);
    int relation(int x,int y,QPoint pt);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);

    int computeLevel;

    int score();
    Step* getBestMove();
    int getMinScore(int level, int curMin);
    int getMaxScore(int level, int curMax);
    void fakeMove(Step *step);
    void unfakeMove(Step *step);
    void getAllPossibleMove(QVector<Step *> &steps);


signals:

public slots:
    void back();
    void computerMove();
};

#endif // BOARD_H
