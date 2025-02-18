#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <memory>  // Ajout pour shared_ptr

class Agent {
public:
    // Constructeur avec solution et fitness
    Agent(const std::vector<double>& solution, double fitness);
    Agent();  // Constructeur par défaut

    // Méthode de copie
    std::shared_ptr<Agent> copy() const;  // Retourne un shared_ptr

    // Getters
    std::vector<double> get_solution() const;
    double get_target() const;

    // Setters
    void set_solution(const std::vector<double>& solution);
    void set_target(double fitness);

private:
    std::vector<double> solution;
    double fitness;  // anciennement target
};

#endif // AGENT_H
