#ifndef STONE_H
#define STONE_H
#include<QObject>
#include<QPoint>

enum Type{CHE,MA,XIANG,SHI,JIANG,PAO,BING};

class Stone
{
public:
    Stone();
    int id;
    int type;
    bool beRed=false;
    bool beDead=false;
    QString name;
    int x=0;
    int y=0;
    int r;
    void initStone(int init_id);
    QPoint getPos();
    void setDead();
    void setAlive();
};

#endif // STONE_H
