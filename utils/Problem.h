#ifndef PROBLEM_H
#define PROBLEM_H
#include <vector>
#include <functional>
#include <memory>  // Nécessaire pour std::shared_ptr

class Problem {
public:
    Problem(const std::vector<std::vector<double>>& lb, const std::vector<std::vector<double>>& ub, const std::string& minmax,
            const std::function<std::vector<double>(const std::vector<double>&)>& obj_func);

    std::vector<double> correct_solution(const std::vector<double>& x) const;
    std::vector<double> generate_solution();

    size_t getNDims() const;
    std::string getMinMax() const;

    // Remplacer la méthode get_target
    double get_fitness(const std::vector<double>& solution) const;  // Retourne directement la fitness

private:
    std::vector<std::vector<double>> lb_;  // Liste des bornes inférieures pour chaque dimension
    std::vector<std::vector<double>> ub_;  // Liste des bornes supérieures pour chaque dimension
    std::vector<double> lb_flat_;  // Vecteur aplati des bornes inférieures
    std::vector<double> ub_flat_;  // Vecteur aplati des bornes supérieures
    std::string minmax_;
    std::function<std::vector<double>(const std::vector<double>&)> obj_func_;
    std::vector<double> obj_weights_;
    size_t n_dims_;

    void set_bounds(const std::vector<std::vector<double>>& lb, const std::vector<std::vector<double>>& ub);
    void set_functions();
};

#endif // PROBLEM_H
