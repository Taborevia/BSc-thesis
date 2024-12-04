#ifndef SIMULATION_H
#define SIMULATION_H

#include "model.hpp"
#include "player.hpp"
#include <vector>
#include <chrono>
#include <random>

// Deklaracje funkcji
std::vector<int> simulation(std::vector<int> genotype0, std::vector<int> genotype1, int depth0, int depth1, int numberOfIndividual0, int numberOfIndividual1);
int simulation_mcts_minmax(std::vector<int> genotype, int depth, float resources, float c_parameter);
int random_simulation(std::array<std::array<int, 11>, 2> board, bool is_maximazing_player);
int simulation_mcts_mcts(float resources, float c_parameter, float resources2, float c_parameter2);
#endif // SIMULATION_H

