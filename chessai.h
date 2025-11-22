#ifndef CHESSAI_H
#define CHESSAI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChessAi;
}
QT_END_NAMESPACE

class ChessAi : public QMainWindow
{
    Q_OBJECT

public:
    ChessAi(QWidget *parent = nullptr);
    ~ChessAi();

private:
    Ui::ChessAi *ui;
};
#endif // CHESSAI_H
