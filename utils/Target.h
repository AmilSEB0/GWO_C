//
// Created by amil on 10/02/25.
//

#ifndef TARGET_H
#define TARGET_H

#include <vector>

class Target {
public:
    // Constructeur
    Target(const std::vector<double>& objectives = {}, const std::vector<double>& weights = {});

    // Méthode de copie
    Target* copy() const;  // Retourne un objet par valeur

    // Getters
    std::vector<double> objectives() const;
    std::vector<double> weights() const;
    double fitness() const;

    // Setters
    void set_objectives(const std::vector<double>& objectives);
    void set_weights(const std::vector<double>& weights);

    // Calcul de la fitness
    void calculate_fitness(const std::vector<double>& weights);

private:
    std::vector<double> _objectives;
    std::vector<double> _weights;
    double _fitness;
};

#endif // TARGET_H