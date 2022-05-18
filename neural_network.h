#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <list>
#include <array>
#include <vector>
#include <string>
#include <tuple>
#include <utility>
#include <set>
#include <iterator>
#include <iostream>
#include <fstream>
#include "perceptron.h"

#define DEFAULT_WEIGHT 0.0
#define DEFAULT_ACTIVATION_FUNCTION soma::activationFunction::RELU

class neural_network
{
public:
    typedef std::vector<std::list<int>> List2D;
    typedef std::vector<std::array<int, 2>> ListOfConnections;
    typedef std::vector<std::tuple<int, int, double>> ListOfConnectionsWithWeights;
    typedef std::vector<int> List;
private:
    std::vector<synapse*> synapsies;
    std::vector<std::pair<int, int>> synapseConnectionIdx;
    std::vector<bool> synapseComputed;
    bool synapseComputed_mask = 0;

    std::vector<std::vector<int>> somaOutputIdx;
    std::vector<std::vector<int>> somaInputIdx;
    std::vector<int> inputSomasIdx;
    std::vector<int> biasedSomaIdx;
    std::vector<soma*> somas;
    std::vector<int> outputSomasIdx;
    ListOfConnectionsWithWeights toBeWiered;
    std::vector<soma::activationFunction> somaActivationFunction;

    std::set<int> compute_(int);
    bool hasBeenBuild;

    void read_soma_data(std::ifstream&, int count);
    void read_synapse_data(std::ifstream&, int count);
    std::vector<std::shared_ptr<double>> synapse_weight_spagheti_fix;
    void wire_();

public:
    std::vector<double*> synapse_weight;

    neural_network(/* args */);
    ~neural_network();
    // void networkMatrix(int units, int hiddenLayer);
    // void randomReservoir(int numberOfNeurons);
    // void rewire(synapse, int, int, double = 1.00);
    void wire(int, int, double = DEFAULT_WEIGHT);
    void wire(ListOfConnections);
    void wire(ListOfConnectionsWithWeights);
    void build();
    void setAllSomaActivation(soma::activationFunction);
    std::vector<soma> getInputSomas();
    std::vector<soma> getOutputSomas();
    std::vector<synapse> getInputSynapse(std::vector<soma>);
    std::vector<synapse> getOuputSynapse(std::vector<soma>);
    std::vector<synapse> getPreviousSynapse(std::vector<synapse>);
    std::vector<synapse> getNextSynapse(std::vector<synapse>);
    // soma& getSoma(int somaIdx);

    void setInputSoma(std::vector<int>);
    void setInput(std::vector<double>);

    void setOutputSoma(std::vector<int>);
    void compute();
    std::vector<double> getOutput();     


    void add(int, soma::activationFunction, bool bias = false);
    void add(int rows, int columns, soma::activationFunction, bool bias = false);

    void save(std::string);
    void load(std::string);

    /*test code*/
    void neural_test();
};

#endif //NEURAL_NETWORK_H