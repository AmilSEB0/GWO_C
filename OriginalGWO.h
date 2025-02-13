//
// Created by amil on 12/02/25.
//
#ifndef ORIGINAL_GWO_H
#define ORIGINAL_GWO_H

#include "Optimizer.h"
#include <vector>

class OriginalGWO : public Optimizer {
public:
    OriginalGWO(int epoch = 10000, int pop_size = 100);

    void evolve(int epoch) override;

    ~OriginalGWO() override;

    std::vector<double> generate_random_vector(int size, double min, double max);
private:
    int epoch;
    int pop_size;
    std::mt19937 generator;
    double checkInt(const std::string& name, double value, std::pair<int, int> bound = {1, 100000});
};

#endif // ORIGINAL_GWO_H
