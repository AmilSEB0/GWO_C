#include "Agent.h"
#include <memory>  // Ajout pour shared_ptr

// Constructeur avec solution et fitness
Agent::Agent(const std::vector<double>& solution, double fitness)
    : solution(solution), fitness(fitness) {}

// Constructeur par défaut
Agent::Agent() : fitness(0.0) {}

// Méthode de copie
std::shared_ptr<Agent> Agent::copy() const {
    // Retourne un shared_ptr pointant vers une copie de l'instance actuelle
    return std::make_shared<Agent>(*this);  // Utilisation de make_shared pour la gestion automatique de la mémoire
}

// Getters
std::vector<double> Agent::get_solution() const {
    return solution;
}

double Agent::get_target() const {
    return fitness;
}

// Setters
void Agent::set_solution(const std::vector<double>& solution) {
    this->solution = solution;
}

void Agent::set_target(double fitness) {
    this->fitness = fitness;
}

