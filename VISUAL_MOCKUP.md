# Chess Interface Visual Mockup

```
┌────────────────────────────────────────────────────────────────────────────────┐
│  ChessAi - Stockfish Engine                                         ⊗ ⊡ ⊟    │
├────────────────────────────────────────────────────────────────────────────────┤
│                                                                                │
│  ┌─────────────────────────┐    ┌──────────────────────────────────────────┐ │
│  │     Chess Board         │    │  Chess with Stockfish Engine             │ │
│  │                         │    ├──────────────────────────────────────────┤ │
│  │   a  b  c  d  e  f  g  h│    │ ┌────────────────────────────────────┐   │ │
│  │ 8 ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜       │    │ │ Chess engine started successfully! │   │ │
│  │ 7 ♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟       │    │ │ >> uci                             │   │ │
│  │ 6 · · · · · · · ·       │    │ │ << Stockfish 16 by T. Romstad...   │   │ │
│  │ 5 · · · · · · · ·       │    │ │ << uciok                           │   │ │
│  │ 4 · · · · · · · ·       │    │ │ Engine is ready!                   │   │ │
│  │ 3 · · · · · · · ·       │    │ │                                    │   │ │
│  │ 2 ♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙       │    │ │ === New Game Started ===          │   │ │
│  │ 1 ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖       │    │ │ Your move: e2e4                    │   │ │
│  │                         │    │ │ >> position startpos moves e2e4    │   │ │
│  │  (Click pieces to move) │    │ │ >> go movetime 1000                │   │ │
│  └─────────────────────────┘    │ │ << bestmove e7e5                   │   │ │
│                                  │ │ ==> Engine's move: e7e5            │   │ │
│                                  │ └────────────────────────────────────┘   │ │
│                                  │                                          │ │
│                                  │ Enter Move (UCI format): [e2e4____] [Make Move]│
│                                  │                                          │ │
│                                  │          [New Game]                      │ │
│                                  │                                          │ │
│                                  │ Instructions: Click "New Game" to start. │ │
│                                  │ Either click pieces on the board to move │ │
│                                  │ them, or enter moves in UCI format.      │ │
│                                  └──────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────────────────────────────┘

Legend:
  - Left side: Interactive chess board with visual pieces
  - Right side: Engine communication log and controls
  - Yellow highlighting (not shown): Selected pieces and last moves
  - Board squares alternate between light beige and dark brown
  - Unicode symbols show white pieces (♔♕♖♗♘♙) and black pieces (♚♛♜♝♞♟)
```

## Interaction Flow

1. **Game Start**:
   - User clicks "New Game"
   - Board resets to starting position
   - Engine initializes

2. **Making a Move (Visual)**:
   - User clicks on a white piece (e.g., the pawn at e2)
   - Piece is highlighted with yellow border
   - User clicks destination square (e.g., e4)
   - Move is converted to UCI notation (e2e4)
   - Move is sent to engine
   - Board updates visually

3. **Engine Response**:
   - Engine calculates best move (e.g., e7e5)
   - Move appears in log: "==> Engine's move: e7e5"
   - Board automatically updates showing engine's move
   - Last move squares (e7 and e5) are highlighted in yellow

4. **Continuing Play**:
   - User makes next move (click or type)
   - Cycle continues until game ends

## Visual Features

### Board Colors
- Light squares: #F0D9B5 (beige)
- Dark squares: #B58863 (brown)

### Highlights
- Selected piece: Yellow border (255, 255, 0, 200)
- Last move: Yellow overlay (255, 255, 0, 60) on both from and to squares

### Piece Display
- Font size: Proportional to square size (2/3 of square size)
- White pieces: White color with outline
- Black pieces: Black color with outline
- Centered in each square
