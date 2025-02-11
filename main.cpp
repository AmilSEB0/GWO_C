#include <iostream>
#include <vector>
#include "utils/Agent.h"
#include "utils/Target.h"  // Assurez-vous d'inclure le fichier d'en-tête de la classe Target

int main() {
    try {
        // Création d'une instance de Target avec des objectifs et des poids
        std::vector<double> objectives = {10.0, 20.0, 30.0}; // Objectifs pour le target
        std::vector<double> weights = {0.1, 0.3, 0.6}; // Poids pour le target
        Target target(objectives, weights); // Création d'un objet Target

        // Création d'une solution pour l'agent (par exemple une liste de doubles)
        std::vector<double> solution = {1.0, 2.0, 3.0}; // Solution pour l'agent

        // Création de l'agent avec la solution et le target
        Agent agent(solution, target);

        // Affichage des informations sur l'agent
        std::cout << "Solution de l'Agent : ";
        for (const auto& s : agent.get_solution()) {
            std::cout << s << " ";
        }
        std::cout << std::endl;

        // Affichage des informations sur le target de l'agent
        std::cout << "Objectifs du Target : ";
        for (const auto& obj : agent.get_target().objectives()) {
            std::cout << obj << " ";
        }
        std::cout << std::endl;

        std::cout << "Poids du Target : ";
        for (const auto& w : agent.get_target().weights()) {
            std::cout << w << " ";
        }
        std::cout << std::endl;

        // Copie de l'agent
        Agent copiedAgent = agent.copy();
        std::cout << "Solution de l'Agent Copié : ";
        for (const auto& s : copiedAgent.get_solution()) {
            std::cout << s << " ";
        }
        std::cout << std::endl;

        std::cout << "Objectifs du Target de l'Agent Copié : ";
        for (const auto& obj : copiedAgent.get_target().objectives()) {
            std::cout << obj << " ";
        }
        std::cout << std::endl;

        std::cout << "Poids du Target de l'Agent Copié : ";
        for (const auto& w : copiedAgent.get_target().weights()) {
            std::cout << w << " ";
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}
