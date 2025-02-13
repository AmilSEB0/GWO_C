//
// Created by amil on 11/02/25.
//
#include "FloatVar.h"
#include <stdexcept>

// Constructeur pour tableaux de bornes
FloatVar::FloatVar(const std::vector<double>& lb, const std::vector<double>& ub) {
    _set_bounds(lb, ub);
}

// Méthode pour définir les bornes
void FloatVar::_set_bounds(const std::vector<double>& lb, const std::vector<double>& ub) {
    if (lb.size() == 1 && ub.size() == 1) {
        this->lb = lb;
        this->ub = ub;
        this->n_vars = 1;
    } else if (lb.size() == ub.size()) {
        this->lb = lb;
        this->ub = ub;
        this->n_vars = lb.size();
    } else {
        throw std::invalid_argument("Invalid lb or ub. Length of lb should equal length of ub.");
    }
}

// Getters
std::vector<double> FloatVar::get_lb() const {
    return lb;
}

std::vector<double> FloatVar::get_ub() const {
    return ub;
}

int FloatVar::get_n_vars() const {
    return n_vars;
}
