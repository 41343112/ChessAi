#ifndef CHESSAI_H
#define CHESSAI_H

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChessAi;
}
QT_END_NAMESPACE

class ChessBoard;

class ChessAi : public QMainWindow
{
    Q_OBJECT

public:
    ChessAi(QWidget *parent = nullptr);
    ~ChessAi();

private slots:
    void startNewGame();
    void makeMove();
    void readEngineOutput();
    void engineError(QProcess::ProcessError error);
    void onBoardMoveSelected(const QString& uciMove);

private:
    void initializeEngine();
    void sendCommandToEngine(const QString& command);
    QString getEnginePath();
    bool isValidUCIMove(const QString& move);
    void updateBoardFromMoveHistory();

    Ui::ChessAi *ui;
    QProcess* engineProcess;
    QString currentPosition;
    bool isEngineReady;
    ChessBoard* chessBoard;
};
#endif // CHESSAI_H
