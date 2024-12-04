#include "model.hpp"
#include "player.hpp"
#include "simulation.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>

int main(int argc, char* argv[]){
    // 88, 114, 87, 14, 24, 48, 21, 7, 57, 106, 97, 39, -18, 68, -81, 12, 38, 86, 88, 0, 56, 54, 9, 93, 61, 63
    // 124, 45, 39, -39, 106, -40, 119, 31, 37, 64, 36, 11, -18, 31, -9, 82, -17, -13, 119, 33, 43, 59, -14, 2, -47, 96
    // 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    // 120 46 87 10 -56 -97 -43 -58 30 11 23 51 112 -114 -28 48 -90 39 -116 61 114 -9 65 34 43 95 
    // 103 76 5 -53 18 -99 66 4 -32 101 -16 54 61 38 74 117 106 -47 42 25 113 93 22 -104 -45 57 
    // 103, 93, 48, 97, 70, 66, 114, 27, 76, 89, 81, 91, 89, 60, 47, -11, 104, 54, -58, 17, 79, 68, 109, -19, 56, 61
    // 103, 95, 4, 97, 84, 66, 114, -109, 76, 89, 81, 91, 89, 60, 47, -11, -18, 54, -58, 17, 79, 68, 116, -19, 35, 61
    // 110, 102, 22, 28, 79, 95, 72, 87, 55, 53, 81, 116, 18, 118, 58, 37, 52, -105, 16, 13, 38, 42, 40, -20, 45, 8
    // 128, 128, 34, 121, 68, 55, 52, 91, 76, 60, 34, 22, 107, 96, 75, 21, 32, 70, 23, 21, 43, 37, 40, 33, 44, 52
    // 123, 120, 11, 47, 75, 64, 48, 125, 127, 63, 10, 11, 106, 42, 21, 118, 81, 86, 63, 56, 57, 41, 30, 12, 43, 3 
    // 49, 120, 11, 47, 75, 64, 48, 42, 127, 63, 10, 54, 106, 42, 21, 118, 81, 86, 63, 56, 57, 41, 30, 12, 43, 3 
    // int result = simualtion_mcts_minmax(std::vector<int>{103, 93, 4, 97, 84, 66, 114, -109, 76, 89, 81, 91, 89, 60, 47, -11, -18, 54, -58, 17, 79, 68, 116, -19, 35, 61},2,0.3);
    //new
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> genotypy;
    std::vector<int> wyniki_1_new{117, 79, 11, 77, 22, 100, 73, 79, 48, -51, -3, 121, 30, 102, 88, 127, -114, 22, 60, 44, 53, 56, 60, -43, 53, -8}; //wyniki_1_new 
    std::vector<int> wyniki_2_new{34, 107, -2, 47, 92, 64, 26, 125, 26, 63, 112, 48, 66, 42, 63, 119, 47, 11, 63, 6, 80, 80, 6, 12, 22, 79};  //wyniki_2_new
    std::vector<int> wyniki_3_new{89, 126, 73, 10, 86, 102, 30, 111, 59, 50, 113, 104, 1, 74, 127, 55, -2, 21, 12, 37, 27, 15, 17, 49, 14, 16}; //wyniki_3_new
    std::vector<int> wyniki_4_new{123, 128, 11, 56, 28, 98, 55, 39, 89, 35, 37, 114, 4, 69, 109, 89, 41, 40, 29, 4, 23, 13, 18, 62, 4, 62};  //wyniki_4_new
    std::vector<int> funkcja_0{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    genotypy.push_back(funkcja_0);
    genotypy.push_back(wyniki_1_new);
    genotypy.push_back(wyniki_2_new);
    genotypy.push_back(wyniki_3_new);
    genotypy.push_back(wyniki_4_new);
    std::ofstream output("parametryzacja_mcts_2_test.txt");
    for (float i = 0;i<4;i+=0.5){
        for(int j=1;j<=10;j++){
            int result = simulation_mcts_mcts(100,i,0.2,i+0.2);
            output<<" "<<i<<" "<<i+0.5<<" "<<result<<std::endl;
            // output<<" "<<i+0.5<<" "<<i<<" "<<result<<std::endl;
            // output<<"parametr: "<<i<<"z botem o glebokosci: "<<j<<"wynik: "<<result<<std::endl;
            // result = simulation(genotypy.at(j), genotypy.at(i), std::stoi(argv[1]) ,std::stoi(argv[2]),0,0).at(0);
            // output<<"bot o glebokosci: "<<j<<"z parametrem: "<<i<<"wynik: "<<result<<std::endl;
            std::cout<<std::endl<<i<<" "<<j;
        }
        output<<std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Czas wykonania: " << duration.count() << " sekund" << std::endl;
}