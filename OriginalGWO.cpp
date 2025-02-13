//
// Created by amil on 12/02/25.
//
#include "OriginalGWO.h"
#include "Optimizer.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

OriginalGWO::OriginalGWO(int epoch, int pop_size) : Optimizer() {
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
std::vector<double> OriginalGWO::generate_random_vector(int n_dims) {
    std::vector<double> vec(n_dims);
    for (int i = 0; i < n_dims; ++i) {
        vec[i] = std::uniform_real_distribution<>(0.0, 1.0)(generator);
    }
    return vec;
}

// Fonction qui réalise l'évolution de la population à chaque itération (époque)
/*
void Optimizer::evolve(int epoch) {
    // Calcul du paramètre 'a' qui décroît linéairement de 2 à 0
    double a = 2.0 - 2.0 * static_cast<double>(epoch) / static_cast<double>(this->epoch);

    // Tri de la population selon la fitness (en fonction du critère minmax)
    std::vector<double> list_fits;
    for (Agent* agent : this->pop) {
        list_fits.push_back(agent->get_target().fitness());
    }

    std::vector<size_t> indices(list_fits.size());
    std::iota(indices.begin(), indices.end(), 0);  // Remplir le tableau d'indices

    if (this->problem->getMinMax() == "max") {
        std::sort(indices.begin(), indices.end(), [&list_fits](size_t i1, size_t i2) {
            return list_fits[i1] > list_fits[i2];  // Ordre décroissant
        });
    } else {
        std::sort(indices.begin(), indices.end(), [&list_fits](size_t i1, size_t i2) {
            return list_fits[i1] < list_fits[i2];  // Ordre croissant
        });
    }

    // Recréer la population triée
    std::vector<Agent*> sorted_pop;
    for (size_t idx : indices) {
        sorted_pop.push_back(this->pop[idx]);
    }
    this->pop = sorted_pop;

    // Récupérer les 3 meilleurs agents
    std::vector<Agent*> list_best = {this->pop[0], this->pop[1], this->pop[2]};  // Les 3 premiers agents sont les meilleurs

    // Initialisation de la nouvelle population
    std::vector<Agent*> pop_new;

    for (size_t idx = 0; idx < this->pop_size; ++idx) {
        // Générer les valeurs A1, A2, A3, C1, C2, C3 pour chaque agent
        std::vector<double> A1 = a * (2.0 * generate_random_vector(this->problem->getNDims()) - 1.0);
        std::vector<double> A2 = a * (2.0 * this->generate_random_vector(this->problem->n_dims) - 1.0);
        std::vector<double> A3 = a * (2.0 * this->generate_random_vector(this->problem->n_dims) - 1.0);

        std::vector<double> C1 = 2.0 * this->generate_random_vector(this->problem->n_dims);
        std::vector<double> C2 = 2.0 * this->generate_random_vector(this->problem->n_dims);
        std::vector<double> C3 = 2.0 * this->generate_random_vector(this->problem->n_dims);

        // Calcul des nouvelles positions X1, X2, X3
        std::vector<double> X1 = list_best[0]->solution;
        std::vector<double> X2 = list_best[1]->solution;
        std::vector<double> X3 = list_best[2]->solution;

        for (size_t i = 0; i < this->problem->n_dims; ++i) {
            X1[i] -= A1[i] * std::abs(C1[i] * list_best[0]->solution[i] - this->pop[idx]->solution[i]);
            X2[i] -= A2[i] * std::abs(C2[i] * list_best[1]->solution[i] - this->pop[idx]->solution[i]);
            X3[i] -= A3[i] * std::abs(C3[i] * list_best[2]->solution[i] - this->pop[idx]->solution[i]);
        }

        // Calcul de la position moyenne
        std::vector<double> pos_new(this->problem->n_dims);
        for (size_t i = 0; i < this->problem->n_dims; ++i) {
            pos_new[i] = (X1[i] + X2[i] + X3[i]) / 3.0;
        }

        // Corriger la solution (si nécessaire)
        pos_new = correct_solution(pos_new);

        // Générer un nouvel agent avec la nouvelle position
        Agent* agent = generate_empty_agent(pos_new);
        pop_new.push_back(agent);

        // Mise à jour de la cible de l'agent
        agent->target = get_target(pos_new);

        // Mettre à jour l'agent dans la population
        this->pop[idx] = get_better_agent(agent, this->pop[idx], this->problem->minmax);
    }
}
*/

 // Evolve method (equivalent to the Python version)
    void OriginalGWO::evolve(int epoch) {
        // linearly decreased from 2 to 0
        double a = 2.0 - 2.0 * epoch / this->epoch;

        // Tri de la population selon la fitness
        std::vector<double> list_fits;
        for (auto& agent : pop) {
            list_fits.push_back(agent->get_target().fitness());
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

        for (int idx = 0; idx < pop_size; ++idx) {
            // Génération de valeurs aléatoires pour A1, A2, A3, C1, C2, C3
            std::vector<double> random_vector = generate_random_vector(problem->getNDims() - 1); // Générer un vecteur aléatoire

            std::vector<double> A1(random_vector.size());
            std::vector<double> A2(random_vector.size());
            std::vector<double> A3(random_vector.size());
            std::vector<double> C1(random_vector.size());
            std::vector<double> C2(random_vector.size());
            std::vector<double> C3(random_vector.size());

            // Multiplier chaque élément par 'a'
            for (size_t i = 0; i < random_vector.size(); ++i) {
                A1[i] = a * random_vector[i];
                A2[i] = a * random_vector[i];
                A3[i] = a * random_vector[i];
                C1[i] = 2.0 * random_vector[i];
                C2[i] = 2.0 * random_vector[i];
                C3[i] = 2.0 * random_vector[i];
            }

            // Calcul de la nouvelle position des agents
            std::vector<double> X1 = list_best[0]->get_solution();
            std::vector<double> X2 = list_best[1]->get_solution();
            std::vector<double> X3 = list_best[2]->get_solution();

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
            agent->get_target() = get_target(pos_new);
            pop[idx] = get_better_agent(agent, pop[idx], problem->getMinMax());
        }

        // Mise à jour de la population avec la nouvelle population
        pop = pop_new;
    }
