//
// Created by amil on 06/02/25.
//
#include "Agent.h"

// Constructeur avec solution et target
Agent::Agent(const std::vector<double>& solution, const Target& target)
    : solution(solution), target(target) {}

// Méthode de copie
Agent Agent::copy() const {
    Agent agent(*this); // Copie de l’instance actuelle

    // Ajout d’autres attributs si nécessaire (non précisés dans la classe d'origine)
    return agent;
}

// Getters
std::vector<double> Agent::get_solution() const {
    return solution;
}

Target Agent::get_target() const {
    return target;
}

// Setters
void Agent::set_solution(const std::vector<double>& solution) {
    this->solution = solution;
}

void Agent::set_target(const Target& target) {
    this->target = target;
}
