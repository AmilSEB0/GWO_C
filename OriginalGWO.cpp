#include "OriginalGWO.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

// Constructeur de la classe OriginalGWO
OriginalGWO::OriginalGWO(int epoch, int pop_size) : g_best(nullptr), problem(nullptr) {
    // Vérification et assignation des valeurs pour epoch et pop_size
    this->epoch = checkInt("epoch", epoch, {1, 100000});
    this->pop_size = checkInt("pop_size", pop_size, {5, 10000});

    // Initialisation des vecteurs
    list_epoch_time.clear();
    list_global_best.clear();
    list_current_best.clear();
    list_global_best_fit.clear();
    list_current_best_fit.clear();
    list_diversity.clear();
}

// Destructeur de la classe
OriginalGWO::~OriginalGWO() {
    // Nettoyage des populations et listes utilisées
    pop.clear();
    list_global_best.clear();
    list_current_best.clear();
    list_epoch_time.clear();
    list_global_best_fit.clear();
    list_current_best_fit.clear();
    list_diversity.clear();
}

// Fonction pour vérifier les valeurs entières dans les bornes spécifiées
double OriginalGWO::checkInt(const std::string& name, double value, std::pair<int, int> bound) {
    // Si la valeur est hors des bornes, une exception est levée
    if (value < bound.first || value > bound.second) {
        throw std::invalid_argument(name + " must be in range [" + std::to_string(bound.first) + ", " + std::to_string(bound.second) + "].");
    }
    return static_cast<int>(value); // Conversion en entier et retour
}

// Vérification et préparation du problème avant de résoudre
void OriginalGWO::check_problem(Problem* problem) {
    if (!problem) {
        throw std::invalid_argument("Problem instance is null.");
    }
    this->problem = problem; // Assignation de l'instance de problème
    pop.clear();
    g_best = nullptr;
}

// Fonction principale de résolution de l'optimiseur GWO
std::shared_ptr<Agent> OriginalGWO::solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions, int seed) {
    check_problem(problem); // Vérification du problème avant résolution

    // Générer la population initiale
    pop = generate_population(pop_size); // Générer la population initiale

    // Exécution de l'optimisation sur plusieurs epochs
    for (int e = 1; e <= epoch; ++e) {
        evolve(e); // Met à jour la population selon l'algorithme GWO
        g_best = update_global_best_agent(pop); // Mise à jour de l'agent globalement meilleur

        auto start_time = std::chrono::high_resolution_clock::now();
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        double time_epoch = duration.count();

        track_optimize_step(pop, e, time_epoch); // Suivi de l'optimisation pour cette epoch
    }

    return g_best; // Retour de l'agent globalement meilleur
}

// Fonction pour générer un agent vide
std::shared_ptr<Agent> OriginalGWO::generate_empty_agent(const std::vector<double>& solution) {
    std::vector<double> agent_solution = solution.empty() ? problem->generate_solution() : solution;
    return std::make_shared<Agent>(agent_solution, problem->get_fitness(agent_solution)); // Création d'un agent
}

// Fonction pour générer un agent avec une solution donnée
std::shared_ptr<Agent> OriginalGWO::generate_agent(const std::vector<double>& solution) {
    auto agent = generate_empty_agent(solution);
    agent->set_target(get_target(agent->get_solution()));  // Assignation du fitness à l'agent
    return agent;
}

// Fonction pour obtenir l'agent avec le meilleur fitness global
std::shared_ptr<Agent> OriginalGWO::get_global_best() const {
    if ( this->g_best == nullptr) {
        throw std::runtime_error("No global best found. The optimizer has not run yet.");
    }
    return this->g_best; // Le dernier élément est le meilleur
}

// Fonction pour obtenir l'agent "meilleur" entre deux
std::shared_ptr<Agent> OriginalGWO::get_better_agent(
    std::shared_ptr<Agent> agent_x,
    std::shared_ptr<Agent> agent_y,
    const std::string& minmax,
    bool reverse) {
    /**
     * Cette fonction retourne l'agent avec la meilleure performance en fonction de la "fitness".
     */
    if (agent_y == nullptr) {
        return agent_x;
    }
    std::map<std::string, int> minmax_dict = {{"min", 0}, {"max", 1}};
    int idx = minmax_dict[minmax];  // 0 pour minimiser, 1 pour maximiser

    if (reverse) {
        idx = 1 - idx;  // Inversion de la comparaison si nécessaire
    }

    // Comparer les agents en fonction de leur "fitness"
    if (idx == 0) {  // Min (Chercher le meilleur "fitness" minimum)
        return (agent_x->get_target() < agent_y->get_target()) ? agent_x : agent_y;
    } else {  // Max (Chercher le meilleur "fitness" maximum)
        return (agent_x->get_target() < agent_y->get_target()) ? agent_y : agent_x;
    }
}

// Générer la population de manière aléatoire
std::vector<std::shared_ptr<Agent>> OriginalGWO::generate_population(int pop_size) {
    if (pop_size == 0) {
        pop_size = this->pop_size; // Utilisation de pop_size si pop_size est 0
    }

    std::vector<std::shared_ptr<Agent>> population;
    for (int i = 0; i < pop_size; ++i) {
        population.push_back(generate_agent()); // Génération d'un agent
    }

    return population; // Retourner la population
}

// Correction de la solution en fonction du problème
std::vector<double> OriginalGWO::correct_solution(const std::vector<double>& solution) {
    return problem->correct_solution(solution); // Correction de la solution
}

// Fonction pour obtenir la cible (fitness) d'une solution
double OriginalGWO::get_target(const std::vector<double>& solution, bool counted) {
    return problem->get_fitness(solution); // Appel à la méthode de fitness du problème
}

// Mise à jour de l'agent globalement meilleur
std::shared_ptr<Agent> OriginalGWO::update_global_best_agent(std::vector<std::shared_ptr<Agent>>& pop, bool save) {
    std::vector<double> list_fits;
    // Collecte des fitness des agents
    for (auto& agent : pop) {
        list_fits.push_back(agent->get_target());
    }

    std::vector<int> indices(pop.size());
    std::iota(indices.begin(), indices.end(), 0); // Initialisation des indices

    // Tri des indices par fitness
    std::sort(indices.begin(), indices.end(), [&list_fits](int a, int b) {
        return list_fits[a] < list_fits[b];
    });

    // Inversion pour maximisation
    if (this->problem->getMinMax() == "max") {
        std::reverse(indices.begin(), indices.end());
    }

    std::vector<std::shared_ptr<Agent>> sorted_pop;
    for (int idx : indices) {
        sorted_pop.push_back(pop[idx]);
    }

    auto c_best = sorted_pop[0];  // Agent avec le meilleur fitness

    // Sauvegarde des meilleurs et pires agents
    this->list_current_best.push_back(c_best);
    auto better = get_better_agent(c_best, g_best, this->problem->getMinMax());
    this->list_global_best.push_back(better);

    return better;  // Retourner le meilleur agent
}

// Suivi de l'optimisation pour chaque epoch
void OriginalGWO::track_optimize_step(std::vector<std::shared_ptr<Agent>>& pop, int epoch, double runtime) {
    // Ajouter le temps et les performances de cette epoch
    this->list_epoch_time.push_back(runtime);
    this->list_global_best_fit.push_back(this->list_global_best.back()->get_target());
    this->list_current_best_fit.push_back(this->list_current_best.back());

    // Calcul de la diversité dans la population
    std::vector<std::vector<double>> pos_matrix;
    for (auto& agent : pop) {
        pos_matrix.push_back(agent->get_solution());
    }

    size_t num_agents = pos_matrix.size();
    size_t num_dimensions = pos_matrix[0].size();
    std::vector<double> median_pos(num_dimensions, 0.0);

    for (size_t dim = 0; dim < num_dimensions; ++dim) {
        std::vector<double> dimension_values(num_agents);
        for (size_t i = 0; i < num_agents; ++i) {
            dimension_values[i] = pos_matrix[i][dim];
        }
        std::sort(dimension_values.begin(), dimension_values.end());
        median_pos[dim] = dimension_values[num_agents / 2];
    }

    // Calcul de la diversité
    std::vector<double> div(num_dimensions, 0.0);
    for (size_t dim = 0; dim < num_dimensions; ++dim) {
        double sum_abs = 0.0;
        for (size_t i = 0; i < num_agents; ++i) {
            sum_abs += std::abs(median_pos[dim] - pos_matrix[i][dim]);
        }
        div[dim] = sum_abs / num_agents;
    }

    double avg_div = std::accumulate(div.begin(), div.end(), 0.0) / num_dimensions;
    this->list_diversity.push_back(avg_div);

    std::cout << ">>> Epoch: " << epoch
              << ", Current best: " << this->list_current_best.back()->get_target()
              << ", Global best: " << this->list_global_best.back()->get_target()
              << ", Runtime: " << runtime << " seconds" << std::endl;
}

// Fonction pour générer un vecteur de nombres aléatoires
std::vector<double> OriginalGWO::generate_random_vector(int size, double min, double max) {
    std::vector<double> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = min + (max - min) * ((double) rand() / RAND_MAX); // Génère des valeurs entre min et max
    }
    return vec;
}

void OriginalGWO::evolve(int epoch) {
    // Linearly decreased from 2 to 0
    double a = 2.0 - 2.0 * epoch / this->epoch;  // Identique au code Python

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

    // Recréer la population triée avec des shared_ptr
    std::vector<std::shared_ptr<Agent>> sorted_pop;
    for (auto idx : indices) {
        sorted_pop.push_back(pop[idx]);
    }
    pop = sorted_pop;

    // Récupérer les 3 meilleurs agents
    std::vector<std::shared_ptr<Agent>> list_best(pop.begin(), pop.begin() + 3);

    // Initialisation de la nouvelle population
    std::vector<std::shared_ptr<Agent>> pop_new;
    // Générer des nouveaux agents pour la population
    for (int idx = 0; idx < pop_size; ++idx) {
        // Générer des vecteurs aléatoires pour A1, A2, A3, C1, C2, C3
        std::vector<double> A1 = generate_random_vector(problem->getNDims(), -1.0, 1.0);
        for (auto& val : A1) {
            val *= a;  // Multiplication élément par élément
        }

        std::vector<double> A2 = generate_random_vector(problem->getNDims(), -1.0, 1.0);
        for (auto& val : A2) {
            val *= a;
        }

        std::vector<double> A3 = generate_random_vector(problem->getNDims(), -1.0, 1.0);
        for (auto& val : A3) {
            val *= a;
        }

        std::vector<double> C1 = generate_random_vector(problem->getNDims(), 0.0, 2.0);
        std::vector<double> C2 = generate_random_vector(problem->getNDims(), 0.0, 2.0);
        std::vector<double> C3 = generate_random_vector(problem->getNDims(), 0.0, 2.0);

        // Calcul de la nouvelle position des agents
        std::vector<double> X1 = list_best[0]->get_solution(); // Alpha
        std::vector<double> X2 = list_best[1]->get_solution(); // Beta
        std::vector<double> X3 = list_best[2]->get_solution(); // Delta

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
        auto agent = generate_empty_agent(pos_new);
        pop_new.push_back(agent);

        // Mise à jour de la cible de l'agent
        agent->set_target(get_target(pos_new));

        // Mise à jour de la population avec l'agent amélioré
        pop[idx] = get_better_agent(agent, pop[idx], problem->getMinMax());
    }

    // Mise à jour de la population
    pop = pop_new;
}
