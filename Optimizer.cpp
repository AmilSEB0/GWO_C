#include "Optimizer.h"
#include "utils/Agent.h"

Optimizer::Optimizer(int epoch, int pop_size)
    : epoch(epoch), pop_size(pop_size), g_best(nullptr), g_worst(nullptr), problem(nullptr) {
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

Optimizer::~Optimizer() {
    // Plus besoin de gérer explicitement la mémoire avec shared_ptr, donc on peut supprimer cette section
    pop.clear();
    list_global_best.clear();
    list_current_best.clear();
    list_global_worst.clear();
    list_current_worst.clear();
    list_epoch_time.clear();
    list_global_best_fit.clear();
    list_current_best_fit.clear();
    list_diversity.clear();
}

void Optimizer::before_initialization(const std::vector<std::vector<double>>& starting_solutions) {
    if (starting_solutions.empty()) {
        return;
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

void Optimizer::check_problem(Problem* problem) {
    if (!problem) {
        throw std::invalid_argument("Problem instance is null.");
    }
    this->problem = problem;
    pop.clear();
    g_best = nullptr;
    g_worst = nullptr;
}

std::shared_ptr<Agent> Optimizer::solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions, int seed) {
    check_problem(problem);

    before_initialization(starting_solutions);

    if (pop.empty()) {
        if (pop_size <= 0) {
            throw std::runtime_error("Population size must be greater than zero.");
        }
        pop = generate_population(pop_size);  // Si pop_size est 0 ici, tu devrais avoir une erreur.
    }

    after_initialization();

    for (int e = 1; e <= epoch; ++e) {
        evolve(e);
        g_best = update_global_best_agent(pop);

        auto start_time = std::chrono::high_resolution_clock::now();
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        double time_epoch = duration.count();

        track_optimize_step(pop, e, time_epoch);
    }

    return g_best;
}

void Optimizer::after_initialization() {
    if (pop.empty()) {
        throw std::runtime_error("Population is empty after initialization.");
    }

    std::vector<double> list_fits;
    for (auto& agent : pop) {
        list_fits.push_back(agent->get_target());
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

    std::vector<std::shared_ptr<Agent>> sorted_pop;
    for (size_t idx : indices) {
        sorted_pop.push_back(pop[idx]);
    }

    pop = sorted_pop;
    g_best = pop.front();
    g_worst = pop.back();

    list_global_best.push_back(g_best->copy());
    list_current_best.push_back(g_best->copy());
    list_global_worst.push_back(g_worst->copy());
    list_current_worst.push_back(g_worst->copy());
}

// Modifications suivantes dans `generate_empty_agent` et `generate_agent`
std::shared_ptr<Agent> Optimizer::generate_empty_agent(const std::vector<double>& solution) {
    std::vector<double> agent_solution = solution.empty() ? problem->generate_solution(true) : solution;
    return std::make_shared<Agent>(agent_solution, problem->get_fitness(agent_solution));
}

std::shared_ptr<Agent> Optimizer::generate_agent(const std::vector<double>& solution) {
    auto agent = generate_empty_agent(solution);
    agent->set_target(get_target(agent->get_solution()));  // Assigner la cible à l'agent
    return agent;
}

std::shared_ptr<Agent> Optimizer::get_global_best() const {
    if (this->list_global_best.empty()) {
        throw std::runtime_error("No global best found. The optimizer has not run yet.");
    }

    // Retourne l'agent avec le meilleur fitness global enregistré
    return this->list_global_best.back(); // Le dernier élément de la liste est le meilleur global
}

std::shared_ptr<Agent> Optimizer::get_better_agent(
    std::shared_ptr<Agent> agent_x,
    std::shared_ptr<Agent> agent_y,
    const std::string& minmax,
    bool reverse) {

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
        return (agent_x->get_target() < agent_y->get_target()) ? agent_x : agent_y;
    } else {  // Max (Chercher le meilleur "fitness" maximum)
        return (agent_x->get_target() < agent_y->get_target()) ? agent_y : agent_x;
    }
}

std::vector<std::shared_ptr<Agent>> Optimizer::generate_population(int pop_size) {
    if (pop_size == 0) {
        pop_size = this->pop_size;
    }

    std::vector<std::shared_ptr<Agent>> population;
    for (int i = 0; i < pop_size; ++i) {
        population.push_back(generate_agent());  // Assurez-vous que generate_agent() retourne un shared_ptr
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
double Optimizer::get_target(const std::vector<double>& solution, bool counted) {
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
    return problem->get_fitness(solution);  // Appel à la méthode get_target de la classe Problem.
}
std::shared_ptr<Agent> Optimizer::update_global_best_agent(std::vector<std::shared_ptr<Agent>>& pop, bool save) {
    std::vector<double> list_fits;
    for (auto& agent : pop) {
        list_fits.push_back(agent->get_target());
    }

    std::vector<int> indices(pop.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&list_fits](int a, int b) {
        return list_fits[a] < list_fits[b];
    });

    if (this->problem->getMinMax() == "max") {
        std::reverse(indices.begin(), indices.end());
    }

    std::vector<std::shared_ptr<Agent>> sorted_pop;
    for (int idx : indices) {
        sorted_pop.push_back(pop[idx]);
    }

    auto c_best = sorted_pop[0];
    auto c_worst = sorted_pop.back();

    if (save) {
        this->list_current_best.push_back(c_best);
        auto better = get_better_agent(c_best, this->list_global_best.back(), this->problem->getMinMax());
        this->list_global_best.push_back(better);

        this->list_current_worst.push_back(c_worst);
        auto worse = get_better_agent(c_worst, this->list_global_worst.back(), this->problem->getMinMax(), true);
        this->list_global_worst.push_back(worse);

        return better;
    } else {
        auto local_better = get_better_agent(c_best, this->list_current_best.back(), this->problem->getMinMax());
        this->list_current_best.back() = local_better;

        auto global_better = get_better_agent(c_best, this->list_global_best.back(), this->problem->getMinMax());
        this->list_global_best.back() = global_better;

        auto local_worst = get_better_agent(c_worst, this->list_current_worst.back(), this->problem->getMinMax(), true);
        this->list_current_worst.back() = local_worst;

        auto global_worst = get_better_agent(c_worst, this->list_global_worst.back(), this->problem->getMinMax(), true);
        this->list_global_worst.back() = global_worst;

        return global_better;
    }
}

void Optimizer::track_optimize_step(std::vector<std::shared_ptr<Agent>>& pop, int epoch, double runtime) {
    this->list_epoch_time.push_back(runtime);
    this->list_global_best_fit.push_back(this->list_global_best.back()->get_target());
    this->list_current_best_fit.push_back(this->list_current_best.back());

    // Calcul de la diversité
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

