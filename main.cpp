#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <chrono>
#include "OriginalGWO.h"
#include "utils/Problem.h"

// Fonction objectif : Rosenbrock
std::vector<double> rosenbrock_objective(const std::vector<double>& solution) {
    std::vector<double> result(1, 0.0);  // Initialisation du résultat à 0
    for (size_t i = 0; i < solution.size() - 1; ++i) {
        result[0] += 100.0 * std::pow(solution[i + 1] - std::pow(solution[i], 2), 2) + std::pow(1 - solution[i], 2);
    }
    return result;
}

// Fonction objectif : Rastrigin
std::vector<double> rastrigin_objective(const std::vector<double>& solution) {
    const double A = 10;  // Coefficient de la fonction
    double sum = 0;
    for (size_t i = 0; i < solution.size(); ++i) {
        sum += std::pow(solution[i], 2) - A * std::cos(2 * M_PI * solution[i]);
    }
    return std::vector<double>{A * solution.size() + sum};  // Retourne le résultat de la fonction Rastrigin
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

// Fonction pour exécuter un benchmark pour une fonction et une dimension spécifiques
// Effectue un certain nombre de runs, calcule les statistiques et enregistre les résultats dans des fichiers CSV.
void run_benchmark(int n_dims, const std::vector<std::vector<double>>& lb, const std::vector<std::vector<double>>& ub, const std::string& function_name, int epoch, int pop_size, int n_runs, std::ofstream& execution_file, std::ofstream& stats_file) {
    double total_fitness = 0.0;  // Variable pour la somme des valeurs de fitness
    double fitness_squared_sum = 0.0;  // Variable pour la somme des carrés des valeurs de fitness
    double total_time = 0.0;  // Variable pour le calcul du temps total
    std::vector<double> fitness_values;  // Liste pour stocker les valeurs de fitness
    std::vector<double> time_values;     // Liste pour stocker les temps d'exécution

    // Exécuter les tests pour un nombre défini de runs
    for (int run_idx = 0; run_idx < n_runs; ++run_idx) {
        // Créer un objet Problem avec la fonction objectif appropriée
        Problem problem(lb, ub, "min", (function_name == "rosenbrock") ? rosenbrock_objective :
                                                    (function_name == "rastrigin") ? rastrigin_objective : ackley_objective);

        // Initialiser l'optimiseur GWO
        OriginalGWO gwo(epoch, pop_size);

        // Mesurer le temps d'exécution
        auto start_time = std::chrono::high_resolution_clock::now();  // Temps avant l'exécution

        std::shared_ptr<Agent> g_best = gwo.solve(&problem);  // Résultat de la solution optimale

        auto end_time = std::chrono::high_resolution_clock::now();  // Temps après l'exécution
        std::chrono::duration<double> execution_duration = end_time - start_time;  // Durée d'exécution en secondes

        double fitness = g_best->get_target();  // Récupérer la fitness du meilleur agent
        total_fitness += fitness;  // Ajouter la fitness à la somme totale
        fitness_squared_sum += fitness * fitness;  // Ajouter le carré de la fitness à la somme des carrés
        fitness_values.push_back(fitness);  // Enregistrer la valeur de fitness

        // Ajouter le temps d'exécution pour cette exécution
        total_time += execution_duration.count();  // Additionner le temps d'exécution
        time_values.push_back(execution_duration.count());  // Enregistrer le temps d'exécution

        // Enregistrer la fitness et le temps d'exécution dans le fichier CSV d'exécution
        execution_file << function_name << "," << n_dims << "," << run_idx + 1 << "," << fitness << "," << execution_duration.count() << std::endl;
    }

    // Calculer la moyenne et l'écart-type pour cette fonction et dimension
    double mean_fitness = total_fitness / n_runs;

    // Calcul de la variance et de l'écart-type pour la fitness : Diviser par n-1 pour un échantillon
    double variance_fitness = 0.0;
    for (double fitness : fitness_values) {
        variance_fitness += std::pow(fitness - mean_fitness, 2);
    }
    variance_fitness /= (n_runs - 1);  // Diviser par n-1
    double std_fitness = std::sqrt(variance_fitness);  // Calcul de l'écart-type de la fitness

    // Calcul de la moyenne et de l'écart-type du temps d'exécution
    double mean_time = total_time / n_runs;
    double variance_time = 0.0;
    for (double time : time_values) {
        variance_time += std::pow(time - mean_time, 2);
    }
    variance_time /= (n_runs - 1);  // Diviser par n-1
    double std_time = std::sqrt(variance_time);  // Calcul de l'écart-type du temps d'exécution

    // Enregistrer les résultats dans le fichier CSV de statistiques
    stats_file << function_name << "," << n_dims << "," << mean_fitness << "," << std_fitness << "," << mean_time << "," << std_time << std::endl;
}

int main() {
    // Fichier CSV d'exécution pour enregistrer les résultats de chaque exécution
    std::ofstream execution_file("execution_results.csv");
    execution_file << "Function,Dimension,Run,Fitness,Execution_Time\n";  // Ajout de la colonne pour le temps d'exécution

    // Fichier CSV de statistiques pour enregistrer les moyennes et écart-types
    std::ofstream stats_file("mean_std_fitness.csv");
    stats_file << "Function,Dimension,Mean Fitness,Std Fitness,Mean Time,Std Time\n";  // Ajout des colonnes pour les temps d'exécution

    // Paramètres du test
    int epoch = 5000;  // Nombre d'époques (itérations)
    int pop_size = 30;  // Taille de la population
    int n_runs = 10;    // Nombre d'exécutions pour chaque test
    std::vector<int> dims = {30, 50, 100};  // Dimensions des problèmes à tester

    // Exécuter les benchmarks pour chaque fonction et chaque dimension
    for (int dim : dims) {
        // Définir les bornes du problème
        std::vector<std::vector<double>> lb(dim, std::vector<double>(1, -10.0));  // Bornes inférieures pour chaque dimension
        std::vector<std::vector<double>> ub(dim, std::vector<double>(1, 10.0));   // Bornes supérieures pour chaque dimension

        // Effectuer les benchmarks pour chaque fonction
        run_benchmark(dim, lb, ub, "rosenbrock", epoch, pop_size, n_runs, execution_file, stats_file);
        run_benchmark(dim, lb, ub, "rastrigin", epoch, pop_size, n_runs, execution_file, stats_file);
        run_benchmark(dim, lb, ub, "ackley", epoch, pop_size, n_runs, execution_file, stats_file);
    }

    // Fermer les fichiers après l'exécution des benchmarks
    execution_file.close();
    stats_file.close();

    std::cout << "Benchmarking complet. Les résultats ont été enregistrés dans les fichiers CSV." << std::endl;

    return 0;
}
