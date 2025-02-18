#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <memory>
#include <vector>
#include "utils/Agent.h"
#include "utils/Problem.h"
#include <string>
#include <random>
#include <functional>
#include <iostream>
#include <chrono>
#include <map>
#include <type_traits>

class Optimizer {
public:
    // Constructeur
    Optimizer(int epoch, int pop_size);

    // Méthodes principales
    std::shared_ptr<Agent> solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions = {}, int seed = 0);
    virtual void evolve(int epoch) = 0;

    virtual ~Optimizer();

    std::shared_ptr<Agent> get_global_best() const;

protected:
    void before_initialization(const std::vector<std::vector<double>>& starting_solutions = {});
    void after_initialization();
    void check_problem(Problem* problem);
    std::shared_ptr<Agent> generate_empty_agent(const std::vector<double>& solution = {});
    std::shared_ptr<Agent> generate_agent(const std::vector<double>& solution = {});
    std::vector<std::shared_ptr<Agent>> generate_population(int pop_size = 0);
    std::shared_ptr<Agent> update_global_best_agent(std::vector<std::shared_ptr<Agent>>& pop, bool save = true);

    std::vector<double> correct_solution(const std::vector<double>& solution);
    double get_target(const std::vector<double>& solution, bool counted = true);
    static std::shared_ptr<Agent> get_better_agent(std::shared_ptr<Agent> agent_x, std::shared_ptr<Agent> agent_y, const std::string& minmax = "min", bool reverse = false);

    std::vector<double> generate_random_vector(int n_dims);

    void track_optimize_step(std::vector<std::shared_ptr<Agent>>& pop, int epoch, double runtime);

    std::vector<std::shared_ptr<Agent>> pop;
    Problem* problem;

private:
    int epoch;
    int pop_size;
    std::shared_ptr<Agent> g_best;
    std::shared_ptr<Agent> g_worst;
    std::vector<std::shared_ptr<Agent>> list_global_best;
    std::vector<std::shared_ptr<Agent>> list_current_best;
    std::vector<std::shared_ptr<Agent>> list_global_worst;
    std::vector<std::shared_ptr<Agent>> list_current_worst;
    std::vector<double> list_epoch_time;
    std::vector<double> list_global_best_fit;
    std::vector<std::shared_ptr<Agent>> list_current_best_fit;
    std::vector<double> list_diversity;
};
#endif // OPTIMIZER_H
