#include "chessai.h"
#include "ui_chessai.h"

ChessAi::ChessAi(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChessAi)
{
    ui->setupUi(this);
}

ChessAi::~ChessAi()
{
    delete ui;
}
