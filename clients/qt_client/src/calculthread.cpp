#include "calculthread.h"


CalculThread::CalculThread()
{

}

CalculThread::~CalculThread()
{

}

void CalculThread::Slot_startCalcul(QJsonObject args)
{
    _continueCalcul = true;

    long i = 1; //Pour simuler
    while (i > 0 && _continueCalcul)
    {
        //emit calculAborted();
        i++;
    }

    QJsonObject res;
    emit sig_calculDone(res);
}

void CalculThread::Slot_stopCalcul()
{
    _continueCalcul = false;
}
