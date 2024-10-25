#include "player.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <utility>

// std::ofstream testy("wyniki.txt");

Player::Player(Model &board, bool numberOfPlayer, std::vector<int> weightEvaluateFunction)
    : model(board), player(numberOfPlayer), weightEvaluateFunction(weightEvaluateFunction) {
}
// DODAJ FUNKCJE FIND BEST MOVE
void Player::make_move(int move){
    model.make_move(move, player, model.get_board());
}
void Player::make_minmax_move(int depth){
    model.make_move(minmax(model.get_board(),depth,player, INT_MIN, INT_MAX).second,player, model.get_board());
}
float Player::evaluate(std::array<std::array<int,11>,2> board){
    // for(int i = 0; i<2; i++){
    //     for(int j = 0; j<11; j++){
    //         testy<<std::setfill('0')<<std::setw(2)<<board.at(i).at(j)<<" ";
    //     }
    //     testy<<std::endl;
    // }

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
    // C1,C2
    C_values.at(0) = float(board.at(1).at(9));
    C_values.at(1) = float(-board.at(0).at(9));
    // C2-C9,C10-C17, C18,C19

    for (int player = 0; player<2; player++){
        if (board.at(player).at(10)){
            for (int i = 0;i<9;i++){
                if (board.at(!player).at(i) == -3){
                    if (player){
                        C_values.at(2+i)++;  //C2-C9 czytajac od lewej
                    }else {
                        C_values.at(9+i)--; //C10-C17 czytajac od lewej
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
                    //last hole case
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
        // for (int& move : model.get_legal_moves(isMaximizingPlayer, board)) {
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
        // for (int& move : model.get_legal_moves(isMaximizingPlayer, board)) {
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
// int Player::findBestMove(std::array<std::array<int,11>,2> board, int depth, int isMaximizingPlayer) {
//     float bestEval = isMaximizingPlayer ? INT32_MIN : INT32_MAX;
//     int bestMove = -1;
//     for (int& move : model.get_legal_moves(isMaximizingPlayer,board)) {
//         std::array<std::array<int,11>,2> tempBoard = std::array<std::array<int,11>,2>(board);
//         model.make_move(move,isMaximizingPlayer,tempBoard);
//         float eval = minimax(tempBoard, depth, isMaximizingPlayer, -100000, 100000);
//         // std::cout<<move<<" , "<<eval<<std::endl;
//         if(isMaximizingPlayer){ //player1
//             if (eval > bestEval) {
//                 bestEval = eval;
//                 bestMove = move;
//                 // std::cout<<bestEval<<" "<<move;
//             }
//         }else{// player0
//             if (eval < bestEval) {
//                 bestEval = eval;
//                 bestMove = move;
//                 // std::cout<<bestEval<<" "<<move;
//             }
//         }
//     }
//     // std::cout<<"best: "<<bestMove<<std::endl;
//     return bestMove;
// }