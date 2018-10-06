#ifndef __MOVE_HPP
#define __MOVE_HPP

#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include "piece.hpp"

class Move {
  public:
    enum MoveType {MOVE, OO, OOO, PASSANT, PROMOTE};
    Piece piece;
    std::pair<int, int> from;
    std::pair<int, int> to;
    Piece takes;
    Piece promotes;
    bool is_reverse;
    Move():
        piece(Piece::NONE),
        from(std::make_pair(0, 0)),
        to(std::make_pair(0, 0)),
        takes(Piece::NONE),
        promotes(Piece::NONE),
        is_reverse(false) {}
    Move (
        Piece _piece,
        std::pair<int, int> _from,
        std::pair<int, int> _to,
        Piece _takes=Piece::NONE,
        Piece _promotes=Piece::NONE
    ):
        piece(_piece),
        from(_from),
        to(_to),
        takes(_takes),
        promotes(_promotes),
        is_reverse(false) {}
    void reverse() {
        std::swap(from, to);
        is_reverse = !is_reverse;
    }
    friend std::ostream& operator<<(std::ostream& xx, const Move& move);

};
std::ostream& operator<<(std::ostream& xx, const Move& move) {
    xx << move.piece
       << static_cast<char>('a' + move.from.first) << 1 + move.from.second;

    if (move.takes.type != Piece::NONE) {
        xx << 'x' << move.takes;
    }

    xx << static_cast<char>('a' + move.to.first) << 1 + move.to.second;

    if (move.promotes.type != Piece::NONE) {
        xx << '=' << move.promotes;
    }
    return xx;
}

#endif
