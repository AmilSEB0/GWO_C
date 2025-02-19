#ifndef ORIGINAL_GWO_H
#define ORIGINAL_GWO_H

#include <vector>
#include <memory>
#include "utils/Agent.h"
#include "utils/Problem.h"
#include <random>
#include <chrono>
#include <map>

class OriginalGWO {
public:
    OriginalGWO(int epoch = 10000, int pop_size = 100);

    void evolve(int epoch);

    ~OriginalGWO();

    std::vector<double> generate_random_vector(int size, double min, double max);

    // Méthodes principales
    std::shared_ptr<Agent> solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions = {}, int seed = 0);

    std::shared_ptr<Agent> get_global_best() const;
private:
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
    int epoch;
    int pop_size;
    std::mt19937 generator;
    double checkInt(const std::string& name, double value, std::pair<int, int> bound = {1, 100000});
    std::shared_ptr<Agent> g_best;
    std::vector<std::shared_ptr<Agent>> list_global_best;
    std::vector<std::shared_ptr<Agent>> list_current_best;
    std::vector<double> list_epoch_time;
    std::vector<double> list_global_best_fit;
    std::vector<std::shared_ptr<Agent>> list_current_best_fit;
    std::vector<double> list_diversity;
};

#endif // ORIGINAL_GWO_H
