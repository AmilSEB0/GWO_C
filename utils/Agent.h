//
// Created by amil on 06/02/25.
//

#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include "Target.h"

class Agent {
public:
    Agent(const std::vector<double>& solution, const Target& target);

    // Méthode de copie
    Agent copy() const;

    // Accesseurs (Getters)
    std::vector<double> get_solution() const;
    Target get_target() const;

    // Mutateurs (Setters)
    void set_solution(const std::vector<double>& solution);
    void set_target(const Target& target);

private:
    std::vector<double> solution;
    Target target;
};

#endif // AGENT_H
