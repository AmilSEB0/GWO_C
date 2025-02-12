//
// Created by amil on 06/02/25.
//
#include "Optimizer.h"
#include "utils/Agent.h"

Optimizer::Optimizer()
    : epoch(0), pop_size(0), g_best(new Agent()), g_worst(nullptr), problem(nullptr) {
    // Initialisation des vecteurs
    list_epoch_time.clear();
    list_global_best.clear();
    list_current_best.clear();
    list_global_worst.clear();
    list_current_worst.clear();
    list_global_best_fit.clear();
    list_current_best_fit.clear();
    list_diversity.clear();
}

void Optimizer::before_initialization(const std::vector<std::vector<double>>& starting_solutions) {
    if (starting_solutions.empty()) {
        return; // Pas de traitement si aucune solution initiale
    } else if (starting_solutions.size() == static_cast<size_t>(pop_size)) {
        for (const auto& solution : starting_solutions) {
            if (solution.size() == problem->getNDims()) {
                pop.push_back(generate_agent(solution)); // Générer un agent pour chaque solution
            } else {
                throw std::invalid_argument("Invalid starting_solutions dimensions.");
            }
        }
    } else {
        throw std::invalid_argument("Invalid starting_solutions size.");
    }
}

void Optimizer::after_initialization() {
    std::vector<double> list_fits;
    for (auto* agent : pop) {
        list_fits.push_back(agent->get_target().fitness());
    }

    std::vector<size_t> indices(list_fits.size());
    std::iota(indices.begin(), indices.end(), 0);

    if (problem->getMinMax() == "max") {
        std::sort(indices.begin(), indices.end(), [&list_fits](size_t i1, size_t i2) {
            return list_fits[i1] > list_fits[i2];
        });
    } else {
        std::sort(indices.begin(), indices.end(), [&list_fits](size_t i1, size_t i2) {
            return list_fits[i1] < list_fits[i2];
        });
    }

    std::vector<Agent*> sorted_pop;
    for (size_t idx : indices) {
        sorted_pop.push_back(pop[idx]);
    }

    pop = sorted_pop;
    g_best = pop[0];
    g_worst = pop.back();

    list_global_best.push_back(g_best->copy());
    list_current_best.push_back(g_best->copy());
    list_global_worst.push_back(g_worst->copy());
    list_current_worst.push_back(g_worst->copy());
}

void Optimizer::check_problem(Problem* problem) {
    this->problem = problem;  pop.clear();
    g_best = nullptr;
    g_worst = nullptr;
}

Agent* Optimizer::solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions,
                       int seed) {
    // Vérification et initialisation du problème
    check_problem(problem);

    // Appel de la méthode avant initialisation
    before_initialization(starting_solutions);

    // Génération de la population si elle est vide
    if (pop.empty()) {
        pop = generate_population(pop_size);
    }

    // Appel de la méthode après initialisation
    after_initialization();

    // Boucle d'optimisation pour chaque époque
    for (int epoch = 1; epoch <= epoch; ++epoch) {
        // Appel de la méthode évolutive spécifique à la classe fille
        evolve(epoch);

        // Mise à jour de la meilleure solution globale (g_best)
        g_best = update_global_best_agent(pop);

        // Optionnel : mesure du temps d'une époque (cette partie simule le comportement de time.perf_counter())
        auto start_time = std::chrono::high_resolution_clock::now();
        // Calcul du temps écoulé pour l'époque
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        double time_epoch = duration.count();

        // Suivi de l'optimisation pour l'époque courante
        track_optimize_step(pop, epoch, time_epoch);
    }

    // Suivi final du processus d'optimisation
    track_optimize_process();

    // Retourner l'agent globalement meilleur trouvé
    return g_best;
}

Agent* Optimizer::generate_empty_agent(const std::vector<double>& solution) {
    /**
     * Génère un nouvel agent avec une solution.
     *
     * Args:
     *     solution: La solution sous forme de tableau (std::vector<double>).
     *               Si aucune solution n'est fournie, une nouvelle solution sera générée.
     */

    // Si aucune solution n'est fournie, générer une solution par défaut à l'aide du problème
    std::vector<double> agent_solution = solution.empty() ? problem->generate_solution(true) : solution;

    // Créer un nouvel agent avec la solution générée et retourner un pointeur vers cet agent
    return new Agent(agent_solution, problem->get_target(agent_solution));
}

Agent* Optimizer::generate_agent(const std::vector<double>& solution) {
    /**
     * Génère un agent complet avec une solution donnée.
     *
     * Args :
     *     solution : La solution sous forme de vecteur de doubles.
     *                Si aucune solution n'est fournie, une solution par défaut est générée.
     *
     * Retourne :
     *     Un pointeur vers l'agent généré.
     */
    Agent* agent = generate_empty_agent(solution);
    agent->set_target(get_target(agent->get_solution()));  // Assigner la cible à l'agent
    return agent;
}


std::vector<Agent*> Optimizer::generate_population(int pop_size) {
    /**
     * Génère une population d'agents.
     *
     * Args:
     *     pop_size : Le nombre d'agents à générer (solutions).
     *
     * Retourne :
     *     Une liste (std::vector) d'agents.
     */

    // Si pop_size n'est pas spécifié, on utilise pop_size de la classe Optimizer.
    if (pop_size == 0) {
        pop_size = this->pop_size;
    }

    // Créer un vecteur d'agents
    std::vector<Agent*> population;

    // Générer pop_size agents et les ajouter à la population
    for (int i = 0; i < pop_size; ++i) {
        population.push_back(generate_agent());  // Assurez-vous que generate_agent() retourne un pointeur
    }

    return population;
}

std::vector<double> Optimizer::correct_solution(const std::vector<double>& solution) {
    /**
     * Cette fonction est basée sur la stratégie de l'optimiseur et la condition spécifique au problème.
     * NE PAS redéfinir cette fonction.
     *
     * Args :
     *     solution : La position sous forme de vecteur de doubles.
     *
     * Retourne :
     *     La solution corrigée qui peut être utilisée pour calculer la cible.
     */
    return problem->correct_solution(solution);
}

// Implémentation de la méthode generate_solution_optimizer
std::vector<double> Optimizer::generate_solution(bool encoded) {
    std::vector<double> solution;

    // Vérifier si un problème a été défini
    if (problem == nullptr) {
        throw std::invalid_argument("Problem must be defined before generating solutions.");
    }

    // Appel de la méthode generate_solution() de la classe Problem
    solution = problem->generate_solution(encoded);

    // Optionnellement, ajouter un traitement spécifique pour l'optimisation ici (par exemple, mutation, normalisation, etc.)
    if (!encoded) {
        // Par exemple, ici tu pourrais appliquer une transformation particulière si non encodé
        solution = problem->correct_solution(solution);  // Correction de la solution selon les bornes
    }

    return solution;
}

Target Optimizer::get_target(const std::vector<double>& solution, bool counted) {
    /**
     * Obtenez la valeur cible (target).
     *
     * Args:
     *     solution: La solution réelle (valeurs en virgule flottante).
     *     counted: Indique si le nombre d'évaluations de la fonction est en train d'augmenter ou non.
     *
     * Retourne :
     *     La valeur cible (Target).
     */
    return problem->get_target(solution);  // Appel à la méthode get_target de la classe Problem.
}

Agent* Optimizer::get_better_agent(Agent* agent_x, Agent* agent_y, const std::string& minmax , bool reverse) {
    /**
     * Retourne l'agent ayant la meilleure performance en fonction de la "fitness".
     *
     * Args:
     *     agent_x: Premier agent
     *     agent_y: Deuxième agent
     *     minmax: Type de problème, "min" ou "max"
     *     reverse: Inverser la comparaison de fitness
     *
     * Retourne :
     *     L'agent ayant la meilleure performance (fitness).
     */

    // Dictionnaire minmax pour déterminer l'indice
    std::map<std::string, int> minmax_dict = {{"min", 0}, {"max", 1}};
    int idx = minmax_dict[minmax];  // 0 pour minimiser, 1 pour maximiser

    if (reverse) {
        idx = 1 - idx;  // Inverser si nécessaire
    }

    // Comparer les agents en fonction de leur "fitness"
    if (idx == 0) {  // Min (Chercher le meilleur "fitness" minimum)
        return (agent_x->get_target().fitness() < agent_y->get_target().fitness()) ? agent_x : agent_y;
    } else {  // Max (Chercher le meilleur "fitness" maximum)
        return (agent_x->get_target().fitness() < agent_y->get_target().fitness()) ? agent_y : agent_x;
    }
}

Agent* Optimizer::update_global_best_agent(std::vector<Agent*>& pop, bool save) {
    // Tri de la population en fonction de la fitness
    std::vector<double> list_fits;
    for (auto& agent : pop) {
        list_fits.push_back(agent->get_target().fitness());
    }

    // Tri des indices selon la fitness (croissante par défaut)
    std::vector<int> indices(pop.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&list_fits](int a, int b) {
        return list_fits[a] < list_fits[b];
    });

    // Si on veut maximiser la fitness, on inverse les indices
    if (this->problem->getMinMax() == "max") {
        std::reverse(indices.begin(), indices.end());
    }

    // Tri de la population
    std::vector<Agent*> sorted_pop;
    for (int idx : indices) {
        sorted_pop.push_back(pop[idx]);
    }

    Agent* c_best = sorted_pop[0];
    Agent* c_worst = sorted_pop.back();

    if (save) {
        // Sauvegarde du meilleur actuel
        this->list_current_best.push_back(c_best);
        Agent* better = get_better_agent(c_best, this->list_global_best.back(), this->problem->getMinMax());
        this->list_global_best.push_back(better);

        // Sauvegarde du pire actuel
        this->list_current_worst.push_back(c_worst);
        Agent* worse = get_better_agent(c_worst, this->list_global_worst.back(), this->problem->getMinMax(), true);
        this->list_global_worst.push_back(worse);

        return better;
    } else {
        // Gestion du meilleur actuel
        Agent* local_better = get_better_agent(c_best, this->list_current_best.back(), this->problem->getMinMax());
        this->list_current_best.back() = local_better;

        // Gestion du meilleur global
        Agent* global_better = get_better_agent(c_best, this->list_global_best.back(), this->problem->getMinMax());
        this->list_global_best.back() = global_better;

        // Gestion du pire actuel
        Agent* local_worst = get_better_agent(c_worst, this->list_current_worst.back(), this->problem->getMinMax(), true);
        this->list_current_worst.back() = local_worst;

        // Gestion du pire global
        Agent* global_worst = get_better_agent(c_worst, this->list_global_worst.back(), this->problem->getMinMax(), true);
        this->list_global_worst.back() = global_worst;

        return global_better;
    }
}

void Optimizer::track_optimize_step(std::vector<Agent*>& pop, int epoch, double runtime) {
    // Ajouter les données historiques
    this->list_epoch_time.push_back(runtime);
    this->list_global_best_fit.push_back(this->list_global_best.back()->get_target().fitness());
    this->list_current_best_fit.push_back(this->list_current_best.back());

    // Calcul de la diversité (exploration et exploitation)
    std::vector<std::vector<double>> pos_matrix;
    for (auto& agent : pop) {
        pos_matrix.push_back(agent->get_solution());
    }

    // Calcul de la médiane sur chaque dimension de la solution
    size_t num_agents = pos_matrix.size();
    size_t num_dimensions = pos_matrix[0].size();
    std::vector<double> median_pos(num_dimensions, 0.0);

    for (size_t dim = 0; dim < num_dimensions; ++dim) {
        std::vector<double> dimension_values(num_agents);
        for (size_t i = 0; i < num_agents; ++i) {
            dimension_values[i] = pos_matrix[i][dim];
        }
        std::sort(dimension_values.begin(), dimension_values.end());
        median_pos[dim] = dimension_values[num_agents / 2];  // Médiane
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

    // Calcul de la diversité moyenne
    double avg_div = std::accumulate(div.begin(), div.end(), 0.0) / num_dimensions;
    this->list_diversity.push_back(avg_div);

    // Affichage de l'état de l'itération
    std::cout << ">>> Epoch: " << epoch
              << ", Current best: " << this->list_current_best.back()->get_target().fitness()
              << ", Global best: " << this->list_global_best.back()->get_target().fitness()
              << ", Runtime: " << runtime << " seconds" << std::endl;
}

void Optimizer::track_optimize_process() {
    /**
     * Sauvegarde des données historiques après la fin du processus d'entraînement
     */

    // Mettre à jour l'epoch
    this->epoch = this->list_diversity.size();

    // Calculer la diversité maximale
    double div_max = *std::max_element(this->list_diversity.begin(), this->list_diversity.end());

    // Calculer la liste d'exploration
    this->list_exploration.clear();
    for (double div : this->list_diversity) {
        this->list_exploration.push_back(100 * (div / div_max));
    }

    // Calculer la liste d'exploitation
    this->list_exploitation.clear();
    for (double exploration : this->list_exploration) {
        this->list_exploitation.push_back(100 - exploration);
    }

    // Supprimer le premier élément de chaque liste historique (équivalent à "self.list_global_best = self.list_global_best[1:]")
    this->list_global_best.erase(this->list_global_best.begin());
    this->list_current_best.erase(this->list_current_best.begin());
    this->list_global_worst.erase(this->list_global_worst.begin());
    this->list_current_worst.erase(this->list_current_worst.begin());
}
