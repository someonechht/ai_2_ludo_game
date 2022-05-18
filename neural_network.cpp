#include "neural_network.h"
#include "perceptron.h"
#include <iostream>

int findMaxIn2D(neural_network::ListOfConnectionsWithWeights list)
{
    int highestValue=0;
    for (auto pair : list)
    {
        highestValue = (std::get<0>(pair) > highestValue) ? (std::get<0>(pair)) : (highestValue);
        highestValue = (std::get<1>(pair) > highestValue) ? (std::get<1>(pair)) : (highestValue);
    }
    return highestValue;
}
int findMaxIn2D(neural_network::ListOfConnections list)
{
    int highestValue=0;
    for (auto pair : list)
        for (auto value : pair)
            highestValue = (value > highestValue) ? (value) : (highestValue);
    return highestValue;
}

neural_network::neural_network(/* args */)
{
}

neural_network::~neural_network()
{
}

void neural_network::wire_()
{
    synapsies.clear();
    somas.clear();
    
    // synapsies.resize(toBeWiered.size());
    for (int i = 0; i < toBeWiered.size(); i++){synapsies.push_back(new synapse);}
    
    
    int soma_count = findMaxIn2D(toBeWiered)+1;
    for (int i = 0; i < soma_count; i++)
    {
        somas.push_back(new soma);
        somas[i]->setActivationFunction(somaActivationFunction[i]);
    }

    // somas.resize(soma_count);
    // std::fill(somas.begin(), somas.end(), new soma);

    somaOutputIdx.resize(soma_count);
    somaInputIdx.resize(soma_count);
    

    synapseComputed.resize(synapsies.size());
    std::fill(synapseComputed.begin(), synapseComputed.end(), !synapseComputed_mask);

    int i = 0;
    for (auto wire : toBeWiered)
    {
        int input = std::get<0>(wire);
        int output = std::get<1>(wire);
        double weight = std::get<2>(wire);
        synapsies[i]->setInput(somas[input]);
        synapsies[i]->setOutput(somas[output]);
        synapsies[i]->setWeight(weight);
        synapse_weight.push_back(&synapsies[i]->weight);

        synapseConnectionIdx.push_back(std::make_pair(input, output)); //save synapse connection index

        somaOutputIdx.at(input).push_back(i); 
        somaInputIdx.at(output).push_back(i);  

        i++;
    }
    hasBeenBuild = true;
    

}

void neural_network::wire(int input, int output, double w)
{
    // int som_size = somas.size();
    // bool debug = min_soma_req > (som_size);
    // if (debug)
    // {
    //     for (size_t i = 0; i < min_soma_req-som_size; i++)
    //         somas.push_back(std::make_unique<soma>());
        
    //     somaOutputIdx.resize(min_soma_req);
    //     somaInputIdx.resize(min_soma_req);
    // }
    
    // std::shared_ptr<synapse> temp(new synapse);
    // synapsies.push_back(std::move(temp)); 
    // synapsies.back().get()->setInput(somas[input]);
    // synapsies.back().get()->setOutput(somas[output]);
    // synapsies.back().get()->setWeight(w);
    // synapse_weight.push_back(std::addressof(synapsies.back().weight));
    // synapseConnectionIdx.push_back(std::make_pair(input, output)); //save synapse connection index
    // synapseComputed.push_back(!synapseComputed_mask);
    hasBeenBuild = false;

        toBeWiered.push_back({input, output, w});


}

void neural_network::wire(ListOfConnections wireing)
{
    // int somaCount = findMaxIn2D(wireing)+1;
    // if (somaCount-1 > somas.size())
    // {
    //     for (size_t i = 0; i < somaCount-somas.size(); i++)
    //         somas.push_back(std::make_unique<soma>());
    //     somaOutputIdx.resize(somaCount);
    //     somaInputIdx.resize(somaCount);
    // }
    hasBeenBuild = false;
    int somaCount = findMaxIn2D(wireing)+1; //soma {0, 1, 2, 3} is 4 somas aka 3+1
    int size = somaActivationFunction.size(); 
    int i = 0;
    if (somaCount > size) // if number of soma and number of activation function does not match
        somaActivationFunction.resize(somaCount);

    for (auto wire : wireing)
    {
        toBeWiered.push_back({wire[0], wire[1], DEFAULT_WEIGHT});
        // somaActivationFunction[std::get<0>(wire)] = DEFAULT_ACTIVATION_FUNCTION;
        // somaActivationFunction[std::get<1>(wire)] = DEFAULT_ACTIVATION_FUNCTION;
    }
}
void neural_network::wire(ListOfConnectionsWithWeights wireing)
{
    // int somaCount = findMaxIn2D(wireing)+1;
    // if (somaCount-1 > somas.size())
    // {
    //     for (size_t i = 0; i < somaCount-somas.size(); i++)
    //         somas.push_back(std::make_unique<soma>());
    //     somaOutputIdx.resize(somaCount);
    //     somaInputIdx.resize(somaCount);
    // }
    hasBeenBuild = false;
    int somaCount = findMaxIn2D(wireing)+1; //soma {0, 1, 2, 3} is 4 somas aka 3+1
    int size = somaActivationFunction.size(); 
    int i = 0;
    if (somaCount > size) // if number of soma and number of activation function does not match
        somaActivationFunction.resize(somaCount);

    for (auto wire : wireing)
    {
        toBeWiered.push_back({std::get<0>(wire), std::get<1>(wire), std::get<2>(wire)});
        // somaActivationFunction[std::get<0>(wire)] = DEFAULT_ACTIVATION_FUNCTION;
        // somaActivationFunction[std::get<1>(wire)] = DEFAULT_ACTIVATION_FUNCTION;
    }
}

void neural_network::build()
{
    wire_();
}

void neural_network::setAllSomaActivation(soma::activationFunction f)
{
    if (!somas.empty())
        for (auto soma_ : somas)
            soma_->setActivationFunction(f);
    
    std::fill(somaActivationFunction.begin(), somaActivationFunction.end(), f);
}


// soma& neural_network::getSoma(int somaIdx)
// {
//     return somas[somaIdx].get();
// }

void neural_network::setInputSoma(std::vector<int> vec)
{
    inputSomasIdx = vec;
}

void neural_network::setInput(std::vector<double> vec)
{
    for (size_t i = 0; i < vec.size(); i++)
    {
        somas[inputSomasIdx[i]]->initialInput(vec[i]);
    }
}

void neural_network::setOutputSoma(std::vector<int> vec)
{
    outputSomasIdx = vec;
}

void neural_network::compute()
{
    if (!hasBeenBuild)
    {
        wire_(); //hasBeenBuild = true inside function
    }
    
    std::set<int> somaToCompute;
    somaToCompute.insert(inputSomasIdx.begin(), inputSomasIdx.end());
    std::set<int> nextSomaToCompute;

    //calculate bias terms
    for (auto somaIdx : biasedSomaIdx)
    {
        if (!somaOutputIdx[somaIdx].empty())
        {
            for (auto synIdx : somaOutputIdx[somaIdx])
            {
                synapsies[synIdx]->sendSignal();
            }
        }
        
    }

    // while there are still output to compute
    while (!somaToCompute.empty())
    {
        // go through all soma
        for (auto somaIdx : inputSomasIdx)
        {
            if (!somaOutputIdx[somaIdx].empty()) //if there are an output synapse for the soma
                //go through all synapse for a single soma
                for (auto synIdx : somaOutputIdx[somaIdx])
                {
                    if (synapseComputed[synIdx] ^ synapseComputed_mask)
                    {
                        synapseComputed[synIdx] = !synapseComputed[synIdx];
                        //append somas that the synapse is connected
                        nextSomaToCompute.insert(synapseConnectionIdx[synIdx].second); 
                        synapsies[synIdx]->sendSignal(); //this calls the somas `computeOutput()` 
                    }
                    
                }
            else
                outputSomasIdx.push_back(somaIdx);
        }
        somaToCompute = nextSomaToCompute;
        nextSomaToCompute.clear();
    }
    synapseComputed_mask = !synapseComputed_mask;
}



std::vector<double> neural_network::getOutput()
{
    std::vector<double> output;
    for (auto soma_idx : outputSomasIdx)
    {
        somas[soma_idx]->computeOutput(); // neural_network::compute() only computes synapsies
        output.push_back(somas[soma_idx]->getOutput());
    }
    return output;
}

// wire matrix of neurons
void neural_network::add(int rows, int columns, soma::activationFunction f, bool bias) //bias is not implemented and does nothing
{
    int init = somaActivationFunction.size();
    somaActivationFunction.resize(rows*columns+init);
    // synapsies.reserve(synapsies.size() + rows*rows*(columns-1));
    // int init = somas.size();
    // List newBiasSomaIdx;
    // if (bias)
    // {    
    //     somas.resize(somas.size()+columns);
    //     for (int i = 0; i < columns; i++)
    //     {
    //         biasedSomaIdx.push_back(init+i);
    //         somas[init+i].initialInput(1);
    //     }
    //     newBiasSomaIdx = biasedSomaIdx;    
    //     init = somas.size();
    // }
    
    //find indexies to wire
    std::vector<std::array<int, 2>> connection;
    for (int col = init, i = 0; col < init+columns-1; col++, i++)
    {
        for (int row_i = init+(col*rows); row_i < init+rows+(col*rows); row_i++)
        {
            for (int row_i1 = init+rows+(col*rows); row_i1 < init+(rows*2)+(col*rows); row_i1++)
            {
                toBeWiered.push_back({row_i, row_i1, DEFAULT_WEIGHT});
                // std::array<int, 2> con = {row_i, row_i1};
                // connection.push_back(con);
            }
            // if (bias)
            //     connection.push_back({newBiasSomaIdx[i], row_i});
        }
    }    
    //wire them
    // wire(connection);
    // std::vector<int> somaIdxInNet;
    for (size_t somaIdx = init; somaIdx < rows*columns+init; somaIdx++)
    {
        // somaIdxInNet.push_back(somaIdx);
        // somas.at(somaIdx)->setActivationFunction(f);
        somaActivationFunction.at(somaIdx) = f;
    }

    /* wire the input of newly added network */
    if (inputSomasIdx.empty() && init == 0) // if no input and no previous somas exist
    {
        // pushback first column of somas as input
        for (int row = 0; row < rows; row++)
            inputSomasIdx.push_back(row);
        
    }
    else if (inputSomasIdx.empty() && !somaActivationFunction.empty()) // if soma extist but no input soma defined
    {
        // wire all previous soma to new netowrk
        std::vector<std::array<int, 2>> connections;
        for (int row = 0; row < rows; row++)
        {
            for (int somaIdx = 0; somaIdx < somas.size(); somaIdx++)
            {
                // std::array<int, 2> con = {somaIdx, row+init};
                // connections.push_back(con);
                toBeWiered.push_back({somaIdx, row+init, DEFAULT_WEIGHT});
            }   
        } 
        // wire(connections);
    }
    else if (!inputSomasIdx.empty()) // if input soma exist (not empty)
    {
        /* wire input into new layer */
        std::vector<std::array<int, 2>> connection;
        for (int row = 0; row < rows; row++)
        {
            for (auto somaIdx : inputSomasIdx)
            {
                // std::array<int, 2> con = {somaIdx, row+init};
                // connection.push_back(con);
                toBeWiered.push_back({somaIdx, row+init, DEFAULT_WEIGHT});

            }   
        } 
        // wire(connection);
    }
    else
        throw ("unknown state when wiring input in neural_network::add(int, int, activationFunction, bool)");

    //wire new network as output and overwrites the old (if there are any)
    outputSomasIdx.clear();
    for (int row = rows*(columns-1); row < rows*(columns-1)+rows; row++)
        outputSomasIdx.push_back(row+init);
}

//bias is not implemented
void neural_network::add(int rows, soma::activationFunction f, bool bias)
{
    int init = somaActivationFunction.size();
    somaActivationFunction.resize(rows+init);
    // synapsies.reserve(synapsies.size() + rows*rows);
    //find indexies to wire
    // std::vector<std::array<int, 2>> connection;

    for (int row_i = init; row_i < init+rows; row_i++)
    {
        for (int row_i1 = init+rows; row_i1 < init+(rows*2); row_i1++)
        {
            
            // std::array<int, 2> con = {row_i, row_i1};
            // connection.push_back(con);
            toBeWiered.push_back({row_i, row_i1, DEFAULT_WEIGHT});
        }
    }
    //wire them
    // wire(connection);
    // std::vector<int> somaIdxInNet;
    for (size_t somaIdx = init; somaIdx < rows+init; somaIdx++)
    {
        // somaIdxInNet.push_back(somaIdx);
        // somas.at(somaIdx)->setActivationFunction(f);
        somaActivationFunction.at(somaIdx) = f;
    }

    /* wire the input of newly added network if it exist */
    if (inputSomasIdx.empty() && init == 0) // if no input and no previous somas exist
    {
        // pushback first column of somas as input
        for (int row = 0; row < rows; row++)
            inputSomasIdx.push_back(row);
        
    }
    else if (inputSomasIdx.empty() && !somaActivationFunction.empty()) // if soma extist but no input soma defined
    {
        // wire all previous soma to new netowrk
        // std::vector<std::array<int, 2>> connections;
        for (int row = 0; row < rows; row++)
        {
            for (int somaIdx = 0; somaIdx < somas.size(); somaIdx++)
            {
                // std::array<int, 2> con = {somaIdx, row+init};
                // connections.push_back(con);
                toBeWiered.push_back({somaIdx, row+init, DEFAULT_WEIGHT});
            }   
        } 
        // wire(connections);
    }
    else if (!inputSomasIdx.empty()) // if input soma is defined
    {

        // std::vector<std::array<int, 2>> connection;
        for (int row = 0; row < rows; row++)
        {
            for (auto somaIdx : inputSomasIdx)
            {
                // std::array<int, 2> con = {somaIdx, row+init};
                // connection.push_back(con);
                toBeWiered.push_back({somaIdx, row+init, DEFAULT_WEIGHT});
            }   
        } 
        // wire(connection);
    }
    else
        throw ("unknown state when wiring input in neural_network::add(int, int, activationFunction)");
    

    //wire new network as output and overwrites the old (if there are any)
    outputSomasIdx.clear();
    for (int row = 0; row < rows; row++)
        outputSomasIdx.push_back(row+init);

    // synapse_weight.resize(synapsies.size());
    // int i = 0;
    // for (auto& syn: synapsies)
    //     synapse_weight[i++].reset(&syn->weight);
    
}


//only save and load synapsies input output
void neural_network::load(std::string filename)
{
    // reset everything if someone choose to build a network before they load one
    synapsies.clear();
    synapseConnectionIdx.clear();
    somaOutputIdx.clear();
    somaInputIdx.clear();
    inputSomasIdx.clear();
    somas.clear();
    outputSomasIdx.clear();

    std::string word, line, data_type;
    std::ifstream file(filename);
    if (file.is_open())
    {
        while (file >> word)
        {
            // skip coment in file
            if (word[0] == '#')
            {
                std::getline(file, line, '\n');
            }
            else
            {
                data_type = word;
                if (data_type == "soma")
                {
                    int soma_count;
                    std::string soma_count_str;
                    file >> soma_count_str;
                    soma_count = std::stoi(soma_count_str);
                    read_soma_data(file, soma_count);
                }
                else if (data_type == "synapse")
                {
                    int synapse_count;
                    std::string synapse_count_str;
                    file >> synapse_count_str;
                    synapse_count = std::stoi(synapse_count_str);
                    read_synapse_data(file, synapse_count);
                }
                else
                    std::cout << "could not read data" << std::endl;
            }
        }
    }
    else
        std::cout << "could not open file" << std::endl;
    
}

void neural_network::save(std::string filename)
{
    std::ofstream file;
    file.open(filename);
    // write soma data
    file << "soma " << somas.size() << "\n";
    // write the activation function for the somas
    for (auto soma : somas)
        switch (soma->getFunction())
        {
            case soma::LINEAR:
                file << "LINEAR\n";
                break;
            case soma::GAUSSIAN:
                file << "GAUSSIAN\n";
                break;
            case soma::RELU:
                file << "RELU\n";
                break;
            case soma::SIGMOID:
                file << "SIGMOID\n";
                break;
            default:
                break;
        }
    // write list input soma
    file << inputSomasIdx.size() << ' ';
    std::string s;
    for (auto idx : inputSomasIdx)
    {
        s += std::to_string(idx) + ' ';
    }
    s.pop_back();
    file << s << '\n';
    // write the synapsies
    file << "# input_soma output_soma weight\n"; // write a comment
    file << "synapse " << synapsies.size() << "\n";
    for(auto i = 0; i < synapseConnectionIdx.size(); i++)
    {
        file << synapseConnectionIdx[i].first << " " << synapseConnectionIdx[i].second << " " << synapsies[i]->getWeight() << "\n";
    }
    file.close();
}

void neural_network::read_soma_data(std::ifstream& file, int count)
{    
    // somas.resize(count);

    // if (somas.size() != count)
    // {
    //     for (size_t i = 0; i < count-somas.size(); i++)
    //         somas.push_back(std::make_unique<soma>());
    //     somaOutputIdx.resize(count);
    //     somaInputIdx.resize(count);
    // }
    
    for(int i = 0; i < count; i++)
    {
        std::string activation_str;
        file >> activation_str;
        soma::activationFunction func;
        if (activation_str == "LINEAR")
            func = soma::LINEAR;
        else if (activation_str == "GAUSSIAN")
            func = soma::GAUSSIAN;
        else if (activation_str == "RELU")
            func = soma::RELU;
        else if (activation_str == "SIGMOID")
            func = soma::SIGMOID;
        else
            std::cout << "could not read activation function" << std::endl;
        somaActivationFunction.push_back(func);
    }
    int input_soma_count;
    List input_soma;
    file >> input_soma_count;
    input_soma.resize(input_soma_count);

    for (int i = 0; i < input_soma_count; i++)
    {
        int idx;
        file >> idx;
        input_soma[i] = idx;
    }
    setInputSoma(input_soma);
}

void neural_network::read_synapse_data(std::ifstream& file, int count)
{
    ListOfConnectionsWithWeights connection;
    for(int i = 0; i < count; i++)
    {
        std::string in_str, out_str, w_str;
        int in, out;
        double w;

        file >> in_str >> out_str >> w_str;
        in = std::stoi(in_str);
        out = std::stoi(out_str);
        w = std::stod(w_str);
        connection.push_back({in, out, w});
    }
    wire(connection);
}


void neural_network::neural_test()
{
    wire({{0,2, 1.0}, {1,2, 1.0}, {1,1, 0.0}, {0,0, 0.5}});
    setAllSomaActivation(soma::LINEAR);
    
    setInputSoma({0,1});
    setInput({0.2, 0.5});
    setOutputSoma({2});
    for (size_t i = 0; i < 10; i++)
    {
        compute();
        std::vector<double> result = getOutput();
        for (auto res : result)
        {
            std::cout << res << ' ';
        }
        std::cout << std::endl;
    }
    add(2, 3, soma::GAUSSIAN);

    save("test");

    // load("test");
    // for (auto idx : inputSomasIdx)
    // {
    //     std::cout << idx << ' ';
    // }
    

}