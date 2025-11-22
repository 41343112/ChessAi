#include "chessai.h"
#include "ui_chessai.h"
#include "chessboard.h"
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QRegularExpression>

ChessAi::ChessAi(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChessAi)
    , engineProcess(nullptr)
    , currentPosition("startpos")
    , isEngineReady(false)
    , chessBoard(nullptr)
{
    ui->setupUi(this);
    
    // Create and setup chess board
    chessBoard = new ChessBoard(this);
    ui->boardLayout->addWidget(chessBoard);
    
    // Connect UI signals to slots
    connect(ui->btnNewGame, &QPushButton::clicked, this, &ChessAi::startNewGame);
    connect(ui->btnMakeMove, &QPushButton::clicked, this, &ChessAi::makeMove);
    connect(chessBoard, &ChessBoard::moveSelected, this, &ChessAi::onBoardMoveSelected);
    
    // Initialize the chess engine
    initializeEngine();
}

ChessAi::~ChessAi()
{
    if (engineProcess) {
        engineProcess->write("quit\n");
        engineProcess->waitForFinished(1000);
        delete engineProcess;
    }
    delete ui;
}

QString ChessAi::getEnginePath()
{
    // Get the application directory
    QString appDir = QApplication::applicationDirPath();
    QString enginePath;
    
    // List of possible engine names to check
    QStringList possibleEngines;
    
    #ifdef Q_OS_WIN
        possibleEngines << "engine/stockfish-windows-x86-64-avx2.exe"
                       << "engine/stockfish.exe"
                       << "engine/stockfish-windows.exe";
    #elif defined(Q_OS_MAC)
        possibleEngines << "engine/stockfish-macos"
                       << "engine/stockfish";
    #else
        possibleEngines << "engine/stockfish-linux"
                       << "engine/stockfish";
    #endif
    
    // Check each possible path
    for (const QString& engineName : possibleEngines) {
        QString path = appDir + "/" + engineName;
        if (QFile::exists(path)) {
            enginePath = path;
            break;
        }
    }
    
    if (enginePath.isEmpty() && !possibleEngines.isEmpty()) {
        // Default to first option if none found
        enginePath = appDir + "/" + possibleEngines.first();
    }
    
    qDebug() << "Engine path:" << enginePath;
    return enginePath;
}

void ChessAi::initializeEngine()
{
    engineProcess = new QProcess(this);
    
    // Connect process signals
    connect(engineProcess, &QProcess::readyReadStandardOutput, this, &ChessAi::readEngineOutput);
    connect(engineProcess, &QProcess::errorOccurred, this, &ChessAi::engineError);
    
    QString enginePath = getEnginePath();
    
    if (!QFile::exists(enginePath)) {
        QMessageBox::warning(this, "Engine Not Found", 
            "Chess engine not found at: " + enginePath + 
            "\nPlease make sure the Stockfish engine is in the engine folder.");
        ui->textBrowser->append("Error: Chess engine not found!");
        return;
    }
    
    engineProcess->start(enginePath);
    
    if (!engineProcess->waitForStarted(3000)) {
        QMessageBox::critical(this, "Engine Error", "Failed to start chess engine!");
        ui->textBrowser->append("Error: Failed to start chess engine!");
        return;
    }
    
    ui->textBrowser->append("Chess engine started successfully!");
    
    // Initialize UCI protocol
    sendCommandToEngine("uci");
}

void ChessAi::sendCommandToEngine(const QString& command)
{
    if (engineProcess && engineProcess->state() == QProcess::Running) {
        qDebug() << "Sending to engine:" << command;
        ui->textBrowser->append(">> " + command);
        engineProcess->write((command + "\n").toUtf8());
    }
}

void ChessAi::readEngineOutput()
{
    if (!engineProcess) return;
    
    while (engineProcess->canReadLine()) {
        QString line = QString::fromUtf8(engineProcess->readLine()).trimmed();
        qDebug() << "Engine output:" << line;
        ui->textBrowser->append("<< " + line);
        
        if (line == "uciok") {
            isEngineReady = true;
            ui->textBrowser->append("Engine is ready!");
        }
        else if (line.startsWith("bestmove")) {
            // Extract the best move
            QStringList parts = line.split(" ");
            if (parts.size() >= 2) {
                QString move = parts[1];
                ui->textBrowser->append("==> Engine's move: " + move);
                
                // Update the chess board with engine's move
                if (chessBoard) {
                    chessBoard->makeMove(move);
                }
                
                // Add engine's move to position tracking
                if (currentPosition == "startpos") {
                    currentPosition = "startpos moves " + move;
                } else {
                    currentPosition += " " + move;
                }
            }
        }
    }
}

void ChessAi::engineError(QProcess::ProcessError error)
{
    QString errorMsg;
    switch (error) {
        case QProcess::FailedToStart:
            errorMsg = "Failed to start engine";
            break;
        case QProcess::Crashed:
            errorMsg = "Engine crashed";
            break;
        default:
            errorMsg = "Engine error occurred";
            break;
    }
    
    ui->textBrowser->append("Error: " + errorMsg);
    QMessageBox::critical(this, "Engine Error", errorMsg);
}

void ChessAi::startNewGame()
{
    if (!isEngineReady) {
        QMessageBox::warning(this, "Not Ready", "Chess engine is not ready yet!");
        return;
    }
    
    currentPosition = "startpos";
    ui->textBrowser->append("\n=== New Game Started ===");
    ui->lineEditMove->clear();
    
    // Reset the chess board
    if (chessBoard) {
        chessBoard->resetBoard();
    }
    
    // Reset the engine to new game state
    sendCommandToEngine("ucinewgame");
    sendCommandToEngine("isready");
}

void ChessAi::makeMove()
{
    if (!isEngineReady) {
        QMessageBox::warning(this, "Not Ready", "Chess engine is not ready yet!");
        return;
    }
    
    QString userMove = ui->lineEditMove->text().trimmed();
    
    if (userMove.isEmpty()) {
        QMessageBox::warning(this, "Invalid Move", "Please enter a move in UCI format (e.g., e2e4)");
        return;
    }
    
    // Validate UCI move format
    if (!isValidUCIMove(userMove)) {
        QMessageBox::warning(this, "Invalid Move", 
            "Invalid UCI move format. Examples:\n"
            "- Normal move: e2e4\n"
            "- Pawn promotion: e7e8q (q for queen, r for rook, b for bishop, n for knight)");
        return;
    }
    
    // Update the chess board with user's move
    if (chessBoard) {
        if (!chessBoard->makeMove(userMove)) {
            QMessageBox::warning(this, "Invalid Move", "Could not make the move on the board.");
            return;
        }
    }
    
    // Append the user's move to the position
    if (currentPosition == "startpos") {
        currentPosition = "startpos moves " + userMove;
    } else {
        currentPosition += " " + userMove;
    }
    
    ui->textBrowser->append("Your move: " + userMove);
    
    // Send position to engine and ask for best move
    sendCommandToEngine("position " + currentPosition);
    sendCommandToEngine("go movetime 1000");  // Think for 1 second
    
    ui->lineEditMove->clear();
}

void ChessAi::onBoardMoveSelected(const QString& uciMove)
{
    if (!isEngineReady) {
        QMessageBox::warning(this, "Not Ready", "Chess engine is not ready yet!");
        return;
    }
    
    // Validate the move
    if (!isValidUCIMove(uciMove)) {
        QMessageBox::warning(this, "Invalid Move", "The selected move is not valid.");
        chessBoard->clearSelection();
        return;
    }
    
    // Update the chess board with user's move
    if (chessBoard) {
        if (!chessBoard->makeMove(uciMove)) {
            QMessageBox::warning(this, "Invalid Move", "Could not make the move on the board.");
            return;
        }
    }
    
    // Append the user's move to the position
    if (currentPosition == "startpos") {
        currentPosition = "startpos moves " + uciMove;
    } else {
        currentPosition += " " + uciMove;
    }
    
    ui->textBrowser->append("Your move: " + uciMove);
    
    // Send position to engine and ask for best move
    sendCommandToEngine("position " + currentPosition);
    sendCommandToEngine("go movetime 1000");  // Think for 1 second
}

bool ChessAi::isValidUCIMove(const QString& move)
{
    // UCI move format: [file][rank][file][rank][promotion]
    // file: a-h, rank: 1-8
    // promotion (optional): q, r, b, n (queen, rook, bishop, knight)
    // Examples: e2e4, e7e8q, a7a8r
    
    QRegularExpression uciPattern("^[a-h][1-8][a-h][1-8][qrbn]?$");
    return uciPattern.match(move).hasMatch();
}
