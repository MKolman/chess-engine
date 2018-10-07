#include <iostream>
#include <random>
#include "chess.hpp"
#include "engine.hpp"

template<class value_t>
std::ostream& operator<<(std::ostream& xx, const std::vector<value_t>& vec) {
    xx << '[';
    for (const value_t& v : vec) xx << v << ", ";
    xx << ']';
    return xx;
}

int main() {
    Chess chess;
    bool is_whites_turn = true, is_bot_white = false;
    while (chess.state == Chess::PLAY) {
        if (is_whites_turn == is_bot_white) {
            Engine<Chess> engine(chess);
            auto best_move = engine.calculate_move(5, chess.moves.size()%2 == 0).first;
            std::cout << "Moving best move: " << best_move[0] << std::endl;
            chess.make_move(best_move[0]);
        } else {
            std::cout << chess;
            std::vector<Move> local_moves;
            char column;
            int row = -1;
            while (row == -1) {
                std::cout << "Which piece do you want to move?\n";
                std::cin >> column >> row;
                chess.get_single_moves(local_moves, row-1, column - 'a');
                std::cout << "Where to move?\n  -2: Undo\n  -1: Retry\n";
                for (int i = 0; i < local_moves.size(); i++) {
                    std::cout << "  " << i << ": " << local_moves[i] << '\n';
                }
                std::cout << "Write number: ";
                std::cin >> row;
                if (row == -2) {
                    chess.reverse_move();
                    chess.reverse_move();
                    std::cout << chess;
                }
                if (row < 0 || row >= local_moves.size()) row = -1;
            }
            chess.make_move(local_moves[row]);
        }
        is_whites_turn = !is_whites_turn;
    }
}
