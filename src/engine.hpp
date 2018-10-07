#include <iostream>
#include <random>
#include <chrono>
#include <tuple>
#include <map>

template<class Game>
class Engine {
  public:
    typedef typename Game::MoveType MoveType;
    // Memo: hash -> depth, score, move
    typedef std::map<std::string, std::pair<int, std::pair<std::vector<MoveType>, int>>> MemoType;
    Game game;
    std::vector<long long> memo_hit;
    std::vector<long long> memo_miss;
    Engine(const Game& _game):game(_game), memo_hit(10, 0), memo_miss(10, 0){}

    std::pair<std::vector<MoveType>, int> calculate_move(
            int depth=1, bool is_alpha=true, MemoType* memo=nullptr) {
        if (memo == nullptr) memo = new MemoType();
        if (depth == 0) {
            return std::make_pair(std::vector<MoveType>(), game.score);
        }
        std::string game_hash = game.get_state_hash();
        if (memo->count(game_hash) > 0 && memo->at(game_hash).first >= depth){
            memo_hit[depth]++;
            return memo->at(game_hash).second;
        }
        memo_miss[depth]++;

        int best_score = -2000 * (2 * is_alpha - 1);
        std::vector<std::vector<MoveType>> best_moves;
        std::vector<MoveType> all_moves = game.get_all_moves();
        for (MoveType& move : all_moves) {
            if (!game.make_move(move)) {
                if (depth == 4) {
                    std::cout << "WTF" << move << "\n";
                }
                continue;
            }
            auto move_score = calculate_move(depth-1, !is_alpha, memo);
            int score = move_score.second;
            std::vector<MoveType> this_move = move_score.first;
            this_move.insert(this_move.begin(), move);

            game.reverse_move();
            if ((is_alpha && score > best_score)
                || (!is_alpha && score < best_score)) {
                best_score = score;
                best_moves = {this_move};
            } else if (score == best_score) {
                best_moves.push_back(this_move);
            }
        }
        if (best_moves.size() == 0) {
            memo->operator[](game_hash) = {depth, std::make_pair(std::vector<MoveType>(), game.score)};
            return memo->at(game_hash).second;
        }
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed1);
        std::uniform_int_distribution<int> distribution(0, best_moves.size()-1);
        memo->operator[](game_hash) = {depth, std::make_pair(best_moves[distribution(generator)], best_score)};
        return memo->at(game_hash).second;
    }

};
