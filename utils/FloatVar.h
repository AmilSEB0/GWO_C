//
// Created by amil on 11/02/25.
//

#ifndef FLOATVAR_H
#define FLOATVAR_H

#include <vector>

class FloatVar {
public:
    // Constructeur
    FloatVar(double lb = -10.0, double ub = 10.0);

    // Constructeur surchargé pour les tableaux de bornes
    FloatVar(const std::vector<double>& lb, const std::vector<double>& ub);

    // Accesseurs
    std::vector<double> get_lb() const;
    std::vector<double> get_ub() const;
    int get_n_vars() const;

private:
    std::vector<double> lb, ub; // Bornes inférieure et supérieure
    int n_vars;

    // Méthode pour définir les bornes
    void _set_bounds(const std::vector<double>& lb, const std::vector<double>& ub);
};

#endif // FLOATVAR_H

