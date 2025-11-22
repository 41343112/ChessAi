#include "chessai.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessAi w;
    w.show();
    return a.exec();
}
