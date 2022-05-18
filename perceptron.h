#include <vector>
#include <array>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <memory>

#ifndef PERCEPTRON_H
#define PERCEPTRON_H
class synapse;

class soma
{
friend class synapse;

public:
    enum activationFunction {GAUSSIAN, RELU, LINEAR, SIGMOID};

private:
    //TODO: pass arguments to the functions such as tilt and offset
    static double gaussianFunction(double x);
    static double reluFunction(double x);
    static double linearFunction(double x);
    static double sigmoidFunction(double x);
    static std::array<double (*)(double), 4> functions;
    activationFunction function = LINEAR;
    
    std::vector<int> deletedInputIdx;
    std::vector<double> input = {0};
    // std::vector<synapse*> outputSynapsies;
    int freeInputIdx = 1;
    double activation(double x);

public:
    soma(/* args */);
    ~soma();

    /**
     * @brief Set the activation function for the soma
     * @param f The activation function (use soma::activationFunction)
     */
    void setActivationFunction(activationFunction f);
    
    int requestInput(synapse*);
    void terminateInput(int idx, synapse* outputSynapse);
    void initialInput(double input); //use this if soma has no input synapse
    void computeOutput();
    double getOutput();
    activationFunction getFunction();

    double output;
};




class synapse
{
private:
    /* data */
    soma* inputSoma;
    soma* outputSoma;
    int signalIdx; //index for input to soma (soma needs to know this idx)
public:
    double weight = 1;
    synapse(/* args */);
    synapse(const synapse&) = delete;
    synapse& operator=(const synapse&) = delete;
    ~synapse();
    void setInput(soma* inputSoma);
    void setOutput(soma* outputSoma);
    void sendSignal();
    void setWeight(double w);
    double getWeight();
};

#endif //PERCEPTRON_H