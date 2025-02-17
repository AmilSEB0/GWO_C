//
// Created by amil on 06/02/25.
//
#include "Agent.h"

// Constructeur avec solution et fitness
Agent::Agent(const std::vector<double>& solution, double fitness)
    : solution(solution), fitness(fitness) {}

// Constructeur
Agent::Agent() : fitness(0.0) {}

// Méthode de copie
Agent* Agent::copy() const {
    Agent* agent = new Agent(*this); // Allocation dynamique d'une copie de l'instance actuelle
    return agent;
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

