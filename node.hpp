#ifndef NODE_H
#define NODE_H
#include <vector>
#include <array>
#include <memory>

struct node
{
    std::vector<std::shared_ptr<node>> childrens;
    std::vector<int> possible_actions; //nie zbadane wczesniej 
    std::shared_ptr<node> parent;
    std::array<std::array<int,11>,2> board;
    bool player;
    int game_status = 0;
    int action;
    float reward = 0;
    int simulations = 0;
    node(bool maximazing_player, std::array<std::array<int,11>,2> state, std::shared_ptr<node> parent, int action_from_parent, std::vector<int> possible_actions, int game_status) : player(maximazing_player), board(state), parent(parent),action(action_from_parent), possible_actions(possible_actions), game_status(game_status) {}
};

#endif