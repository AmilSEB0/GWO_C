#include <iostream>
#include <vector>
#include <cmath>
#include "utils/Problem.h"
#include "utils/FloatVar.h"
#include "utils/Target.h"

// Fonction objectif : somme des carrés des éléments du vecteur
std::vector<double> objective_function(const std::vector<double>& solution) {
    double sum = 0.0;
    for (double val : solution) {
        sum += val * val;
    }
    return {sum}; // Retourne un vecteur pour gérer plusieurs objectifs
}

int main() {
    // Définir les bornes du problème (30 variables entre -10 et 10)
    FloatVar bounds(std::vector<double>(30, -10.0), std::vector<double>(30, 10.0));

    // Définir le problème
    Problem problem({bounds}, "min", objective_function);

    // Générer une solution
    std::vector<double> solution = problem.generate_solution(true);
    std::cout << "Solution générée : ";
    for (double val : solution) std::cout << val << " ";
    std::cout << std::endl;

    // Corriger la solution
    std::vector<double> corrected_solution;
    problem.correct_solution(solution);
    std::cout << "Solution corrigée : ";
    for (double val : corrected_solution) std::cout << val << " ";
    std::cout << std::endl;

    // Obtenir le Target (objectifs et poids)
    Target target = problem.get_target(solution);
    std::cout << "Objectifs du target : ";
    for (double obj : target.objectives()) std::cout << obj << " ";
    std::cout << std::endl;

    std::cout << "Poids du target : ";
    for (double weight : target.weights()) std::cout << weight << " ";
    std::cout << std::endl;

    return 0;
}
