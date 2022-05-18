#ifndef CUSTOM_PLAYER_H
#define CUSTOM_PLAYER_H

#include <vector>
#include <functional>
#include "random"
#include "iplayer.h"
#include "neural_network.h"

class custom_player : public iplayer
{
private:
    std::mt19937 generator;
    std::normal_distribution<> distribution;
    double variation = 2;
    double start_weight = 0;
    std::vector<double> input;
    
public:
    neural_network brain;

    custom_player();
    void randomize_brain();
private:
    int  make_decision();
};

#endif // CUSTOM_PLAYER_H
