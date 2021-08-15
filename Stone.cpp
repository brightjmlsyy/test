#include "Stone.h"

Stone::Stone()
{
    r = 20;
}

QPoint Stone::getPos()
{
    QPoint p(this->x,this->y);
    return p;
}

void Stone::setDead()
{
    beDead = true;
    x = -1;
    y = -1;
}

void Stone::setAlive()
{
    beDead = false;
}

void Stone::initStone(int init_id)
{
    id = init_id;
    int tmp = init_id;
    if(id>15)
    {
        tmp = id - 16;
        beRed = true;
    }
    //初始化类型和坐标
    if(tmp<5)
    {
        type = tmp;
        x = (1+tmp)*2*r;
        if(id<16)
        {
            y = 2*r;
        }
        else
        {
            y = 20*r;
        }

    }
    else if(tmp<9)
    {
        type = 8-tmp;
        x = (1+tmp)*2*r;
        if(id<16)
        {
            y = 2*r;
        }
        else
        {
            y = 10*2*r;
        }
    }
    else if(tmp==9 || tmp==10)
    {
        type = 5;
        x = ((tmp-9)*6+2)*2*r;
        if(id<16)
        {
            y = 3*2*r;
        }
        else
        {
            y = 8*2*r;
        }
    }
    else
    {
        type = 6;
        x = ((tmp-10)*2-1)*2*r;
        if(id<16)
        {
            y = 4*2*r;
        }
        else
        {
            y = 7*2*r;
        }
    }
    //初始化名称
    switch (type) {
    case CHE:
        name = "车";
        break;
    case MA:
        name = "马";
        break;
    case XIANG:
        name = "象";
        break;
    case SHI:
        name = "士";
        break;
    case JIANG:
        name = "将";
        break;
    case PAO:
        name = "炮";
        break;
    case BING:
        name = "兵";
        break;
    default:
        break;
    }

}
