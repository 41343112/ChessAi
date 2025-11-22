# ChessAi

A Qt-based chess application that uses the Stockfish engine as an opponent.

## Features

- Play chess against the powerful Stockfish chess engine
- UCI (Universal Chess Interface) protocol implementation
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
3. Enter your moves in UCI notation format (e.g., `e2e4` to move pawn from e2 to e4)
4. Click "Make Move" to submit your move
5. The engine will respond with its best move after thinking for 1 second
6. Continue alternating moves with the engine

### UCI Move Format

- Standard move: `[source square][destination square]`
  - Example: `e2e4` (move piece from e2 to e4)
- Pawn promotion: `[source square][destination square][promotion piece]`
  - Example: `e7e8q` (promote pawn to queen)
  - Promotion pieces: `q` (queen), `r` (rook), `b` (bishop), `n` (knight)

## Project Structure

- `main.cpp` - Application entry point
- `chessai.h/cpp` - Main window implementation with engine integration
- `chessai.ui` - Qt Designer UI file
- `engine/` - Stockfish chess engine executables

## Implementation Details

The application uses QProcess to communicate with the Stockfish engine through the UCI protocol:
- Initializes the engine with `uci` command
- Starts new games with `ucinewgame`
- Sends positions with `position startpos moves ...`
- Requests moves with `go movetime 1000` (1 second thinking time)
- Receives best moves from the engine

All communication is displayed in the text browser for transparency and debugging.