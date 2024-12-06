#include "simulation.hpp"
#include <iostream>
std::vector<int> simulation(std::vector<int> genotype0, std::vector<int> genotype1, int depth0, int depth1, int numberOfIndividual0, int numberOfIndividual1){
    // auto start = std::chrono::high_resolution_clock::now();
    Model board;
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
    return std::vector<int>{0,0,0};
}

int random_simulation(std::array<std::array<int,11>,2> board, bool is_maximazing_player){
    // auto start = std::chrono::high_resolution_clock::now();
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
    Model model;
    model.set_board(board);
    Player player0(model,0);
    Player player1(model,1);
    bool player = is_maximazing_player;
    int moves = 0;
    while(true){
        if (moves >1000){
            return 3;
        }
        moves++;
        // board.print_board();
        // std::cout<<std::endl;
        auto moves = model.get_legal_moves(player,model.get_board());
        if (moves.empty()){
            model.print_board();
        }
        std::uniform_int_distribution<int> genRand(0, moves.size()-1);
        int move = genRand(rng);
        if (player){
            player1.make_move(moves.at(move));
        }else{
            player0.make_move(moves.at(move));
        }
        player = !player;
        int game_status = model.game_over_check(model.get_board());
        // std::cout<<game_status<<std::endl;
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = end - start;
        // if (duration.count()>60){
        //     board.print_board();
        //     std::cout<<std::endl<<game_status<<std::endl;
        // }
        if (game_status==0){
            continue;
        }else{
            return game_status;
        }
    }
    return 0;
}
std::vector<float> simulation_mcts_minmax(std::vector<int> genotype, int depth, float resources, float c_parameter, bool is_mcts_first_player){
    // auto start = std::chrono::high_resolution_clock::now();
    Model board;
    Player player0(board,0, genotype);
    Player player1(board,1, genotype);
    float time1 = 0;
    float time2 = 0;
    bool game = true;
    bool player = 0;
    int moves = 0;
    while(game){
        moves++;
        if (moves >1000){
            return std::vector<float>{3,time1/float(std::ceil((moves/2))),time2/float(std::floor((moves/2)))};
        }
        // board.print_board();
        std::cout<<moves;
        if(is_mcts_first_player){
            if (!player){
                auto start = std::chrono::high_resolution_clock::now();
                player0.make_mcts_move(resources,c_parameter);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                time1 += duration.count();
            }else{
                auto start = std::chrono::high_resolution_clock::now();
                player1.make_minmax_move(depth);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                time2 += duration.count();
            }
        }else{
            if (!player){
                auto start = std::chrono::high_resolution_clock::now();
                player0.make_minmax_move(depth);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                time1 += duration.count();
            }else{
                auto start = std::chrono::high_resolution_clock::now();
                player1.make_mcts_move(resources,c_parameter);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                time2 += duration.count();
            }
        }
        player = !player;
        int game_status = board.game_over_check(board.get_board());
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = end - start;
        // if (duration.count()>60){
        //     board.print_board();
        //     std::cout<<std::endl<<game_status<<std::endl;
        // }
        if (game_status==0){
            continue;
        }else{
            return std::vector<float>{float(game_status),time1/float(std::ceil((moves/2))),time2/float(std::floor((moves/2)))};
        }
        
    }
}
int simulation_mcts_mcts(float resources, float c_parameter, float resources2, float c_parameter2){
    Model board;
    Player player0(board,0, std::vector(26,0));
    Player player1(board,1, std::vector(26,0));
    bool game = true;
    bool player = 0;
    int moves = 0;
    while(game){
        if (moves >1000){
            return 3;
        }
        moves++;
        std::cout<<moves;
        if (!player){
            player0.make_mcts_move(resources,c_parameter);
        }else{
            player1.make_mcts_move(resources2,c_parameter2);
        }
        player = !player;
        int game_status = board.game_over_check(board.get_board());
        if (game_status==0){
            continue;
        }else{
            return game_status;
        }
        
    }
    return 0;
}
