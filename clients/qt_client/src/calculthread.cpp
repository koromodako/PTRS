#include "calculthread.h"


CalculThread::CalculThread()
{

}

CalculThread::~CalculThread()
{

}

void CalculThread::StartCalcul(QJsonObject args)
{
    _continueCalcul = true;

    long i = 1; //Pour simuler
    while (i > 0 && _continueCalcul)
    {
        //emit calculAborted();
        i++;
    }

    QJsonObject res;
    emit calculDone(res);
}

void CalculThread::StopCalcul()
{
    _continueCalcul = false;
}
