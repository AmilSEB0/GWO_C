#include "Problem.h"
#include <iostream>
#include <stdexcept>
#include <random>
#include <cmath>

Problem::Problem(const std::vector<std::vector<double>>& lb, const std::vector<std::vector<double>>& ub, const std::string& minmax,
                 const std::function<std::vector<double>(const std::vector<double>&)>& obj_func)
    : lb_(lb), ub_(ub), minmax_(minmax), obj_func_(obj_func) {
    set_bounds(lb, ub);
    set_functions();
}

void Problem::set_bounds(const std::vector<std::vector<double>>& lb, const std::vector<std::vector<double>>& ub) {
    lb_ = lb;
    ub_ = ub;

    lb_flat_.clear();
    ub_flat_.clear();

    for (size_t i = 0; i < lb_.size(); ++i) {
        lb_flat_.insert(lb_flat_.end(), lb_[i].begin(), lb_[i].end());
        ub_flat_.insert(ub_flat_.end(), ub_[i].begin(), ub_[i].end());
    }
}

std::vector<double> Problem::correct_solution(const std::vector<double>& x) const {
    std::vector<double> x_new;
    size_t n_vars = 0;

    for (size_t i = 0; i < lb_.size(); ++i) {
        for (size_t j = 0; j < lb_[i].size(); ++j) {
            double corrected_value = std::clamp(x[n_vars + j], lb_[i][j], ub_[i][j]);
            x_new.push_back(corrected_value);
        }
        n_vars += lb_[i].size();
    }

    return x_new; // Solution corrigée
}

std::vector<double> Problem::generate_solution() {
    std::vector<double> x;
    std::random_device rd;
    std::mt19937 gen(rd());

    for (size_t i = 0; i < lb_.size(); ++i) {
        for (size_t j = 0; j < lb_[i].size(); ++j) {
            std::uniform_real_distribution<> dis(lb_[i][j], ub_[i][j]);
            x.push_back(dis(gen));
        }
    }

    return x;  // Retourner la solution générée
}

void Problem::set_functions() {
    std::vector<double> tested_solution = generate_solution();
    n_dims_ = tested_solution.size();
    std::vector<double> result = obj_func_(tested_solution);

    if (result.empty()) {
        throw std::invalid_argument("obj_func needs to return a non-empty vector of values.");
    }

    obj_weights_ = std::vector<double>(result.size(), 1.0); // Multi-objectif supporté
}

size_t Problem::getNDims() const {
    return n_dims_;
}

std::string Problem::getMinMax() const {
    return minmax_;
}

double Problem::get_fitness(const std::vector<double>& solution) const {
    std::vector<double> objectives = obj_func_(solution);
    double fitness = 0.0;

    // Calculer la fitness en utilisant les poids et les objectifs
    for (size_t i = 0; i < objectives.size(); ++i) {
        fitness += objectives[i] * obj_weights_[i];  // Produit scalaire des objectifs et des poids
    }

    return fitness;
}
