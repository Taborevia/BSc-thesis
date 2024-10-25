#include "model.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <fstream>
#include <windows.h>

std::ofstream outfile("output.txt");

std::array<std::array<int,11>,2>& Model::get_board()
{
    return board;
}
Model::Model(){}
int Model::get_points(bool player){
    return board.at(player).at(9);
}
bool Model::is_legal_move(int move, bool player, std::array<std::array<int,11>,2>& temp_board){
    if (temp_board.at(player).at(move) == 0 || temp_board.at(player).at(move) == -3 ){
        return false;
    }else{
        return true;
    }
}
// 0 - game in progress, 1 - player 0 won, 2 - player 1 won, 3 - draw
int Model::game_over_check(std::array<std::array<int,11>,2>& temp_board){
    // for(int i = 0; i<2; i++){
    //     for(int j = 0; j<11; j++){
    //         outfile<<std::setfill('0')<<std::setw(2)<<board.at(i).at(j)<<" ";
    //     }
    //     outfile<<std::endl;
    // }
    if (temp_board.at(0).at(9) > 81){
        return 1;
    }
    if (temp_board.at(1).at(9) > 81){
        return 2;
    }
    if (temp_board.at(0).at(9) == 81 and temp_board.at(1).at(9) == 81){
        return 3;
    }
    // if ((temp_board.at(0).at(9) + temp_board.at(0).at(9) > 100) && get_legal_moves(1, temp_board).empty()){
    // if (get_legal_moves(1, temp_board).empty()){
    if (!is_any_legal_move(1, temp_board)){
        for (int i = 0; i < 9; i++){
            // std::cout<<temp_board.at(1).at(i);
            if(temp_board.at(0).at(i)>=0){
                temp_board.at(0).at(9) += temp_board.at(0).at(i);
                temp_board.at(0).at(i) = 0;
            }
            // std::cout<<std::endl;
        }
        return game_over_check(temp_board);
    }
    // if ((temp_board.at(0).at(9) + temp_board.at(0).at(9) > 100) && get_legal_moves(0, temp_board).empty()){
    // if (get_legal_moves(0, temp_board).empty()){
    if (!is_any_legal_move(0, temp_board)){
        for (int i = 0; i < 9; i++){
            // std::cout<<temp_board.at(0).at(i);
            if(temp_board.at(1).at(i)==-3){
                continue;
            }
            temp_board.at(1).at(9) += temp_board.at(1).at(i);
            temp_board.at(1).at(i) = 0;
        }
        // std::cout<<std::endl;
        return game_over_check(temp_board);
    }
    return 0;
}

bool Model::is_any_legal_move(bool player, std::array<std::array<int,11>,2>& temp_board){
    for(int i = 0;i<9;i++){
        if (temp_board.at(player).at(i)>0){
            return true;
        }
    }
    return false;
}

std::vector<int> Model::get_legal_moves(bool player, std::array<std::array<int,11>,2>& temp_board){
    std::vector<int> legal_moves;
    for (int i = 0;i<9;i++){
        if (is_legal_move(i,player, temp_board)){
            legal_moves.push_back(i);
        }
    }
    return legal_moves;
}
bool Model::make_move(int move, bool player, std::array<std::array<int,11>,2>& board){
    // std::cout<<move<<" "<<player<<std::endl<<std::endl;
    if (board == std::array<std::array<int,11>,2>{}) {  
        board = this->board;  
    }
    if (board.at(player).at(move) == 0 or move > 8){
        return 0;
    }else{
        int balls = board.at(player).at(move);
        bool side = player;
        if (board.at(player).at(move) == 1){
            board.at(side).at(move) = 0;
        }else{
            board.at(side).at(move) = 1;
            balls--;
        }
        for(balls; balls>0; balls--){
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
            if(board.at(side).at(move) == -3){
                if (side){
                    board.at(0).at(9)++;
                }else{
                    board.at(1).at(9)++;
                }
            }else{
                board.at(side).at(move)++;
            }
        }
        if (side == !player){
            if (board.at(side).at(move)%2 == 0){
                if (player){
                    board.at(1).at(9) += board.at(side).at(move);
                    board.at(side).at(move) = 0;
                }else{
                    board.at(0).at(9) += board.at(side).at(move);
                    board.at(side).at(move) = 0;
                }
            } else if((board.at(side).at(move) == 3)){
                if (player){ 
                    if (move != 0 && !board.at(player).at(10) && board.at(player).at(8-move)!=-3){
                        board.at(1).at(9) += board.at(side).at(move);
                        board.at(side).at(move) = -3;
                        board.at(player).at(10) = true;
                    }
                }else if(move != 8 && !board.at(player).at(10) && board.at(player).at(8-move)!=-3){
                    board.at(0).at(9) += board.at(side).at(move);
                    board.at(side).at(move) = -3;
                    board.at(player).at(10) = true;
                }
            }
        }
    }
    return true;
}
void Model::print_board(){
    for(int i = 0; i<2; i++){
        for(int j = 0; j<11; j++){
            std::cout<<std::setfill('0')<<std::setw(2)<<board.at(i).at(j)<<" ";
        }
        std::cout<<std::endl;
    }
}