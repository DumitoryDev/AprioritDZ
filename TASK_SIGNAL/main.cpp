#include <QCoreApplication>
#include <iostream>
#include <iterator>
#include "counter.h"

int main(int argc, char *argv[])
{

    try{

        QCoreApplication a(argc, argv);
        Counter c1,c2;

        QObject::connect(
                    &c1,
                    SIGNAL(ValueChanged(int)),
                    &c2,
                    SLOT(SetValue(int)));


        std::wcout<<L"Value counter: \n";
        std::wcout<<L"Object first - "<<c1.get()<<"\n";
        std::wcout<<L"Object second - "<<c2.get()<<"\n";

        std::wcout<<L"Type new value for first object...\n";
        c1.SetValue(*std::istream_iterator<int>(std::cin));


        std::wcout<<L"Value counter: \n";
        std::wcout<<L"Object first - "<<c1.get()<<"\n";
        std::wcout<<L"Object second - "<<c2.get()<<"\n";


        return a.exec();

    }
    catch(...){
        std::wcout<<L"Error!\n";
    }

}
