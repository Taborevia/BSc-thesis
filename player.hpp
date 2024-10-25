#ifndef PLAYER_H
#define PLAYER_H
#include "model.hpp"

class Player{
private:
    bool player;
    Model& model;
    std::vector<int> weightEvaluateFunction;
public:
    Player(Model&, bool, std::vector<int>);
    void make_move(int);
    std::pair<float,int> minmax(std::array<std::array<int,11>,2> board, int depth, int isMaximizingPlayer, float alpha, float beta);
    float evaluate(std::array<std::array<int,11>,2> board);
    void make_minmax_move(int depth);
    int findBestMove(std::array<std::array<int,11>,2> board, int depth, int player);
};

#endif