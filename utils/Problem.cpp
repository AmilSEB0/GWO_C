#include "Problem.h"
#include <iostream>
#include <stdexcept>
#include <random>
#include <cmath>

Problem::Problem(const std::vector<FloatVar>& bounds, const std::string& minmax,
                 const std::function<std::vector<double>(const std::vector<double>&)>& obj_func)
    : bounds_(bounds), minmax_(minmax), obj_func_(obj_func) {
    set_bounds(bounds);
    set_functions();
}

void Problem::set_bounds(const std::vector<FloatVar>& bounds) {
    bounds_ = bounds;
    lb_.clear();
    ub_.clear();

    for (const auto& bound : bounds_) {
        const auto& lb_bound = bound.get_lb();
        const auto& ub_bound = bound.get_ub();
        lb_.insert(lb_.end(), lb_bound.begin(), lb_bound.end());
        ub_.insert(ub_.end(), ub_bound.begin(), ub_bound.end());
    }
}

void Problem::set_seed(int seed) {
    this->seed = seed;
    /*for (size_t idx = 0; idx < bounds_.size(); ++idx) {
        bounds_[idx].seed = seed;
    }*/
}

void Problem::set_functions() {
    std::vector<double> tested_solution = generate_solution(true);
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

std::vector<double> Problem::correct_solution(const std::vector<double>& x) const {
    std::vector<double> x_new;
    size_t n_vars = 0;

    for (const auto& var : bounds_) {
        // Pour chaque sous-ensemble de x, on applique std::clamp pour le limiter entre les bornes
        size_t var_n_vars = var.get_n_vars();
        for (size_t i = 0; i < var_n_vars; ++i) {
            double corrected_value = std::clamp(x[n_vars + i], var.get_lb()[i], var.get_ub()[i]);
            x_new.push_back(corrected_value);
        }
        n_vars += var_n_vars;
    }

    return x_new; // Solution corrigée
}
std::vector<double> Problem::generate_solution(bool encoded) {
    std::vector<double> x;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Logique pour générer la solution...
    for (const auto& var : bounds_) {
        for (size_t i = 0; i < var.get_n_vars(); ++i) {
            std::uniform_real_distribution<> dis(var.get_lb()[i], var.get_ub()[i]);
            x.push_back(dis(gen));
        }
    }

    return x;  // Retourner la solution générée
}

Target Problem::get_target(const std::vector<double>& solution) const {
    std::vector<double> objectives = obj_func_(solution);
    return Target(objectives, obj_weights_);
}
