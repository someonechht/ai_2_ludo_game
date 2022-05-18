#include "perceptron.h"


soma::soma(/* args */)
{
    deletedInputIdx;
}

soma::~soma()
{
}




void soma::setActivationFunction(activationFunction func)
{
    this->function = func;
}

double soma::activation(double x)
{
    return functions[function](x);
}

int soma::requestInput(synapse* outputSynapse)
{
    // outputSynapsies.push_back(outputSynapse);
    // if some idx is available return that, else return new idx;
    if (!deletedInputIdx.empty())
    {
        auto idxPtr = std::min_element(deletedInputIdx.begin(), deletedInputIdx.end());
        deletedInputIdx.erase(idxPtr);
        return *idxPtr;
    }
    else
    {
        input.push_back(0);
        return freeInputIdx++;
    }
    
}

void soma::terminateInput(int idx, synapse* outputSynapse)
{
    // std::remove(outputSynapsies.begin(), outputSynapsies.end(), outputSynapse);
    if (freeInputIdx-1 == idx)
    {
        // erase top indecies that has been deleted
        auto idxPtr = std::max_element(deletedInputIdx.begin(), deletedInputIdx.end());
        while (freeInputIdx-1 == *idxPtr)
        {
            input.pop_back();
            freeInputIdx--;
            deletedInputIdx.erase(idxPtr);
            idxPtr = std::max_element(deletedInputIdx.begin(), deletedInputIdx.end());
        }
    }
    else // if top index was not deleted, set input to zero and remember that index
    {
        input[idx] = 0;
        deletedInputIdx.push_back(idx);        
    }
}

void soma::initialInput(double input_)
{
    // if (input.empty())
    //     input.push_back(input_);
    // else
    input[0] = input_;
}

void soma::computeOutput()
{
    // TODO: optimize
    // HOW: make bool to check if the input has changed 
    output = activation(std::accumulate(input.begin(), input.end(), 0.0));
}

double soma::getOutput()
{
    return output;
}

soma::activationFunction soma::getFunction()
{
    return this->function;
}



/*---------- static stuff begins here ----------*/
double soma::gaussianFunction(double x)
{
    return exp(-pow(x,2));
}
double soma::reluFunction(double x)
{
    return (x <= 0) ? 0 : x;
}
double soma::linearFunction(double x)
{
    return x;
}
double soma::sigmoidFunction(double x)
{
    return tanh(x/100)/2.0+0.5;
}

std::array<double (*)(double), 4> soma::functions = {&gaussianFunction, &reluFunction, &linearFunction, &sigmoidFunction};















/*---------- synapse class starts here ----------*/
synapse::synapse(/* args */)
{
    inputSoma = nullptr;
    outputSoma = nullptr;
    weight = 0;
}

synapse::~synapse()
{
}



void synapse::setInput(soma* inputSoma_)
{
    inputSoma = inputSoma_;
}

void synapse::setOutput(soma* outputSoma_)
{
    if (outputSoma==nullptr)
    {
        outputSoma = outputSoma_;
        signalIdx = outputSoma->requestInput(this);
    }
    else
    {
        outputSoma->terminateInput(signalIdx, this); //terminate existing connection
        outputSoma = outputSoma_;
        signalIdx = outputSoma->requestInput(this);
    }
}

void synapse::sendSignal()
{
    inputSoma->computeOutput();
    outputSoma->input[signalIdx] = inputSoma->output*weight;
}

void synapse::setWeight(double w)
{
    weight = w;
}

double synapse::getWeight()
{
    return weight;
}

