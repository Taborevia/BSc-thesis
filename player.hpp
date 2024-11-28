#ifndef PLAYER_H
#define PLAYER_H
#include "model.hpp"
#include "node.hpp"

class Player{
private:
    bool player;
    Model& model;
    std::vector<int> weightEvaluateFunction;
public:
    Player(Model&, bool, std::vector<int>);
    Player(Model&,bool);
    void make_move(int);
    std::pair<float,int> minmax(std::array<std::array<int,11>,2> board, int depth, int isMaximizingPlayer, float alpha, float beta);
    float evaluate(std::array<std::array<int,11>,2> board);
    void make_minmax_move(int depth);
    void make_mcts_move(float resources);
    int findBestMove(std::array<std::array<int,11>,2> board, int depth, int player);
    int monte_carlo_tree_search(std::array<std::array<int,11>,2> board, float resources, int is_maximizing_player);
    std::shared_ptr<node> tree_policy(std::shared_ptr<node> state);
    void back_propagation(std::shared_ptr<node> state, float reward);
    float default_policy(std::shared_ptr<node> state);
    std::shared_ptr<node> expand(std::shared_ptr<node> state);
    bool fully_expanded(std::shared_ptr<node> state);
    std::shared_ptr<node> best_child(std::shared_ptr<node> state, float c);
    int best_move(std::shared_ptr<node> root);
};

#endif