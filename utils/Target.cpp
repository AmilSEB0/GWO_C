#include "Target.h"

#include <stdexcept>
#include <numeric>

// Constructeur
Target::Target(const std::vector<double>& objectives, const std::vector<double>& weights)
    : _fitness(0.0) {
    set_objectives(objectives);  // Initialise les objectifs
    set_weights(weights);        // Initialise les poids
    calculate_fitness(_weights);         // Calcule la valeur de fitness
}

// Constructeur de copie
Target* Target::copy() const {
    Target* target = new Target(_objectives, _weights);  // Allocation dynamique d'une copie de l'objet actuel
    return target;
}

// Getter pour les objectifs
std::vector<double> Target::objectives() const {
    return _objectives;
}

// Setter pour les objectifs
void Target::set_objectives(const std::vector<double>& objs) {
    if (objs.empty()) {
        throw std::invalid_argument("Les objectifs ne peuvent pas être vides.");
    }
    _objectives = objs;
}

// Getter pour les poids
std::vector<double> Target::weights() const {
    return _weights;
}

// Setter pour les poids
void Target::set_weights(const std::vector<double>& weights) {
    if (weights.empty()) {
        // Si aucun poids n'est fourni, on attribue par défaut un poids de 1.0 à chaque objectif
        _weights = std::vector<double>(_objectives.size(), 1.0);
    } else {
        _weights = weights;
    }

    // Vérifie que la taille des poids correspond bien à celle des objectifs
    if (_weights.size() != _objectives.size()) {
        throw std::invalid_argument("Le nombre de poids doit être égal au nombre d'objectifs.");
    }
}

// Getter pour la valeur de fitness
double Target::fitness() const {
    return _fitness;
}

// Méthode pour calculer la valeur de fitness
void Target::calculate_fitness(const std::vector<double>& weights) {
    if (_weights.size() != _objectives.size()) {
        throw std::invalid_argument("Le nombre de poids doit correspondre au nombre d'objectifs.");
    }

    // Produit scalaire entre les objectifs et les poids
    _fitness = std::inner_product(_weights.begin(), _weights.end(), _objectives.begin(), 0.0);
}
