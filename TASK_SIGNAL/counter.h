#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

class Counter : public QObject
{
    Q_OBJECT
public:
    explicit Counter(QObject *parent = nullptr);

    int get() const noexcept{
        return this->iValue_;
    }

    void set(int iNewValue) noexcept{
        this->iValue_ = iNewValue;
    }

signals:
    void ValueChanged(int iNewValue);
public slots:
    void SetValue(int iNewValue);
private:
    int iValue_{};

};

#endif // COUNTER_H
