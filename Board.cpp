#include "Board.h"
#include <QDebug>
#include <QPushButton>
#include <QtAlgorithms>

Board::Board(QWidget *parent) : QWidget(parent)
{
    computeLevel = 2;
    r = 20;
    resize(QSize(23*r,23*r));
    QPushButton *backAstep = new QPushButton(this);
    backAstep->resize(QSize(3*r,2*r));
    backAstep->setText("悔棋");
    backAstep->move(9.5*2*r,5*2*r);
    connect(backAstep,&QPushButton::released,this,&Board::back);
    for(int i=0;i<32;i++)
    {
        s[i].initStone(i);
    }
}

void Board::back()
{
    if(steps.isEmpty())
        return;
    Step *st = steps.pop();
    s[st->fromid].x = st->pFrom.x();
    s[st->fromid].y = st->pFrom.y();
    if(st->killid != -1)
    {
        s[st->killid].x = st->pTo.x();
        s[st->killid].y = st->pTo.y();
        s[st->killid].beDead = false;
    }
    beRedTurn = !beRedTurn;
    update();
    delete st;
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 2*r;
    //绘制棋盘
    for(int i=1;i<=10;i++)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    for(int i=1;i<10;i++)
    {
        if(i==1 || i==9)
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        }
        else
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(4*d,3*d),QPoint(6*d,d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(4*d,10*d),QPoint(6*d,8*d));
    //绘制棋子
    painter.setFont(QFont("system", r*2, 800));
    for(int i=0;i<32;i++)
    {
        if(s[i].beDead)
        {
            continue;
        }
        QColor color;
        if(s[i].beRed) color = Qt::red;
        else color = Qt::black;
        painter.setPen(QPen(QBrush(color), 2));
        if(s[i].id == selectId) painter.setBrush(Qt::gray);
            else painter.setBrush(Qt::yellow);
        painter.drawEllipse(QPoint(s[i].x,s[i].y),s[i].r,s[i].r);
        QRectF f(s[i].x-s[i].r,s[i].y-s[i].r,2*r,2*r);
        painter.drawText(f,s[i].name,QTextOption(Qt::AlignCenter));
    }
}

//鼠标点击事件
void Board::mouseReleaseEvent(QMouseEvent *event)
{
    if(beRedTurn)
    {
        QPoint p = getClickCenter(event->pos());
        if(!checkPosIsValid(p))
        {
            qDebug() << "invalid position";
            return;
        }

        int checkid = checkIsStone(p);
        qDebug() << checkid;
        if(checkid != -1 && canSelect(checkid))
        {
            selectStone(checkid);
        }
        else if(selectId != -1 && canMove(selectId,p))
        {
            moveStone(selectId,p);
            if(!beRedTurn)
            {
                qDebug() << "12345";
                QTimer::singleShot(100, this, SLOT(computerMove()));
            }
        }
    }
}

//判断点击位置是否在棋盘内
bool Board::checkPosIsValid(QPoint pt)
{
    if(pt.x()<40 || pt.x() > 360 || pt.y() < 40 || pt.y() > 400)
    {
        return false;

    }
    return true;
}

//选定一颗棋子
void Board::selectStone(int id)
{
    if(canSelect(id))
    {
        selectId = id;
        update();
    }
    return;
}

//移动棋子
void Board::moveStone(int id,QPoint pt)
{
    int checkid = checkIsStone(pt);
    if(checkid != -1)
    {
        if(s[checkid].beRed != s[id].beRed)
        {
            s[checkid].setDead();
            //stonePos.remove(QPoint(s[checkid].x,s[checkid].y));
        }
        else
        {
            selectStone(checkid);
            return;
        }

    }
    //stonePos.remove(QPoint(s[id].x,s[id].y));
    //stonePos[pt] = id;
    Step *st = new Step(id,checkid,QPoint(s[id].x,s[id].y),pt);
    steps.push_back(st);
    s[id].x = pt.x();
    s[id].y = pt.y();
    selectId = -1;
    beRedTurn = !beRedTurn;
    update();
}
//判断棋子是否可以被选定
bool Board::canSelect(int id)
{
    return beRedTurn == s[id].beRed;
}

QPoint Board::getClickCenter(QPoint pt)
{   QPoint cp(pt/(2*r));
    qDebug() << cp*2*r;
    return cp*2*r;
}
//判断点击位置是否有棋子
int Board::checkIsStone(QPoint pt)
{

    for(int i=0;i<32;i++)
    {
        if(s[i].x == pt.x() && s[i].y == pt.y())
        {
            return s[i].id;
        }
    }

//    auto it = stonePos.find(pt);
//    if(it != stonePos.end())
//    {
//        return it.value();
//    }
    return -1;
}

bool Board::checkPosBeRed(QPoint pt)
{
    return pt.y()>5*2*r;
}

bool Board::canMove(int id,QPoint pt)
{
    switch (s[id].type) {
    case CHE:
        return canMoveChe(id,pt);
        break;
    case MA:
        return canMoveMa(id,pt);
        break;
    case XIANG:
        return canMoveXiang(id,pt);
        break;
    case SHI:
        return canMoveShi(id,pt);
        break;
    case JIANG:
        return canMoveJiang(id,pt);
        break;
    case PAO:
        return canMovePao(id,pt);
        break;
    case BING:
        return canMoveBing(id,pt);
        break;
    default:
        break;
    }

    return true;
}

bool Board::canMovePao(int id,QPoint pt)
{
    if(pt.x() != s[id].x && pt.y() != s[id].y)
    {
        return false;
    }
    if(checkIsStone(pt) != -1)
    {
        if(1 == stonesInPath(id,pt))
        {
            return true;
        }
        return false;
    }
    else
    {
        if(!stonesInPath(id,pt))
        {
            return true;
        }
        return false;
    }
}

bool Board::canMoveChe(int id,QPoint pt)
{
    if(pt.x() != s[id].x && pt.y() != s[id].y)
    {
        return false;
    }

    if(!stonesInPath(id,pt))
    {
        return true;
    }
    return false;

}

bool Board::canMoveMa(int id,QPoint pt)
{
    int re = relation(s[id].x,s[id].y, pt);
    if(re != 12*2*r && re != 21*2*r)
        return false;

    if(re == 12*2*r)
    {
        if(checkIsStone(QPoint(s[id].x, (pt.y()+s[id].y)/2)) != -1)
            return false;
    }
    else
    {
        if(checkIsStone(QPoint(s[id].y, (s[id].x+pt.x())/2)) != -1)
            return false;
    }

    return true;
}

bool Board::canMoveXiang(int id,QPoint pt)
{
    int re = relation(s[id].x,s[id].y, pt);
    if(re != 22*2*r)
        return false;
    int rEye = (s[id].x+pt.x())/2;
    int cEye = (s[id].y+pt.y())/2;
    if(checkIsStone(QPoint(rEye, cEye)) != -1)
    {
        return false;
    }
    if(checkPosBeRed(pt) != s[id].beRed)
    {
        return false;
    }
    return true;
}

bool Board::canMoveShi(int id,QPoint pt)
{
    int re = relation(s[id].x,s[id].y, pt);
    if(re != 11*2*r)
        return false;
    if(pt.x() < 4*2*r || pt.x() > 6*2*r)
    {
        return false;
    }
    if(checkPosBeRed(pt))
    {
        if(pt.y() < 8*2*r)
        {
            return false;
        }
    }
    else
    {
        if(pt.y() > 3*2*r)
        {
            return false;
        }
    }
    return true;
}
bool Board::canMoveJiang(int id,QPoint pt)
{
    int checkid = checkIsStone(pt);
    if(checkid != -1 && s[checkid].type == JIANG)
        return canMoveChe(id,pt);
    int re = relation(s[id].x,s[id].y, pt);
    if(re != 10*2*r && re != 1*2*r)
        return false;
    if(pt.x() < 4*2*r || pt.x() > 6*2*r)
    {
        return false;
    }
    if(checkPosBeRed(pt))
    {
        if(pt.y() < 8*2*r)
        {
            return false;
        }
    }
    else
    {
        if(pt.y() > 3*2*r)
        {
            return false;
        }
    }
    return true;
}
bool Board::canMoveBing(int id,QPoint pt)
{
    int re = relation(s[id].x,s[id].y, pt);
    if(re != 10*2*r && re != 1*2*r)
        return false;
    if(checkPosBeRed(pt) == s[id].beRed)
    {
        if(s[id].beRed)
        {
            if(s[id].y <= pt.y()) return false;
        }
        if(!s[id].beRed)
        {
            if(s[id].y >= pt.y()) return false;
        }
    }
    else
    {
        if(s[id].beRed)
        {
            if(s[id].y < pt.y()) return false;
        }
        if(!s[id].beRed)
        {
            if(s[id].y > pt.y()) return false;
        }
    }
    return true;
}

int Board::relation(int x,int y,QPoint pt)
{
    return qAbs(pt.x() - x)*10 + qAbs(pt.y() - y);
}

int Board::stonesInPath(int id,QPoint pt)
{
    int sNum = 0;
    if(pt.x() == s[id].x)
    {
        int tmp = s[id].y;
        int dis = (pt.y()>s[id].y)?(2*r):(-2*r);
        while(tmp != (pt.y()-dis))
        {
            tmp += dis;
            if(checkIsStone(QPoint(pt.x(),tmp)) != -1)
            {
                sNum ++;
            }

        }
    }
    else
    {
        int tmp = s[id].x;
        int dis = (pt.x()>s[id].x)?(2*r):(-2*r);
        while(tmp != (pt.x()-dis))
        {
            tmp += dis;
            if(checkIsStone(QPoint(tmp,pt.y())) != -1)
            {
                sNum ++;
            }

        }
    }
    //qDebug() << sNum;
    return sNum;
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    Step* step = new Step(moveid,killid,s[moveid].getPos(),QPoint(row,col));
    steps.append(step);
}


//电脑走棋算法
void Board::computerMove()
{
    qDebug() << "computer move";
    Step* step = getBestMove();
    moveStone(step->fromid,step->pTo);
    delete step;
    update();
}

Step* Board::getBestMove()
{
    Step* ret = NULL;
    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int maxInAllMinScore = -300000;

    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int minScore = getMinScore(this->computeLevel-1, maxInAllMinScore);
        unfakeMove(step);

        if(minScore > maxInAllMinScore)
        {
            if(ret) delete ret;

            ret = step;
            maxInAllMinScore = minScore;
        }
        else
        {
            delete step;
        }
    }
    return ret;
}

int Board::score()
{
  //  enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    static int ss[] = {1000, 499, 501, 200, 15000, 100, 100};
    int scoreBlack = 0;
    int scoreRed = 0;
    for(int i=0; i<16; ++i)
    {
        if(s[i].beDead) continue;
        scoreRed += ss[s[i].type];
    }
    for(int i=16; i<32; ++i)
    {
        if(s[i].beDead) continue;
        scoreBlack += ss[s[i].type];
    }
    return scoreBlack - scoreRed;
}


int Board::getMinScore(int level, int curMin)
{
    if(level == 0)
        return score();

    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int minInAllMaxScore = 300000;

    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int maxScore = getMaxScore(level-1, minInAllMaxScore);
        unfakeMove(step);
        delete step;

        if(maxScore <= curMin)
        {
            while(steps.count())
            {
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }


    }
    return minInAllMaxScore;
}
int Board::getMaxScore(int level, int curMax)
{
    if(level == 0)
        return score();

    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int maxInAllMinScore = -300000;

    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int minScore = getMinScore(level-1, maxInAllMinScore);
        unfakeMove(step);
        delete step;

        if(minScore >= curMax)
        {
            while(steps.count())
            {
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return minScore;
        }
        if(minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
        }


    }
    return maxInAllMinScore;
}


void Board::fakeMove(Step *step)
{
    int checkid = checkIsStone(step->pTo);
    if(checkid != -1)
    {
        s[checkid].setDead();
    }
    s[step->fromid].x = step->pTo.x();
    s[step->fromid].y = step->pTo.y();
    beRedTurn = !beRedTurn;
}

void Board::unfakeMove(Step *step)
{
    if(step->killid != -1)
    {
        s[step->killid].setAlive();
        s[step->killid].x = step->pTo.x();
        s[step->killid].y = step->pTo.y();
    }
    s[step->fromid].x = step->pFrom.x();
    s[step->fromid].y = step->pFrom.y();
    beRedTurn = !beRedTurn;
}

void Board::getAllPossibleMove(QVector<Step *> &steps)
{
    int min, max;
    if(this->beRedTurn)
    {
        min = 0, max = 16;
    }
    else
    {
        min = 16, max = 32;
    }

    for(int i=min;i<max; i++)
    {
        if(this->s[i].beDead) continue;
        for(int row = 0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                int killid = checkIsStone(QPoint(row,col));
                if((killid != -1) && (s[i].beRed == s[killid].beRed)) continue;

                if(canMove(i, QPoint(row, col)))
                {
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
}

