//
// Created by amil on 10/02/25.
//
#include "Space.h"

#include <stdexcept>

// --- BaseVar ---

// Constructeur de BaseVar
BaseVar::BaseVar(const std::string& name) : name(name), n_vars(0), _seed(0) {}

// Getter pour le seed
int BaseVar::get_seed() const {
    return _seed;
}

// Setter pour le seed
void BaseVar::set_seed(int value) {
    _seed = value;
    generator.seed(_seed);  // Re-seed le générateur de nombres aléatoires
}

// --- FloatVar ---

// Constructeur pour FloatVar
FloatVar::FloatVar(double lb, double ub, const std::string& name) : BaseVar(name) {
    _set_bounds(lb, ub);
}

// Méthode pour définir les bornes de FloatVar
void FloatVar::_set_bounds(double lb, double ub) {
    if (lb == ub) {
        this->lb.push_back(lb);
        this->ub.push_back(ub);
        n_vars = 1;
    } else if (lb < ub) {
        this->lb.push_back(lb);
        this->ub.push_back(ub);
        n_vars = 1;
    } else {
        throw std::invalid_argument("Invalid bounds. Lower bound must be less than upper bound.");
    }
}