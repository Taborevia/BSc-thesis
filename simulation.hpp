#ifndef SIMULATION_H
#define SIMULATION_H
#include "model.hpp"
#include "player.hpp"
#include <vector>
#include <chrono>
#include <iostream>

std::vector<int> simulation(std::vector<int> genotype0, std::vector<int> genotype1, int depth0, int depth1, int numberOfIndividual0, int numberOfIndividual1){
    // auto start = std::chrono::high_resolution_clock::now();
    Model board2;
    Model &board = board2;
    Player player0(board,0, genotype0);
    Player player1(board,1, genotype1);
    bool game = true;
    bool player = 0;
    int moves = 0;
    while(game){
        if (moves >1000){
            return std::vector<int>{3,numberOfIndividual0,numberOfIndividual1,moves};
        }
        moves++;
        // board.print_board();
        // std::cout<<std::endl;
        if (!player){
            player0.make_minmax_move(depth0);
        }else{
            player1.make_minmax_move(depth1);
        }
        player = !player;
        int game_status = board.game_over_check(board.get_board());
        auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = end - start;
        // if (duration.count()>60){
        //     board.print_board();
        //     std::cout<<std::endl<<game_status<<std::endl;
        // }
        if (game_status==0){
            continue;
        }else{
            return std::vector<int>{game_status,numberOfIndividual0,numberOfIndividual1,moves};
        }
        
    }
}

#endif