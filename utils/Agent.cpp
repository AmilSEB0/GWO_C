#include "Agent.h"
#include <memory>  // Ajout pour shared_ptr

// Constructeur avec solution et fitness
Agent::Agent(const std::vector<double>& solution, double fitness)
    : solution(solution), fitness(fitness) {}  // Initialisation des valeurs de solution et de fitness

// Constructeur par défaut
Agent::Agent() : fitness(0.0) {}  // Initialisation de fitness à 0.0, solution vide par défaut

// Méthode de copie
std::shared_ptr<Agent> Agent::copy() const {
    // Retourne un shared_ptr pointant vers une copie de l'instance actuelle
    return std::make_shared<Agent>(*this);  // Utilisation de make_shared pour gérer la mémoire automatiquement
}

// Getters
std::vector<double> Agent::get_solution() const {
    return solution;  // Retourne la solution de l'agent
}

double Agent::get_target() const {
    return fitness;  // Retourne la fitness de l'agent
}

// Setters
void Agent::set_solution(const std::vector<double>& solution) {
    this->solution = solution;  // Met à jour la solution de l'agent
}

void Agent::set_target(double fitness) {
    this->fitness = fitness;  // Met à jour la fitness de l'agent
}

