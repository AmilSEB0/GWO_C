//
// Created by amil on 06/02/25.
//

#ifndef GWO_H
#define GWO_H
#include "Optimizer.h"

class OriginalGWO : public Optimizer {
public:
    OriginalGWO(int epoch = 10000, int pop_size = 100);
    void evolve(int epoch);
private:
    int epoch;
    int pop_size;
    bool sort_flag;
};

#endif //GWO_H
