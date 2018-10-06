#ifndef __MOVE_GEN_HPP
#define __MOVE_GEN_HPP

#include "piece.hpp"
#include "move.hpp"
// #include "chess.hpp"

template <class Chess>
class MoveGenerator {
  public:
    Chess* chess;
    MoveGenerator(Chess* _chess):chess(_chess) {}
    void get_king_moves(std::vector<Move>& result, int i, int j) {
        if (chess->board[i][j].type != Piece::K) return;
        for (int k = std::max(i-1, 0); k <= std::min(i+1, 7); k++) {
            for (int l = std::max(j-1, 0); l <= std::min(j+1, 7); l++) {
                if (k == i && l == j) continue;
                if (chess->board[k][l].type == Piece::NONE || chess->board[k][l].is_white != chess->board[i][j].is_white) {
                    result.emplace_back(
                        chess->board[i][j],
                        std::make_pair(j, i),
                        std::make_pair(l, k),
                        chess->board[k][l]
                    );
                }
            }
        }
    }

    void get_queen_moves(std::vector<Move>& result, int i, int j) {
        if (chess->board[i][j].type != Piece::Q) return;
        for (int ik = -1; ik <= 1; ik += 1) for (int jk = -1; jk <= 1; jk += 1)
        for (int k = 1; k*ik + i <= 7 && k*jk + j <= 7 && k*jk + j >= 0 && k*ik + i >= 0; k++) {
            if (chess->board[i+k*ik][j+k*jk].type != Piece::NONE) {
                if (chess->board[i+k*ik][j+k*jk].is_white != chess->board[i][j].is_white) {
                    result.emplace_back(
                        chess->board[i][j],
                        std::make_pair(j, i),
                        std::make_pair(j+k*jk, i+k*ik),
                        chess->board[i+k*ik][j+k*jk]
                    );

                }
                break;
            }
            result.emplace_back(
                chess->board[i][j],
                std::make_pair(j, i),
                std::make_pair(j+k*jk, i+k*ik),
                chess->board[i+k*ik][j+k*jk]
            );
        }
    }
    void get_bishop_moves(std::vector<Move>& result, int i, int j) {
        if (chess->board[i][j].type != Piece::B) return;
        for (int ik = -1; ik <= 1; ik += 2) for (int jk = -1; jk <= 1; jk += 2) if (ik != 0 || jk != 0)
        for (int k = 1; k*ik + i <= 7 && k*jk + j <= 7 && k*jk + j >= 0 && k*ik + i >= 0; k++) {
            if (chess->board[i+k*ik][j+k*jk].type != Piece::NONE) {
                if (chess->board[i+k*ik][j+k*jk].is_white != chess->board[i][j].is_white) {
                    result.emplace_back(
                        chess->board[i][j],
                        std::make_pair(j, i),
                        std::make_pair(j+k*jk, i+k*ik),
                        chess->board[i+k*ik][j+k*jk]
                    );
                }
                break;
            }
            result.emplace_back(
                chess->board[i][j],
                std::make_pair(j, i),
                std::make_pair(j+k*jk, i+k*ik),
                chess->board[i+k*ik][j+k*jk]
            );
        }
    }
    void get_rook_moves(std::vector<Move>& result, int i, int j) {
        if (chess->board[i][j].type != Piece::R) return;
        for (int ik = -1; ik <= 1; ik += 1) for (int jk = std::abs(ik)-1; jk <= -std::abs(ik)+1; jk += 2)
        for (int k = 1; k*ik + i <= 7 && k*jk + j <= 7 && k*jk + j >= 0 && k*ik + i >= 0; k++) {
            if (chess->board[i+k*ik][j+k*jk].type != Piece::NONE) {
                if (chess->board[i+k*ik][j+k*jk].is_white != chess->board[i][j].is_white) {
                    result.emplace_back(
                        chess->board[i][j],
                        std::make_pair(j, i),
                        std::make_pair(j+k*jk, i+k*ik),
                        chess->board[i+k*ik][j+k*jk]
                    );

                }
                break;
            }
            result.emplace_back(
                chess->board[i][j],
                std::make_pair(j, i),
                std::make_pair(j+k*jk, i+k*ik),
                chess->board[i+k*ik][j+k*jk]
            );
        }
    }

    void get_knight_moves(std::vector<Move>& result, int i, int j) {
        if (chess->board[i][j].type != Piece::N) return;
        std::vector<std::pair<int, int>> candidates = {
            {2, 1}, {2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}};
        for (std::pair<int, int>& kl : candidates) {
            int k, l;
            std::tie(k, l) = kl;
            if (i+k < 0 || i + k >= 8 || j+l < 0 || j+l >= 8) continue;
            if (chess->board[i+k][j+l].type == Piece::NONE || chess->board[i+k][j+l].is_white != chess->board[i][j].is_white) {
                result.emplace_back(
                    chess->board[i][j],
                    std::make_pair(j, i),
                    std::make_pair(j+l, i+k),
                    chess->board[i+k][j+l]
                );
            }
        }
    }

    void get_pawn_moves(std::vector<Move>& result, int i, int j) {
        if (chess->board[i][j].type != Piece::P) return;
        int ik = chess->board[i][j].is_white * 2 - 1;
        bool start = (chess->board[i][j].is_white && i == 1) || (!chess->board[i][j].is_white && i == 6);
        bool end = (chess->board[i][j].is_white && i == 6) || (!chess->board[i][j].is_white && i == 1);
        std::vector<Piece::PieceType> promotion_types = {Piece::NONE};
        if (end) promotion_types = {Piece::Q, Piece::R, Piece::B, Piece::N};
        for (int k = 1; k <= 1+start; k++) {
            if (i+k*ik < 0 || i+k*ik >=8 || chess->board[i+k*ik][j].type != Piece::NONE) {
                break;
            } else {
                for (Piece::PieceType promotion_type : promotion_types) {
                    result.emplace_back(
                        chess->board[i][j],
                        std::make_pair(j, i),
                        std::make_pair(j, i+k*ik),
                        chess->board[i+k*ik][j],
                        Piece(promotion_type, chess->board[i][j].is_white)
                    );
                }
            }
        }
        for (int l = -1; l <= 1; l += 2) {
            if (i+ik < 0 || i+ik >=8 || j+l < 0 || j+l >= 7) continue;
            if (chess->board[i+ik][j+l].type != Piece::NONE && chess->board[i][j].is_white != chess->board[i+ik][j+l].is_white) {
                for (Piece::PieceType promotion_type : promotion_types) {
                    result.emplace_back(
                        chess->board[i][j],
                        std::make_pair(j, i),
                        std::make_pair(j+l, i+ik),
                        chess->board[i+ik][j+l],
                        Piece(promotion_type, chess->board[i][j].is_white)
                    );
                }
            }
        }
    }
};

#endif
