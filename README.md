# ChessAi

A Qt-based chess application with a visual chess board interface that uses the Stockfish engine as an opponent.

## Features

- **Visual Chess Board**: Interactive 8x8 board with Unicode chess pieces
- **Click-to-Move**: Select and move pieces by clicking on the board
- **Real-time Visual Feedback**: Highlights for selected pieces and last moves
- Play chess against the powerful Stockfish chess engine
- UCI (Universal Chess Interface) protocol implementation
- Dual input methods: Click on board or type UCI notation
- Simple and intuitive user interface
- Real-time communication with the chess engine
- Move validation to ensure proper UCI format

## Requirements

- Qt 5.x or Qt 6.x
- Stockfish chess engine (included in the `engine` folder)

## Building the Application

Using qmake:
```bash
qmake ChessAi.pro
make
```

Or open `ChessAi.pro` in Qt Creator and build from there.

## How to Use

1. Launch the application
2. Click "New Game" to start a new chess game
3. Make your moves using either method:
   - **Visual Method** (Recommended): Click on a piece to select it, then click on the destination square
   - **Text Method**: Enter your moves in UCI notation format (e.g., `e2e4`) and click "Make Move"
4. The engine will respond with its best move after thinking for 1 second
5. Continue alternating moves with the engine
6. The visual board updates in real-time showing both your moves and the engine's responses

## Visual Interface

The chess board features:
- 8x8 grid with alternating light and dark squares
- Unicode chess symbols for all pieces (♔ ♕ ♖ ♗ ♘ ♙ for white, ♚ ♛ ♜ ♝ ♞ ♟ for black)
- Yellow highlighting for selected pieces
- Yellow overlay for the last move made
- Coordinate labels (a-h files and 1-8 ranks)
- Automatic pawn promotion to queen

### UCI Move Format

- Standard move: `[source square][destination square]`
  - Example: `e2e4` (move piece from e2 to e4)
- Pawn promotion: `[source square][destination square][promotion piece]`
  - Example: `e7e8q` (promote pawn to queen)
  - Promotion pieces: `q` (queen), `r` (rook), `b` (bishop), `n` (knight)

## Project Structure

- `main.cpp` - Application entry point
- `chessai.h/cpp` - Main window implementation with engine integration
- `chessboard.h/cpp` - Chess board widget with visual rendering and interaction
- `chessai.ui` - Qt Designer UI file
- `engine/` - Stockfish chess engine executables
- `INTERFACE_DOCUMENTATION.md` - Detailed documentation of the visual interface

## Implementation Details

The application uses QProcess to communicate with the Stockfish engine through the UCI protocol:
- Initializes the engine with `uci` command
- Starts new games with `ucinewgame`
- Sends positions with `position startpos moves ...`
- Requests moves with `go movetime 1000` (1 second thinking time)
- Receives best moves from the engine

The visual chess board is implemented as a custom QWidget that:
- Maintains internal board state synchronized with the engine
- Converts click events to UCI notation
- Updates visual display based on move history
- Provides real-time feedback with highlighting

All communication is displayed in the text browser for transparency and debugging.