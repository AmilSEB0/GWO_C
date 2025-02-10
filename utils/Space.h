//
// Created by amil on 10/02/25.
//

#ifndef SPACE_H
#define SPACE_H

#include <string>
#include <vector>
#include <random>

// Déclaration de la classe BaseVar
class BaseVar {
public:
    // Constructeur
    BaseVar(const std::string& name = "variable");

    // Getters et Setters pour le seed
    int get_seed() const;
    void set_seed(int value);

    // Générateur de nombres aléatoires
    std::default_random_engine generator;

protected:
    std::string name;
    int n_vars;
    std::vector<double> lb, ub;  // Bornes (inférieure et supérieure)
    int _seed;  // Valeur du seed
};

// Déclaration de la classe FloatVar, qui hérite de BaseVar
class FloatVar : public BaseVar {
public:
    // Constructeur
    FloatVar(double lb = -10.0, double ub = 10.0, const std::string& name = "float");

private:
    // Méthode pour définir les bornes pour FloatVar
    void _set_bounds(double lb, double ub);
};
#endif //SPACE_H
