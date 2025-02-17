//
// Created by amil on 12/02/25.
//
#include "OriginalGWO.h"
#include "Optimizer.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

OriginalGWO::OriginalGWO(int epoch, int pop_size) : Optimizer(epoch, pop_size) {
    this->epoch = checkInt("epoch", epoch, {1, 100000});
    this->pop_size = checkInt("pop_size", pop_size, {5, 10000});
}

OriginalGWO::~OriginalGWO() {

}


double OriginalGWO::checkInt(const std::string& name, double value, std::pair<int, int> bound) {
    if (value < bound.first || value > bound.second) {
        throw std::invalid_argument(name + " must be in range [" + std::to_string(bound.first) + ", " + std::to_string(bound.second) + "].");
    }
    return static_cast<int>(value);
}

// Fonction pour générer un vecteur de nombres aléatoires
std::vector<double> OriginalGWO::generate_random_vector(int size, double min, double max) {
    std::vector<double> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = min + (max - min) * ((double) rand() / RAND_MAX); // Génère des valeurs entre min et max
    }
    return vec;
}

 // Evolve method (equivalent to the Python version)
void OriginalGWO::evolve(int epoch) {
    // linearly decreased from 2 to 0
    double a = 2.0 - 2.0 * epoch / this->epoch;

    // Tri de la population selon la fitness
    std::vector<double> list_fits;
    for (auto& agent : pop) {
        list_fits.push_back(agent->get_target());
    }

    std::vector<int> indices(list_fits.size());
    std::iota(indices.begin(), indices.end(), 0); // Initialize indices with 0, 1, ..., n-1

    std::sort(indices.begin(), indices.end(), [&list_fits](int i1, int i2) {
        return list_fits[i1] < list_fits[i2]; // Tri par fitness
    });

    if (problem->getMinMax() == "max") {
        std::reverse(indices.begin(), indices.end()); // Inverser si l'optimisation est en maximisation
    }

    // Recréer la population triée
    std::vector<Agent*> sorted_pop;
    for (auto idx : indices) {
        sorted_pop.push_back(pop[idx]);
    }
    pop = sorted_pop;

    // Récupérer les 3 meilleurs agents
    std::vector<Agent*> list_best(pop.begin(), pop.begin() + 3);

    // Initialisation de la nouvelle population
    std::vector<Agent*> pop_new;

    // Générer des nouveaux agents pour la population
    for (int idx = 0; idx < pop_size; ++idx) {
        // Générer des vecteurs aléatoires pour A1, A2, A3, C1, C2, C3
        std::vector<double> A1 = generate_random_vector(problem->getNDims(), -1.0, 1.0);
        std::vector<double> A2 = generate_random_vector(problem->getNDims(), -1.0, 1.0);
        std::vector<double> A3 = generate_random_vector(problem->getNDims(), -1.0, 1.0);
        std::vector<double> C1 = generate_random_vector(problem->getNDims(), 0.0, 2.0);
        std::vector<double> C2 = generate_random_vector(problem->getNDims(), 0.0, 2.0);
        std::vector<double> C3 = generate_random_vector(problem->getNDims(), 0.0, 2.0);

        // Calcul de la nouvelle position des agents
        std::vector<double> X1 = list_best[0]->get_solution();
        std::vector<double> X2 = list_best[1]->get_solution();
        std::vector<double> X3 = list_best[2]->get_solution();

        // Mise à jour des positions X1, X2, X3 en fonction des formules
        for (size_t i = 0; i < X1.size(); ++i) {
            X1[i] -= A1[i] * std::abs(C1[i] * X1[i] - pop[idx]->get_solution()[i]);
            X2[i] -= A2[i] * std::abs(C2[i] * X2[i] - pop[idx]->get_solution()[i]);
            X3[i] -= A3[i] * std::abs(C3[i] * X3[i] - pop[idx]->get_solution()[i]);
        }

        // Calcul de la position moyenne
        std::vector<double> pos_new(X1.size());
        for (size_t i = 0; i < X1.size(); ++i) {
            pos_new[i] = (X1[i] + X2[i] + X3[i]) / 3.0;
        }

        // Correction de la position
        pos_new = correct_solution(pos_new);

        // Générer un agent avec la nouvelle position
        Agent* agent = generate_empty_agent(pos_new);
        pop_new.push_back(agent);

        // Mise à jour de la cible de l'agent
        agent->set_target(get_target(pos_new));

        // Mise à jour de la population avec l'agent amélioré
        pop[idx] = get_better_agent(agent, pop[idx], problem->getMinMax());
    }

    // Mise à jour de la population
    pop = pop_new;
}
