#include <iostream>
#include <random>
#include <string.h>
#include "chess.hpp"
#include "engine.hpp"

template<class value_t>
std::ostream& operator<<(std::ostream& xx, const std::vector<value_t>& vec) {
    xx << '[';
    for (const value_t& v : vec) xx << v << ", ";
    xx << ']';
    return xx;
}
int parse_number(char* argv) {
    int result = 0;
    for (int i = 0; i < strlen(argv); i++) {
        if (argv[i] < '0' or argv[i] > '9') {
            return 3;
        }
        result = result * 10 + (argv[i] - '0');
    }
    return result;
}
std::pair<int, int> parse_cmd_args(int argc, char* argv[]) {
    int white_depth = 0, black_depth = 0;  // Human players
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            std::cout << "This is an elementary chess engine.\n"
                << "Default playstyle is human vs human.\n\n"
                << "--help \t\t\t Print this help\n"
                << "--white, -w [depth=3] \t Computer plays white\n"
                << "--black, -b [depth=3] \t Computer plays black\n\n"
                << "Example: " << argv[0] << " --black --white 2\n";
            return std::make_pair(-1, -1);
        } else if (strcmp(argv[i], "--white") == 0) {
            white_depth = 3;
            if (argc > i+1) white_depth = parse_number(argv[i+1]);
        } else if (strcmp(argv[i], "--black") == 0) {
            black_depth = 3;
            if (argc > i+1) black_depth = parse_number(argv[i+1]);
        }
    }
    return std::make_pair(white_depth, black_depth);
}

int main(int argc, char* argv[]) {
    int white_depth, black_depth;
    std::tie(white_depth, black_depth) = parse_cmd_args(argc, argv);
    if (white_depth == -1 && black_depth == -1) return 0;
    Chess chess;
    while (chess.state == Chess::PLAY) {
        bool is_whites_turn = 1 - (chess.moves.size() % 2);
        bool human_turn = (is_whites_turn && white_depth == 0) || (!is_whites_turn && black_depth == 0);
        std::cout << chess;
        if (human_turn) {
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
        } else {
            Engine<Chess> engine(chess);
            auto best_move = engine.calculate_move(
                is_whites_turn?white_depth:black_depth,
                chess.moves.size()%2 == 0
            ).first;
            std::cout << "Moving best move: " << best_move[0] << std::endl;
            chess.make_move(best_move[0]);
        }
    }
    switch(chess.state) {
        case Chess::WHITE_WIN:
            std::cout << "White won!\n";
            break;
        case Chess::BLACK_WIN:
            std::cout << "Black won!\n";
            break;
        case Chess::DRAW:
            std::cout << "It's a draw!\n";
            break;
        default:
            std::cout << "What happened? The score is " << chess.state << "?\n";
    }
}
