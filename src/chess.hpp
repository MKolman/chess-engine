#ifndef __CHESS_HPP
#define __CHESS_HPP

#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <sstream>

#include "piece.hpp"
#include "move.hpp"
#include "move_gen.hpp"

class Chess {
  public:
    enum GameState {PLAY, WHITE_WIN, BLACK_WIN, DRAW};
    typedef Move MoveType;
    std::vector<std::vector<Piece>> board;
    std::vector<Move> moves;
    int score;
    GameState state;
    MoveGenerator<Chess> move_generator;
    Chess():state(PLAY), move_generator(this) {
        score = 0;
        board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece::NONE));
        Piece::PieceType order[] = {Piece::R, Piece::N, Piece::B, Piece::Q,
                                   Piece::K, Piece::B, Piece::N, Piece::R};
        for (int i = 0; i < 8; i++) {
            board[0][i] = Piece(order[i], true);
            board[1][i] = Piece(Piece::P, true);
            board[6][i] = Piece(Piece::P, false);
            board[7][i] = Piece(order[i], false);
        }
    }
    std::vector<Move> get_all_moves() {
        std::vector<Move> result;
        move_generator = MoveGenerator<Chess>(this);
        if (state != PLAY) return result;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (moves.size() % 2 == board[i][j].is_white) continue;
                get_single_moves(result, i, j);
            }
        }
        return result;
    }
    bool is_mate() {
        for (const Move& first_move : get_all_moves()) {
            if (make_move(first_move)) {
                bool can_take_king = false;
                for (const Move& second_move : get_all_moves()) {
                    if (second_move.takes.type == Piece::K){
                        can_take_king = true;
                        break;
                    }
                }
                reverse_move();
                if (can_take_king == false) return false;
            }
        }
        return true;
    }
    void get_single_moves(std::vector<Move>& result, int i, int j) {
        switch (board[i][j].type) {
            case Piece::K: move_generator.get_king_moves(result, i, j); break;
            case Piece::Q: move_generator.get_queen_moves(result, i, j); break;
            case Piece::R: move_generator.get_rook_moves(result, i, j); break;
            case Piece::N: move_generator.get_knight_moves(result, i, j); break;
            case Piece::B: move_generator.get_bishop_moves(result, i, j); break;
            case Piece::P: move_generator.get_pawn_moves(result, i, j); break;
            case Piece::NONE: break;
        };
    }

    friend std::ostream& operator<<(std::ostream& xx, const Chess& chess);
    bool reverse_move() {
        if (moves.size() == 0) return false;
        Move& last_move = moves[moves.size()-1];
        last_move.reverse();
        if (_apply_move(last_move)) {
            moves.pop_back();
            return true;
        }
        return false;
    }
    bool make_move(const Move move) {
        if (_apply_move(move)) {
            moves.push_back(move);
            return true;
        }
        return false;
    }
    bool _apply_move(const Move& move) {
        int i, j, k, l;
        std::tie(j, i) = move.from;
        std::tie(l, k) = move.to;
        if (board[i][j] != move.piece && board[i][j] != move.promotes) {
            // Wrong move. Piece is not on the described location
            std::cerr << move << " is incorrect start (" << board[i][j] << ")\n";
            return false;
        }
        if (board[k][l].type != Piece::NONE && board[i][j].is_white == board[k][l].is_white) {
            // Wrong move. Destination is already taken by a friendly piece
            std::cerr << move << " is incorrect end (" << board[k][l] << ")\n";
            return false;
        }
        board[k][l] = move.piece;
        int move_score = -(2 * move.takes.is_white - 1) * move.takes.worth;
        if (move.promotes.type != Piece::NONE && move.piece.type == Piece::P) {
            if (!move.is_reverse) {
                board[k][l] = move.promotes;
            }
            move_score += (2 * move.promotes.is_white - 1) * (move.promotes.worth - 1);
        }
        if (move.takes.type == Piece::K) {
            state = move.piece.is_white?WHITE_WIN:BLACK_WIN;
        }
        if (move.is_reverse) {
            board[i][j] = move.takes;
            state = PLAY;
            score -= move_score;
        } else {
            score += move_score;
            board[i][j] = Piece::NONE;
        }
        return true;
    }
    std::string get_state_hash() {
        std::ostringstream tmp;
        tmp << moves.size() % 2;
        for (auto& line : board) {
            for (Piece& piece : line) tmp << piece;
        }
    return tmp.str();
    }
};

std::ostream& operator<<(std::ostream& xx, const Chess& chess) {
    int i = 8;
    for (auto line = chess.board.rbegin(); line != chess.board.rend(); line++) {
        std::cout << i << '|';
        i--;
        for (auto field : *line) {
            xx << field << ' ';
        }
        xx << std::endl;
    }
    std::cout << "-+---------------\n";
    std::cout << " |";
    for (int i = 0; i < 8; i++) {
        std::cout << static_cast<char>('a'+i) << ' ';
    }
    std::cout << std::endl;
    return xx;
}

#endif
