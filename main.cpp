#include <iostream>
#include <vector>
#include "OriginalGWO.h"
#include "utils/Problem.h"

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
    std::vector<std::vector<double>> lb(n_dims, std::vector<double>(1, -10.0));  // Vecteur de bornes inférieures, -10 pour chaque dimension
    std::vector<std::vector<double>> ub(n_dims, std::vector<double>(1, 10.0));   // Vecteur de bornes supérieures, 10 pour chaque dimension

    // Créer un objet Problem
    std::string minmax = "min";  // Minimisation
    Problem problem(lb, ub, minmax, objective_function);

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
    std::cout << "Fitness: " << g_best->get_target() << std::endl;

    // Afficher les détails de la meilleure solution trouvée pendant l'optimisation
    std::cout << "Solution globale optimale: ";
    for (double val : gwo.get_global_best()->get_solution()) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << "Fitness globale optimale: " << gwo.get_global_best()->get_target() << std::endl;

    return 0;
}

/*#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include "OriginalGWO.h"
#include "utils/Problem.h"

// Fonction objectif : Rosenbrock
std::vector<double> rosenbrock_objective(const std::vector<double>& solution) {
    std::vector<double> result(1, 0.0);
    for (size_t i = 0; i < solution.size() - 1; ++i) {
        result[0] += 100.0 * std::pow(solution[i + 1] - std::pow(solution[i], 2), 2) + std::pow(1 - solution[i], 2);
    }
    return result;
}

// Fonction objectif : Rastrigin
std::vector<double> rastrigin_objective(const std::vector<double>& solution) {
    const double A = 10;
    double sum = 0;
    for (size_t i = 0; i < solution.size(); ++i) {
        sum += std::pow(solution[i], 2) - A * std::cos(2 * M_PI * solution[i]);
    }
    return std::vector<double>{A * solution.size() + sum};
}

// Fonction objectif : Ackley
std::vector<double> ackley_objective(const std::vector<double>& solution) {
    const double a = 20.0, b = 0.2, c = 2 * M_PI;
    double sum1 = 0.0, sum2 = 0.0;
    for (size_t i = 0; i < solution.size(); ++i) {
        sum1 += std::pow(solution[i], 2);
        sum2 += std::cos(c * solution[i]);
    }
    double term1 = -a * std::exp(-b * std::sqrt(sum1 / solution.size()));
    double term2 = -std::exp(sum2 / solution.size());
    return std::vector<double>{term1 + term2 + a + std::exp(1)};
}

// Fonction pour exécuter un benchmark pour une fonction et une dimension
void run_benchmark(int n_dims, const std::vector<std::vector<double>>& lb, const std::vector<std::vector<double>>& ub, const std::string& function_name, int epoch, int pop_size, int n_runs, std::ofstream& execution_file, std::ofstream& stats_file) {
    double total_fitness = 0.0;
    double fitness_squared_sum = 0.0;

    for (int run_idx = 0; run_idx < n_runs; ++run_idx) {
        // Créer un objet Problem avec la fonction objectif appropriée
        Problem problem(lb, ub, "min", (function_name == "rosenbrock") ? rosenbrock_objective :
                                                    (function_name == "rastrigin") ? rastrigin_objective : ackley_objective);

        // Initialiser l'optimiseur GWO
        OriginalGWO gwo(epoch, pop_size);
        Agent* g_best = gwo.solve(&problem);  // Résultat de la solution optimale

        double fitness = g_best->get_target();
        total_fitness += fitness;
        fitness_squared_sum += fitness * fitness;

        // Enregistrer la fitness de cette exécution dans le fichier CSV d'exécution
        execution_file << function_name << "," << n_dims << "," << run_idx + 1 << "," << fitness << std::endl;
    }

    // Calculer la moyenne et l'écart-type pour cette fonction et dimension
    double mean_fitness = total_fitness / n_runs;
    double variance = (fitness_squared_sum / n_runs) - (mean_fitness * mean_fitness);
    double std_fitness = std::sqrt(variance);

    // Enregistrer la moyenne et l'écart-type dans le fichier CSV de statistiques
    stats_file << function_name << "," << n_dims << "," << mean_fitness << "," << std_fitness << std::endl;
}

int main() {
    // Fichier CSV d'exécution
    std::ofstream execution_file("execution_results.csv");
    execution_file << "Function,Dimension,Run,Fitness\n";

    // Fichier CSV de statistiques
    std::ofstream stats_file("mean_std_fitness.csv");
    stats_file << "Function,Dimension,Mean Fitness,Std Fitness\n";

    // Paramètres
    int epoch = 5000;  // Nombre d'époques
    int pop_size = 30;  // Taille de la population
    int n_runs = 10;    // Nombre d'exécutions
    std::vector<int> dims = {30, 50, 100};  // Dimensions des problèmes

    // Exécuter pour chaque fonction et chaque dimension
    for (int dim : dims) {
        // Définir les bornes du problème
        std::vector<std::vector<double>> lb(dim, std::vector<double>(1, -10.0));  // Vecteur de bornes inférieures
        std::vector<std::vector<double>> ub(dim, std::vector<double>(1, 10.0));   // Vecteur de bornes supérieures

        // Effectuer les benchmarks pour chaque fonction
        run_benchmark(dim, lb, ub, "rosenbrock", epoch, pop_size, n_runs, execution_file, stats_file);
        run_benchmark(dim, lb, ub, "rastrigin", epoch, pop_size, n_runs, execution_file, stats_file);
        run_benchmark(dim, lb, ub, "ackley", epoch, pop_size, n_runs, execution_file, stats_file);
    }

    // Fermer les fichiers
    execution_file.close();
    stats_file.close();

    std::cout << "Benchmarking complet. Les résultats ont été enregistrés dans les fichiers CSV." << std::endl;

    return 0;
}*/
