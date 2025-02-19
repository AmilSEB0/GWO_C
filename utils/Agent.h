#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <memory>  // Ajout pour shared_ptr

class Agent {
public:
    // Constructeur avec une solution et une valeur de fitness
    // 'solution' : vecteur des variables de la solution
    // 'fitness' : évaluation de la solution, généralement une valeur numérique
    Agent(const std::vector<double>& solution, double fitness);
    
    // Constructeur par défaut, initialise 'fitness' à 0 et la solution à vide
    Agent();  

    // Méthode de copie
    // Crée et retourne un shared_ptr pointant vers une copie de l'instance actuelle de l'agent
    std::shared_ptr<Agent> copy() const;  

    // Getters
    // Retourne la solution de l'agent (un vecteur de doubles)
    std::vector<double> get_solution() const;

    // Retourne la valeur de fitness de l'agent
    double get_target() const;

    // Setters
    // Permet de définir la solution de l'agent
    void set_solution(const std::vector<double>& solution);

    // Permet de définir la valeur de fitness de l'agent
    void set_target(double fitness);

private:
    std::vector<double> solution;  // Contient la solution de l'agent
    double fitness;  // Évaluation de la fitness
};

#endif // AGENT_H
