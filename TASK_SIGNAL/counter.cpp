#include "counter.h"

Counter::Counter(QObject *parent) : QObject(parent)
{

}


void Counter::SetValue(int iNewValue){

    this->iValue_= iNewValue;

    emit this->ValueChanged(this->iValue_);

}
