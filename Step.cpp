#include "Step.h"

Step::Step(int fId,int kId,QPoint pF,QPoint pT,QObject *parent) : QObject(parent)
{
    this->fromid = fId;
    this->killid = kId;
    this->pFrom = pF;
    this->pTo = pT;
}
