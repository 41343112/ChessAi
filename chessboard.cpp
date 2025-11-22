#include "chessboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <cctype>
#include<QPainterPath>
ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
    , pieceSelected(false)
    , selectedSquare{-1, -1}
    , lastMoveFrom{-1, -1}
    , lastMoveTo{-1, -1}
{
    setMinimumSize(400, 400);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    initializeBoard();
}

void ChessBoard::initializeBoard()
{
    // Initialize all squares to empty
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = ' ';
        }
    }
    
    // Set up initial position
    // Black pieces (row 0 and 1)
    board[0][0] = 'r'; board[0][1] = 'n'; board[0][2] = 'b'; board[0][3] = 'q';
    board[0][4] = 'k'; board[0][5] = 'b'; board[0][6] = 'n'; board[0][7] = 'r';
    for (int col = 0; col < 8; ++col) {
        board[1][col] = 'p';
    }
    
    // White pieces (row 6 and 7)
    for (int col = 0; col < 8; ++col) {
        board[6][col] = 'P';
    }
    board[7][0] = 'R'; board[7][1] = 'N'; board[7][2] = 'B'; board[7][3] = 'Q';
    board[7][4] = 'K'; board[7][5] = 'B'; board[7][6] = 'N'; board[7][7] = 'R';
}

void ChessBoard::resetBoard()
{
    initializeBoard();
    pieceSelected = false;
    selectedSquare = {-1, -1};
    lastMoveFrom = {-1, -1};
    lastMoveTo = {-1, -1};
    moveHistory.clear();
    update();
}

int ChessBoard::squareSize() const
{
    int w = width();
    int h = height();
    int minDim = qMin(w, h);
    return minDim / 8;
}

void ChessBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    drawBoard(painter);
    drawLastMove(painter);
    drawSelection(painter);
    drawPieces(painter);
}

void ChessBoard::drawBoard(QPainter& painter)
{
    int sz = squareSize();
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            // Alternate colors: light squares on bottom-right
            bool isLight = (row + col) % 2 == 0;
            painter.fillRect(col * sz, row * sz, sz, sz, 
                           isLight ? QColor(240, 217, 181) : QColor(181, 136, 99));
        }
    }
    
    // Draw coordinate labels
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    
    QFontMetrics fm(font);
    int labelOffset = qMax(5, sz / 20);
    
    // Files (a-h) - drawn at bottom of board
    for (int col = 0; col < 8; ++col) {
        QString label = QString(QChar('a' + col));
        int textWidth = fm.horizontalAdvance(label);
        painter.drawText(col * sz + sz - textWidth - labelOffset, 
                        8 * sz - labelOffset, label);
    }
    
    // Ranks (1-8) - drawn at left of board
    for (int row = 0; row < 8; ++row) {
        QString label = QString::number(8 - row);
        painter.drawText(labelOffset, row * sz + fm.height(), label);
    }
}

void ChessBoard::drawPieces(QPainter& painter)
{
    int sz = squareSize();
    QFont font = painter.font();
    font.setPointSize(sz * 2 / 3);
    painter.setFont(font);
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            char piece = board[row][col];
            if (piece != ' ') {
                QString symbol = getPieceSymbol(piece);
                
                // Draw piece with stroke for better visibility
                QRect rect(col * sz, row * sz, sz, sz);
                
                if (isWhitePiece(piece)) {
                    // White pieces: white fill with dark outline
                    QPainterPath path;
                    path.addText(rect.center(), font, symbol);
                    QRectF bounds = path.boundingRect();
                    path.translate(rect.center() - bounds.center());
                    
                    painter.setPen(QPen(Qt::black, 3));
                    painter.setBrush(Qt::white);
                    painter.drawPath(path);
                } else {
                    // Black pieces: black fill with light outline for better visibility
                    QPainterPath path;
                    path.addText(rect.center(), font, symbol);
                    QRectF bounds = path.boundingRect();
                    path.translate(rect.center() - bounds.center());
                    
                    painter.setPen(QPen(QColor(200, 200, 200), 2));
                    painter.setBrush(Qt::black);
                    painter.drawPath(path);
                }
            }
        }
    }
}

void ChessBoard::drawSelection(QPainter& painter)
{
    if (pieceSelected && selectedSquare.isValid()) {
        int sz = squareSize();
        painter.setPen(QPen(QColor(255, 255, 0, 200), 3));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(selectedSquare.col * sz + 2, selectedSquare.row * sz + 2, 
                        sz - 4, sz - 4);
    }
}

void ChessBoard::drawLastMove(QPainter& painter)
{
    if (lastMoveFrom.isValid() && lastMoveTo.isValid()) {
        int sz = squareSize();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 255, 0, 60));
        painter.drawRect(lastMoveFrom.col * sz, lastMoveFrom.row * sz, sz, sz);
        painter.drawRect(lastMoveTo.col * sz, lastMoveTo.row * sz, sz, sz);
    }
}

ChessBoard::Square ChessBoard::pixelToSquare(const QPoint& pos) const
{
    int sz = squareSize();
    Square sq;
    sq.col = pos.x() / sz;
    sq.row = pos.y() / sz;
    
    if (sq.col >= 0 && sq.col < 8 && sq.row >= 0 && sq.row < 8) {
        return sq;
    }
    return {-1, -1};
}

QString ChessBoard::squareToUCI(const Square& sq) const
{
    if (!sq.isValid()) return "";
    
    char file = 'a' + sq.col;
    char rank = '1' + (7 - sq.row);
    return QString("%1%2").arg(file).arg(rank);
}

ChessBoard::Square ChessBoard::uciToSquare(const QString& uci, int offset) const
{
    if (uci.length() < offset + 2) {
        return {-1, -1};
    }
    
    char file = uci[offset].toLatin1();
    char rank = uci[offset + 1].toLatin1();
    
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return {-1, -1};
    }
    
    Square sq;
    sq.col = file - 'a';
    sq.row = 7 - (rank - '1');
    return sq;
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    
    Square clickedSquare = pixelToSquare(event->pos());
    if (!clickedSquare.isValid()) {
        return;
    }
    
    if (!pieceSelected) {
        // Try to select a piece
        char piece = getPieceAt(clickedSquare.row, clickedSquare.col);
        if (piece != ' ') {
            pieceSelected = true;
            selectedSquare = clickedSquare;
            update();
        }
    } else {
        // Try to move the selected piece
        QString fromUCI = squareToUCI(selectedSquare);
        QString toUCI = squareToUCI(clickedSquare);
        QString move = fromUCI + toUCI;
        
        // Check for pawn promotion (simplified - always promote to queen)
        char piece = getPieceAt(selectedSquare.row, selectedSquare.col);
        if ((piece == 'P' && clickedSquare.row == 0) || 
            (piece == 'p' && clickedSquare.row == 7)) {
            move += 'q';
        }
        
        qDebug() << "Move generated:" << move;
        
        // Clear selection
        pieceSelected = false;
        selectedSquare = {-1, -1};
        
        // Emit the move signal
        emit moveSelected(move);
        
        update();
    }
}

bool ChessBoard::makeMove(const QString& uciMove)
{
    if (uciMove.length() < 4) {
        qDebug() << "Invalid move length:" << uciMove;
        return false;
    }
    
    Square from = uciToSquare(uciMove, 0);
    Square to = uciToSquare(uciMove, 2);
    
    if (!from.isValid() || !to.isValid()) {
        qDebug() << "Invalid squares in move:" << uciMove;
        return false;
    }
    
    char piece = getPieceAt(from.row, from.col);
    if (piece == ' ') {
        qDebug() << "No piece at source square:" << uciMove;
        return false;
    }
    
    // Handle promotion
    if (uciMove.length() >= 5) {
        char promotion = uciMove[4].toLatin1();
        if (isWhitePiece(piece)) {
            piece = toupper(promotion);
        } else {
            piece = tolower(promotion);
        }
    }
    
    // Make the move
    setPieceAt(from.row, from.col, ' ');
    setPieceAt(to.row, to.col, piece);
    
    // Update last move highlighting
    lastMoveFrom = from;
    lastMoveTo = to;
    
    // Add to move history
    moveHistory.append(uciMove);
    
    update();
    return true;
}

char ChessBoard::getPieceAt(int row, int col) const
{
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return board[row][col];
    }
    return ' ';
}

void ChessBoard::setPieceAt(int row, int col, char piece)
{
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        board[row][col] = piece;
    }
}

bool ChessBoard::isWhitePiece(char piece) const
{
    return piece >= 'A' && piece <= 'Z';
}

bool ChessBoard::isBlackPiece(char piece) const
{
    return piece >= 'a' && piece <= 'z';
}

QString ChessBoard::getPieceSymbol(char piece) const
{
    // Unicode chess symbols
    switch (piece) {
        case 'K': return QString::fromUtf8("\u2654"); // White King
        case 'Q': return QString::fromUtf8("\u2655"); // White Queen
        case 'R': return QString::fromUtf8("\u2656"); // White Rook
        case 'B': return QString::fromUtf8("\u2657"); // White Bishop
        case 'N': return QString::fromUtf8("\u2658"); // White Knight
        case 'P': return QString::fromUtf8("\u2659"); // White Pawn
        case 'k': return QString::fromUtf8("\u265A"); // Black King
        case 'q': return QString::fromUtf8("\u265B"); // Black Queen
        case 'r': return QString::fromUtf8("\u265C"); // Black Rook
        case 'b': return QString::fromUtf8("\u265D"); // Black Bishop
        case 'n': return QString::fromUtf8("\u265E"); // Black Knight
        case 'p': return QString::fromUtf8("\u265F"); // Black Pawn
        default: return "";
    }
}

void ChessBoard::clearSelection()
{
    pieceSelected = false;
    selectedSquare = {-1, -1};
    update();
}

QString ChessBoard::getBoardFEN() const
{
    // Simplified FEN generation (just piece positions)
    QString fen;
    for (int row = 0; row < 8; ++row) {
        int emptyCount = 0;
        for (int col = 0; col < 8; ++col) {
            char piece = board[row][col];
            if (piece == ' ') {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += QString::number(emptyCount);
                    emptyCount = 0;
                }
                fen += piece;
            }
        }
        if (emptyCount > 0) {
            fen += QString::number(emptyCount);
        }
        if (row < 7) {
            fen += '/';
        }
    }
    return fen;
}

QSize ChessBoard::sizeHint() const
{
    return QSize(480, 480);
}

QSize ChessBoard::minimumSizeHint() const
{
    return QSize(320, 320);
}
