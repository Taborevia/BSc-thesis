#include "genetic_algorithm.hpp"

#include "algorithm"
#include "simulation.hpp"
#include <fstream>
#include <chrono>
#include <thread>
#include <future>
#include <iostream>
#include <sstream>
#define MAX_WAGE 128
#define MIN_WAGE -127
std::ofstream output("genotypy.txt");


individual::individual(int genotypeLength) : genotype(genotypeLength, 0), fitness(0) {}
individual::individual() : fitness(0) {}
bool individual::operator<(const individual& other) const {
        return fitness < other.fitness;
}
bool individual::operator>(const individual& other) const {
        return fitness > other.fitness;
}
void GA::mutation(std::vector<individual>& population, float propability, std::mt19937& rng){
    int n = population.size();
    int size = population.at(0).genotype.size();
    std::uniform_int_distribution<int> genRand(0, size-1);
    std::uniform_int_distribution<int> weightRand(0,64);
    std::uniform_real_distribution<float> rand(0,1);
    float random = 0;
    for (int i = 0;i<n;i++){
        random = rand(rng);
        if (random<=propability){
            std::cout<<"to tu"<<std::endl;
            population.at(i).genotype.at(genRand(rng)) = weightRand(rng);     
            std::cout<<"po tu"<<std::endl;
        }        
    }
}
void GA::fitnessAll(std::vector<individual>& population){
    int size = population.size();
    int result;
    for(int i = 0;i<size;i++){
        population.at(i).fitness = 0;
    }
    std::vector<std::future<std::vector<int>>> results;
    std::cout<<"tutaj1"<<std::endl;
    for(int i = 0; i<size;i++){
        for (int j = i+1; j<size;j++){
            results.push_back(std::async(std::launch::async,simulation,population.at(i).genotype,population.at(j).genotype,4,4,i,j));
            results.push_back(std::async(std::launch::async,simulation,population.at(j).genotype,population.at(i).genotype,4,4,j,i));
        }
    }
    std::cout<<"tutaj2"<<std::endl;
    for (auto& result : results){
        std::vector<int> info = result.get();
        switch (info.at(0)){
            case 1:
                population.at(info.at(1)).fitness += 1;
                break;
            case 2:
                population.at(info.at(2)).fitness += 1;
                break;
            case 3:
                population.at(info.at(1)).fitness += 0.5;
                population.at(info.at(2)).fitness += 0.5;
                break;
        }
    }
    std::cout<<"tutaj3"<<std::endl;

}
void GA::fitnessAll_2(std::vector<individual>& population){
    int size = population.size();
    int result;
    for(int i = 0;i<size;i++){
        population.at(i).fitness = 0;
    }
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
    std::uniform_int_distribution<int> genRand(0, size-1);

    std::vector<std::future<std::vector<int>>> results;
    std::cout<<"tutaj1"<<std::endl;
    for(int i = 0; i<size;i++){
        for (int j = 0; j<10;j++){
            int tester = genRand(rng);
            results.push_back(std::async(std::launch::async,simulation,population.at(i).genotype,population.at(tester).genotype,4,4,i,-1));
            results.push_back(std::async(std::launch::async,simulation,population.at(tester).genotype,population.at(i).genotype,4,4,-1,i));
        }
    }
    std::cout<<"tutaj2"<<std::endl;
    for (auto& result : results){
        std::vector<int> info = result.get();
        if (info.at(1)!=-1){
            switch (info.at(0)){
            case 1:
                population.at(info.at(1)).fitness += 1;
                break;
            case 3:
                population.at(info.at(1)).fitness += 0.5;
                break;
            }
        }else{
            switch (info.at(0)){
            case 2:
                population.at(info.at(2)).fitness += 1;
                break;
            case 3:
                population.at(info.at(2)).fitness += 0.5;
                break;
            }   
        }
    } 
}
//miara zbieznosci, wiadomo ze jesli 1 genotyp rozni sie z innymi maksymalnie o $max to one miedzy soba roznia sie najwyzej o 2*$max
bool GA::genetic_end_check(std::vector<individual>& population,int max){
    int number_of_differences;
    for(int i = 1;i<population.size();i++){
        number_of_differences = 0;
        for (int j=0;j<population.at(0).genotype.size();j++){
            if (population.at(0).genotype.at(j)!=population.at(i).genotype.at(j)){
                number_of_differences++;
            }
        }
        if (number_of_differences>max){
            return false;
        }
    }
    return true;
}
void GA::crossover_roulette(std::vector<individual>& population, std::mt19937& rng){
    int size = population.size();
    float sum_of_fitness=0;
    std::vector<float> propabilities;
    std::uniform_real_distribution<float> dist(0,1);
    std::bernoulli_distribution dist2(0.5);
    for (int i = 0;i<size;i++){
        sum_of_fitness += population.at(i).fitness;
    }
    for (int i = 0;i<size;i++){
        propabilities.push_back(population.at(i).fitness/sum_of_fitness);
        
    }
    for (int i = size-1;i>=0;i--){
        for (int j =0;j<i;j++){
            propabilities.at(i)+=propabilities.at(j);
        }
    }
    
    for (int i = 0;i<size;i++){
        std::cout<<propabilities.at(i)<<" ";
    }
    std::cout<<std::endl;
    for (int pairs = 0; pairs<(size/2);pairs++){
        std::array<individual,2> parents;     
        for (int i = 0;i<2;i++){
            float random = dist(rng);
            std::cout<<"random: "<<random;
            for (int j = 0;j<size;j++){
                if (propabilities.at(j)>=random || j == size-1){
                    parents.at(i) = population.at(j);
                    std::cout<<"wybrano: "<<j<<" ";
                    break;
                }        
            }
        }
        std::cout<<std::endl;
        individual child1;
        individual child2;
        //DOOOOO SPRAWDZENIAAAAA
        for (int k = 0;k<parents.at(0).genotype.size();k++){
            if (dist2(rng)){
                child1.genotype.push_back(parents.at(0).genotype.at(k));
                child2.genotype.push_back(parents.at(1).genotype.at(k));
            }else{
                child1.genotype.push_back(parents.at(1).genotype.at(k));
                child2.genotype.push_back(parents.at(0).genotype.at(k));
            }
        }
        // for(int i =0;i<child1.genotype.size();i++){
        //     std::cout<< child1.genotype.at(i)<<" ";
        // }
        // std::cout<<std::endl;
        // for(int i =0;i<child1.genotype.size();i++){
        //     std::cout<< child2.genotype.at(i)<<" ";
        // }
        // std::cout<<std::endl;
        // for(int i =0;i<child1.genotype.size();i++){
        //     std::cout<< parents.at(0).genotype.at(i)<<" ";
        // }
        // std::cout<<std::endl;
        // for(int i =0;i<child1.genotype.size();i++){
        //     std::cout<< parents.at(1).genotype.at(i)<<" ";
        // }
        // std::cout<<std::endl;
        population.push_back(child1);
        population.push_back(child2);
    }
    population.erase(population.begin(),population.begin()+size);
}
void GA::crossover(std::vector<individual>& population, std::mt19937& rng){
    std::bernoulli_distribution dist(0.5);
    std::vector<individual> parents;
    for (int i = 0;i<floor(sqrt(2.0)*sqrt(population.size())-3);i++){
        parents.push_back(population.at(i));
    }
    population.clear();
    std::cout<<"parents: "<<parents.size();
    for (int i = 0;i<parents.size();i++){
        for (int j = i+1; j<parents.size();j++){
            individual child1;
            individual child2;
            for (int k = 0;k<parents.at(1).genotype.size();k++){
                if (dist(rng)){
                    child1.genotype.push_back(parents.at(i).genotype.at(k));
                    child2.genotype.push_back(parents.at(j).genotype.at(k));
                }else{
                    child1.genotype.push_back(parents.at(j).genotype.at(k));
                    child2.genotype.push_back(parents.at(i).genotype.at(k));
                }
            }
            population.push_back(child1);
            population.push_back(child2);
        }
    }
}
std::vector<int> makeRandomVector(int n, std::mt19937& rng){
    std::uniform_int_distribution<int> distrib(MIN_WAGE,MAX_WAGE);
    std::vector<int> weights;
    for (int i = 0; i < n; i++) {
        weights.push_back(distrib(rng));
    }
    return weights;
}
void GA::genetic_algorithm(int populationSize, int numberOfGenerations, float propabilityOfMutation, int numberOfWages,  std::mt19937& rng){
    std::vector<individual> population(populationSize);
    std::ifstream inputFile("temp_genotypy.txt");  // Nazwa pliku z danymi
    std::string line;
    int numberOfIndividual = 0;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::vector<int> currentVector;  // Tymczasowy wektor dla bieżącej linii
        int value;
        
        while (ss >> value) {
            currentVector.push_back(value);
        }

        if (!currentVector.empty()) {
            population.at(numberOfIndividual).genotype = currentVector;
        }
        numberOfIndividual++;
    }
    inputFile.close();
    // for (int i = 0;i<populationSize;i++){
    //     population.at(i).genotype = makeRandomVector(numberOfWages,rng);
    // }
    
    for (int generation = 0;generation<numberOfGenerations;generation++){
        
        std::cout<<generation<<" "<<population.size()<<std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        fitnessAll_2(population);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout<<population.size()<<" czas: "<<duration.count()<<std::endl;
        output<<std::endl;
        std::sort(population.begin(),population.end(), [](individual a, individual b){return a>b;});
        for (int i = 0;i<population.size();i++){
            for (int j = 0;j<numberOfWages;j++){
                output<<population.at(i).genotype.at(j)<<" ";
            }
            output<<std::endl;
        }
        for(int i = 0;i<population.size();i++){
            output<<population.at(i).fitness<<" ";
        }population.resize(populationSize); //wyrzucenie nadmiarowych najgorszych obiektów
        // crossover(population, rng);
        crossover_roulette(population, rng);
        mutation(population, propabilityOfMutation, rng);
        if(genetic_end_check(population,1)){
            break;
        }
    }
}
int main(){
    GA test;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
    test.genetic_algorithm(1000,10000,0.01,26,rng);
    //test fitnessall
    // std::vector<individual> population(2);
    // population.at(0).genotype = makeRandomVector(26,rng);
    // population.at(1).genotype = makeRandomVector(26,rng);
    // std::cout<<simulation(population.at(0).genotype, population.at(1).genotype,4,4,0,1).at(0)<<std::endl;
    // std::cout<<simulation(population.at(1).genotype,population.at(0).genotype, 4,4,1,0).at(0)<<std::endl;
    // test.fitnessAll(population);
    // std::cout<<population.at(0).fitness<<" "<<population.at(1).fitness;
}
