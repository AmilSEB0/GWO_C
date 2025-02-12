#ifndef PROBLEM_H
#define PROBLEM_H

#include "FloatVar.h"
#include "Target.h"
#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <cmath>

class Problem {
public:
    Problem(const std::vector<FloatVar>& bounds, const std::string& minmax,
            const std::function<std::vector<double>(const std::vector<double>&)>& obj_func);

    void set_bounds(const std::vector<FloatVar>& bounds);
    void set_seed(int seed);
    std::vector<double> correct_solution(const std::vector<double>& x) const;
    std::vector<double> generate_solution(bool encoded = true);
    Target get_target(const std::vector<double>& solution) const;
    size_t getNDims() const;
    std::string getMinMax() const;

private:
    void set_functions();

    std::vector<FloatVar> bounds_;
    std::vector<double> lb_;
    std::vector<double> ub_;
    std::string minmax_;
    std::function<std::vector<double>(const std::vector<double>&)> obj_func_;
    std::vector<double> obj_weights_;
    size_t n_dims_;
    int seed;
    std::mt19937 generator;
};

#endif // PROBLEM_H
