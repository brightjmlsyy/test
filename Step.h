#ifndef STEP_H
#define STEP_H

#include <QObject>
#include <QPoint>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(int fId,int kId,QPoint pF,QPoint pT,QObject *parent = nullptr);
    int fromid;
    int killid;
    QPoint pFrom;
    QPoint pTo;

signals:

public slots:
};

#endif // STEP_H
