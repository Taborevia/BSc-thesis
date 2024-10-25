#ifndef GA_H
#define GA_H

#include <vector>
#include <array>
#include <random>
#define GENOTYPE_SIZE 3

struct individual {
    std::vector<int> genotype;
    float fitness;
    individual(int genotypeLength);
    individual();
    bool operator<(const individual& other) const;
    bool operator>(const individual& other) const;
};


class GA{
public:
    std::vector<std::array<int, GENOTYPE_SIZE>> population; 
    void mutation(std::vector<individual>& population, float propability, std::mt19937& rng);
    void crossover(std::vector<individual>& population, std::mt19937& rng);
    void crossover_roulette(std::vector<individual>& population, std::mt19937& rng);
    void fitnessAll(std::vector<individual>& population);
public:
    void genetic_algorithm(int populationSize, int numberOfGenerations, float propabilityOfMutation, int numberOfWages,  std::mt19937& rng);
};
#endif