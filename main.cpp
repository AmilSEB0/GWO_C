#include <iostream>
#include <vector>
#include "OriginalGWO.h"
#include "utils/Problem.h"
#include "utils/FloatVar.h"

// Fonction objectif : somme des carrés des éléments de la solution
std::vector<double> objective_function(const std::vector<double>& solution) {
    std::vector<double> result(1, 0.0);  // Valeur retournée pour la cible (fitness)
    for (double value : solution) {
        result[0] += value * value;
    }
    return result;
}

int main() {
    // Définir les bornes du problème pour chaque dimension (par exemple, [-10, 10] pour chaque dimension)
    int n_dims = 30;  // Nombre de dimensions

    // Créer des vecteurs de bornes pour chaque dimension
    std::vector<double> lb(n_dims, -10.0);  // Vecteur de bornes inférieures, -10 pour chaque dimension
    std::vector<double> ub(n_dims, 10.0);   // Vecteur de bornes supérieures, 10 pour chaque dimension

    // Créer un vecteur de variables FloatVar pour chaque dimension
    std::vector<FloatVar> bounds;
    for (int i = 0; i < n_dims; ++i) {
        bounds.push_back(FloatVar({lb[i]}, {ub[i]}));  // Créer une variable pour chaque dimension avec ses propres bornes
    }

    // Créer un objet Problem
    std::string minmax = "min";  // Minimisation
    Problem problem(bounds, minmax, objective_function);

    // Initialiser l'optimiseur GWO
    int epoch = 1000;  // Nombre d'époques
    int pop_size = 50;  // Taille de la population
    OriginalGWO gwo(epoch, pop_size);

    // Résoudre le problème avec GWO
    Agent* g_best = gwo.solve(&problem);  // Résultat de la solution optimale

    // Afficher la solution optimale et son fitness
    std::cout << "Solution optimale: ";
    for (double val : g_best->get_solution()) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << "Fitness: " << g_best->get_target().fitness() << std::endl;

    // Afficher les détails de la meilleure solution trouvée pendant l'optimisation
    std::cout << "Solution globale optimale: ";
    for (double val : gwo.get_global_best()->get_solution()) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << "Fitness globale optimale: " << gwo.get_global_best()->get_target().fitness() << std::endl;

    return 0;
}
