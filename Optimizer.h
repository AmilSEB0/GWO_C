#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include <string>
#include <random>
#include <functional>
#include <iostream>
#include <chrono>
#include <map>
#include <type_traits>
#include "utils/Agent.h"
#include "utils/Problem.h"
#include "utils/Target.h"

class Optimizer {
public:

    // Constructeur
    Optimizer();

    // Méthodes principales
    Agent* solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions = {}, int seed = 42);
    // Mise à jour des solutions
    virtual void evolve(int epoch) = 0;

    virtual ~Optimizer() = default; // destructeur virtuel par défaut

protected:
    // Initialisation
    void before_initialization(const std::vector<std::vector<double>>& starting_solutions = {});
    void after_initialization();
    void check_problem(Problem* problem);

    // Génération des agents et de la population
    Agent* generate_empty_agent(const std::vector<double>& solution = {});
    Agent* generate_agent(const std::vector<double>& solution = {});
    std::vector<Agent*> generate_population(int pop_size = 0);

    // Génération des solutions optimisées
    std::vector<double> generate_solution(bool encoded);  // Déclaration de la méthode

    // Optimisation et suivi
    Agent* update_global_best_agent(std::vector<Agent*>& pop, bool save = true);

    // Utilitaires
    std::vector<double> correct_solution(const std::vector<double>& solution);
    Target get_target(const std::vector<double>& solution, bool counted = true);
    static Agent* get_better_agent(Agent* agent_x, Agent* agent_y, const std::string& minmax = "min", bool reverse = false);

    std::vector<double> generate_random_vector(int n_dims);

    // Suivi du processus
    void track_optimize_step(std::vector<Agent*>& pop, int epoch, double runtime);
    void track_optimize_process();
    std::vector<Agent*> pop;
    Problem* problem;
private:
    // Variables membres
    int epoch, pop_size;
    Agent* g_best, *g_worst;

    // Historique de la performance
    std::vector<double> list_epoch_time;
    std::vector<Agent*> list_global_best;
    std::vector<Agent*> list_current_best;
    std::vector<Agent*> list_global_worst;
    std::vector<Agent*> list_current_worst;
    std::vector<double> list_global_best_fit;
    std::vector<Agent*> list_current_best_fit;
    std::vector<double> list_diversity;
    std::vector<double> list_exploitation;
    std::vector<double> list_exploration;

    std::mt19937 generator; // Générateur de nombres aléatoires
};
#endif // OPTIMIZER_H
