// autor: Paweł Grzegorzewski
// modul odpowiedzialny za logike gracza, implementacje botow: min-max i mcts
#include "simulation.hpp"
#include "player.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <utility>
#include <chrono>
#include <math.h>
#include <random>
#include <memory>
#include <thread>
#include <future>


Player::Player(Model &board, bool numberOfPlayer, std::vector<int> weightEvaluateFunction)
    : model(board), player(numberOfPlayer), weightEvaluateFunction(weightEvaluateFunction) {
}
Player::Player(Model &board, bool numberOfPlayer)
    : model(board), player(numberOfPlayer) {
}
void Player::make_move(int move){
    model.make_move(move, player, model.get_board());
}
void Player::make_minmax_move(int depth){
    model.make_move(minmax(model.get_board(),depth,player, INT_MIN, INT_MAX).second,player, model.get_board());
}
void Player::make_mcts_move(float resources, float c_parameter, bool multi_threading){
    model.make_move(monte_carlo_tree_search(model.get_board(),resources,player,c_parameter,multi_threading),player,model.get_board());
}
float Player::evaluate(std::array<std::array<int,11>,2> board){

    // C*
    switch (model.game_over_check(board)){
        case 1:
            return -10000;
        case 2:
            return 10000;
        case 3:
            return 0;
        default:
            break;
    }
    std::vector<float> C_values(26,0);
    // C0,C1
    C_values.at(0) = float(board.at(1).at(9));
    C_values.at(1) = float(-board.at(0).at(9));
    // C2-C9,C10-C17, C18,C19

    for (int player = 0; player<2; player++){
        if (board.at(player).at(10)){
            for (int i = 0;i<9;i++){
                if (board.at(!player).at(i) == -3){
                    if (player){
                        C_values.at(10-i)++;  //C2-C9 czytajac od prawej
                    }else {
                        C_values.at(10+i)--; //C10-C17 czytajac od lewej
                    }
                    bool side = !player;
                    int move = i;
                    for(int j = 18;j>2;j--){ // sprawdzenie 16 dołków, 17 to osobny przypadek(potrzebuje 1, a nie 2 kulek), 18 to tuz
                        if (side == 0){
                            move--;
                        }else{move++;}
                        if (move > 8){
                            move = 8;
                            side = !side;
                        }else if (move <0){
                            move = 0;
                            side = !side;
                        }
                        if(board.at(side).at(move) >= j){
                            if(player){
                                C_values.at(18)++;   //C18
                            }else{
                                C_values.at(19)--;   //C19
                            }
                        }
                    }
                    //przypadek ostatniego dolka
                    if (side == 0){
                        move--;
                    }else{move++;}
                    if (move > 8){
                        move = 8;
                        side = !side;
                    }else if (move <0){
                        move = 0;
                        side = !side;
                    }
                    if(board.at(side).at(move) >= 1){
                        if(player){
                                C_values.at(18)++;  //C18
                            }else{
                                C_values.at(19)--;  //C19
                            }
                    } 
                    break;
                }
            }
        }
    }
    // C3
    for (int i = 0;i<9;i++){
        if (board.at(0).at(i) > 0){
            C_values.at(21)--; //C21 - liczba mozliwych ruchow dla mina
            C_values.at(23)-=board.at(0).at(i); //C23 - liczba kulek po stronie mina
            if (board.at(0).at(i)%2 == 1){
                C_values.at(24) += board.at(0).at(i); //C24 - liczba nieparzystych kulek po stronie przeciwnika
            }
        }
        if (board.at(1).at(i) > 0){
            C_values.at(20)++; //C20 - liczba mozliwych ruchow dla maksa
            C_values.at(22)+=board.at(1).at(i); //C22 - liczba kulek po stronie maksa
            if (board.at(1).at(i)%2 == 1){
                C_values.at(25) -= board.at(1).at(i); //C25 - C24 dla gracza min
            }
        }
    }
    //normalizacja wartosci C
    C_values.at(0) /= 81;
    C_values.at(1) /= 81;
    C_values.at(18) /= 17;
    C_values.at(19) /= 17;
    C_values.at(20) /= 9;
    C_values.at(21) /= 9;
    C_values.at(22) /= 162;
    C_values.at(23) /= 162;
    C_values.at(24) /= 162;
    C_values.at(25) /= 162;
    float value = 0;
    for (int i=0; i<C_values.size();i++){
        value += C_values.at(i) * this->weightEvaluateFunction.at(i);
    }
    return value;
}


std::pair<float,int> Player::minmax(std::array<std::array<int,11>,2> board, int depth, int isMaximizingPlayer, float alpha, float beta) {
    int bestMove = -1;
    if (depth == 0 || !model.is_any_legal_move(isMaximizingPlayer,board)) {
      return std::make_pair(evaluate(board),0);
    }
    if (isMaximizingPlayer) {
        float m = alpha;
        for (int move = 0; move<9; move ++){
            if (board.at(isMaximizingPlayer).at(move)== 0 || board.at(isMaximizingPlayer).at(move)==-3){
                continue;
            }
            std::array<std::array<int,11>,2> tempBoard = std::array<std::array<int,11>,2>(board);
            model.make_move(move, isMaximizingPlayer, tempBoard);
            float eval = minmax(tempBoard, depth - 1, !isMaximizingPlayer, m, beta).first;
            if(eval > m){
                m = eval;
                bestMove = move;
            }
            if(m>=beta){
                break; 
            }
        }
        return std::make_pair(m,bestMove);
    } else {
        float m = beta;
        for (int move = 0; move<9; move ++){
            if (board.at(isMaximizingPlayer).at(move)== 0 || board.at(isMaximizingPlayer).at(move)==-3){
                continue;
            }
            std::array<std::array<int,11>,2> tempBoard = std::array<std::array<int,11>,2>(board);
            model.make_move(move, isMaximizingPlayer, tempBoard);
            float eval = minmax(tempBoard, depth - 1, !isMaximizingPlayer, alpha, m).first;
            if(eval < m){
                m = eval;
                bestMove = move;
            }
            if(m <= alpha){
                break; 
            }
        }
        
        return std::make_pair(m,bestMove);

    }
}
int Player::monte_carlo_tree_search(std::array<std::array<int,11>,2> board, float resources, int is_maximizing_player, float c_parameter, bool multi_threading){
    unsigned int threads = std::thread::hardware_concurrency();
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    auto root = std::make_shared<node>(is_maximizing_player,board,std::weak_ptr<node>(),-1,model.get_legal_moves(is_maximizing_player,board),model.game_over_check(board),true);
    while(duration.count()<=resources){
        std::shared_ptr<node> v = tree_policy(root,c_parameter);
        if (multi_threading){
            std::pair<float,float> reward = default_policy_multi_threading(v,threads);
            back_propagation(v,reward.first,reward.second);
        }else{
            float reward = default_policy(v);
            back_propagation(v,reward,1-reward);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
    }
    return best_move(root);
}
int Player::best_move(std::shared_ptr<node> root){
    int best_move = -1;
    int best_value = -1;
    for (auto child : root->childrens){
        if(child->simulations>best_value){
            best_value=child->simulations;
            best_move=child->action;
        }    
    }
    return best_move;
}
std::shared_ptr<node> Player::tree_policy(std::shared_ptr<node> state, float c){
    if(state->game_status!=0){
        return state;
    }
    if (!fully_expanded(state)){
        return expand(state);
    }else{
        return tree_policy(best_child(state,c),c);
    }
}
bool Player::fully_expanded(std::shared_ptr<node> state){
    std::vector<int> moves = model.get_legal_moves(state->player,state->board);
    if (state->possible_actions.size()>0){
        return false;
    }else{
        return true;
    }
}
std::shared_ptr<node> Player::best_child(std::shared_ptr<node> state, float c){
    int best_child=-1;
    float best_value = -1;
    for (int i = 0;i<state->childrens.size();i++){
        float value = (state->childrens.at(i)->reward/state->childrens.at(i)->simulations)+(c*sqrt(log(state->simulations)/state->childrens.at(i)->simulations));
        if (value > best_value){
            best_child = i;
            best_value = value;
        }
    }
    return state->childrens.at(best_child);
}
std::shared_ptr<node> Player::expand(std::shared_ptr<node> state){
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
    std::uniform_int_distribution<int> genRand(0, state->possible_actions.size()-1);
    std::array<std::array<int,11>,2>temp_board = state->board;
    int move = genRand(rng);
    model.make_move(state->possible_actions.at(move),state->player,temp_board);
    auto new_state = std::make_shared<node>(!state->player,temp_board,state,state->possible_actions.at(move),model.get_legal_moves(!state->player,temp_board),model.game_over_check(temp_board),false);

    state->possible_actions.erase(state->possible_actions.begin()+move);
    state->childrens.push_back(new_state);

    return new_state;
}
float Player::default_policy(std::shared_ptr<node> state){
    int result = 0;
    if (state->game_status==0){
        result = random_simulation(state->board,state->player);
    }else{
        result = state->game_status;
    }
    if (result==1){
        if (state->player){
            return 1;
        }else{
            return 0;
        }
    }else if(result==2){
        if(state->player){
            return 0;
        }else{
            return 1;
        }
    }else if(result==3){
        return 0.5;
    }
    return 0;
}
std::pair<float,float> Player::default_policy_multi_threading(std::shared_ptr<node> state, unsigned int threads){
    float result = 0;
    float result2 = 0;
    if (state->game_status==0){
        if (threads == 0){
            result = random_simulation(state->board,state->player);
        }else{
            std::vector<std::future<int>> results;
            for(int i = 0;i<threads;i++){
                results.push_back(std::async(std::launch::async,random_simulation,state->board,state->player));
            }
            for (auto& score : results){
                switch (score.get()){
                    case 1:
                        if (state->player){
                            result += 1;
                        }else{
                            result2 += 1;
                        }
                        break;
                    case 2:
                        if(!state->player){
                            result += 1;
                        }else{
                            result2 += 1;
                        }
                        break;
                    case 3:
                        result += 0.5;
                        result2 += 0.5;
                        break;
                }
            }
            return std::make_pair(result/threads,result2/threads);
        }
    }else{
        result = state->game_status;
    }
    if (result==1){
        if (state->player){
            return std::make_pair(1,0);
        }else{
            return std::make_pair(0,1);
        }
    }else if(result==2){
        if(state->player){
            return std::make_pair(0,1);
        }else{
            return std::make_pair(1,0);
        }
    }else if(result==3){
        return std::make_pair(0.5,0.5);
    }
    return std::make_pair(0,0);
}
void Player::back_propagation(std::shared_ptr<node> state, float reward1, float reward2){
    state->reward+=reward1;
    state->simulations+=1;

    if(!state->root){
        auto temp = state->parent.lock();
        back_propagation(temp,reward2,reward1);
    }
}
