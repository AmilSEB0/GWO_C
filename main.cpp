#include <iostream>
#include "utils/Target.h"

int main() {
    std::vector<double> objectives = {1.0, 2.0, 3.0};
    std::vector<double> weights = {0.5, 0.3, 0.2};

    Target t(objectives, weights);

    std::cout << "Objectives: ";
    for (double obj : t.objectives()) {
        std::cout << obj << " ";
    }
    std::cout << "\n";

    std::cout << "Weights: ";
    for (double w : t.weights()) {
        std::cout << w << " ";
    }
    std::cout << "\n";

    std::cout << "Fitness: " << t.fitness() << std::endl;

    return 0;
}
