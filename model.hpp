#ifndef MODEL_H
#define MODEL_H
#include <array>
#include <vector>

class Model{
private:
    std::array<std::array<int,11>,2> board  = {{{9,9,9,9,9,9,9,9,9,0,0},{9,9,9,9,9,9,9,9,9,0,0}}}; // in 10th are points, in 11th is tuz presents
public:
    Model();
    std::array<std::array<int,11>,2>& get_board();
    bool make_move(int move, bool player, std::array<std::array<int,11>,2>& board);
    void print_board();
    int get_points(bool player);
    bool is_legal_move(int move, bool player, std::array<std::array<int,11>,2>& board);
    bool is_any_legal_move(bool player, std::array<std::array<int,11>,2>& temp_board);
    std::vector<int> get_legal_moves(bool player, std::array<std::array<int,11>,2>& board);
    int game_over_check(std::array<std::array<int,11>,2>& board);
    void set_board(std::array<std::array<int,11>,2> board);
};

#endif