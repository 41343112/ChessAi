#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QPair>

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = nullptr);
    
    // Reset board to initial position
    void resetBoard();
    
    // Make a move in UCI format (e.g., "e2e4")
    bool makeMove(const QString& uciMove);
    
    // Get the current board state as FEN notation
    QString getBoardFEN() const;
    
    // Get move history
    QStringList getMoveHistory() const { return moveHistory; }
    
    // Clear selection
    void clearSelection();

signals:
    void moveSelected(const QString& uciMove);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    struct Square {
        int row;
        int col;
        bool operator==(const Square& other) const {
            return row == other.row && col == other.col;
        }
        bool isValid() const {
            return row >= 0 && row < 8 && col >= 0 && col < 8;
        }
    };
    
    // Board state: 8x8 array, each cell contains a piece code
    // Uppercase for white, lowercase for black
    // P/p = pawn, R/r = rook, N/n = knight, B/b = bishop, Q/q = queen, K/k = king
    // ' ' = empty square
    char board[8][8];
    
    // Selection state
    bool pieceSelected;
    Square selectedSquare;
    Square lastMoveFrom;
    Square lastMoveTo;
    
    // Move history
    QStringList moveHistory;
    
    // Helper methods
    void initializeBoard();
    void drawBoard(QPainter& painter);
    void drawPieces(QPainter& painter);
    void drawSelection(QPainter& painter);
    void drawLastMove(QPainter& painter);
    
    Square pixelToSquare(const QPoint& pos) const;
    QString squareToUCI(const Square& sq) const;
    Square uciToSquare(const QString& uci, int offset) const;
    
    char getPieceAt(int row, int col) const;
    void setPieceAt(int row, int col, char piece);
    bool isWhitePiece(char piece) const;
    bool isBlackPiece(char piece) const;
    
    QString getPieceSymbol(char piece) const;
    
    int squareSize() const;
};

#endif // CHESSBOARD_H
