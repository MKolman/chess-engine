#ifndef __PIECE_HPP
#define __PIECE_HPP

#include <iostream>

class Piece {
  public:
    typedef enum {K=1, Q=2, R=3, N=4, B=5, P=6, NONE=7} PieceType;
    PieceType type;
    bool is_white;
    int worth;
    Piece(PieceType _type, bool _is_white=true) {
        type = _type;
        is_white = _is_white;
        switch (type) {
            case K: worth = 1000; break;
            case Q: worth = 9; break;
            case R: worth = 5; break;
            case N: worth = 3; break;
            case B: worth = 3; break;
            case P: worth = 1; break;
            case NONE: worth = 0; break;
        };
    }
    bool operator==(const Piece& piece) {
        return type == piece.type && is_white == piece.is_white;
    }
    bool operator!=(const Piece& piece) {
        return !(*this == piece);
    }
    friend std::ostream& operator<<(std::ostream& xx, const Piece& Piece);
};

std::ostream& operator<<(std::ostream& xx, const Piece& piece) {
    if (piece.is_white) xx << "KQRNBP."[piece.type-1];
    else xx << "kqrnbp."[piece.type-1];
    return xx;
}

#endif
