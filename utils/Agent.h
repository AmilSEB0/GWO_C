//
// Created by amil on 06/02/25.
//

#ifndef AGENT_H
#define AGENT_H

class Agent {
    public:
        // Constructeur
        Agent(std::vector<double> solution = {}, Target* target = nullptr, std::unordered_map<std::string, std::string> kwargs = {});

        // Méthode pour obtenir l'ID
        static int increase();

        // Méthode pour créer une copie d'agent
        Agent* copy() const;

        // Attributs de l'agent
        std::vector<double> solution;
        Target* target;
        int id;
        std::unordered_map<std::string, std::string> kwargs;

    private:
        // Méthode pour gérer les arguments supplémentaires (kwargs)
        void set_kwargs(const std::unordered_map<std::string, std::string>& kwargs);

        // Variable statique pour l'ID de l'agent
        static int ID;

};
#endif //AGENT_H
