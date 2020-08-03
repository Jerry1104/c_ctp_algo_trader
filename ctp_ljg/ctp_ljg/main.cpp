#include "ctp_algo_trade.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ctp_algo_trade w;
    w.show();
    return a.exec();
}
