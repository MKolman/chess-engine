# Chess engine

A brute force engine that checks all possible moves up to depth `depth` and
returns the best moves.

The game is played as a normal chess game but ends when the king is captured.
There are also no restrictions on a king ending it's turn in check.

## TODO
* En passant pawn capture
* Castling
* Check and checkmate

# Usage

## Dependencies
A g++ compiler with C++11 support.

## Build
```
git clone git@github.com:MKolman/chess-engine.git
cd chess-engine
mkdir build
cd build
cmake ..
make play
```

## Run
After building you can run the `play` binary. You can see `./play --help`
for more info.
