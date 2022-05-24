
// #include <vector>
// #include "random"
// #include "iplayer.h"
// #include "neural_network.h"
#include "custom_player.h"


custom_player::custom_player()
{
    input.resize(18);
    std::random_device rd;
    generator = std::mt19937(rd());
    brain.setInputSoma({
        0, //dice
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, //player positions
        17 //bias
    }); //input layer
    brain.add(18*2, 3,  soma::SIGMOID); //hidden layer
    brain.add(4, soma::SIGMOID); //output layer

    brain.build(); // bild the brain
    for (auto w : brain.synapse_weight)
        *w = start_weight; // initial weight

    
    
}

void custom_player::randomize_brain()
{
    double max_weight = 0;
    for (auto weights : brain.synapse_weight)
    {
        distribution = std::normal_distribution<double>(0, variation);
        double delta_w = distribution(generator); 
        *weights += delta_w; //weight update
    }
    
}

bool compare(int a, int b, double* number)
{
    return number[a] < number[b];
}

int  custom_player::make_decision() //Selects legal move at random
{
    
    std::vector<int> options;
    // give input to brain
    input[0] = (double(dice)/6.0);
    for(int i = 0; i <= 16; i++)
        input[i+1] = (double(position[i])+1)/100.0;
    input[17] = 1; //bias term
    brain.setInput(input);

    // compute output
    brain.compute();

    std::vector<double> output = brain.getOutput();

    // sort for highest to lowes scoring move
    std::vector<size_t> indicies( output.size(), 0);
    std::iota(indicies.begin(), indicies.end(), 0);
    std::stable_sort(indicies.begin(), indicies.end(), 
        [output](size_t idx_1, size_t idx_2) 
        {
            return output[idx_1] > output[idx_2];
        }
    );

    int move_taken = 0;

    for (auto move : indicies) // loop through best to worst move
    {
        if (is_valid_move(move) and (position[move] < 52))
        {
            move_taken = move;
            break;
        }
    }   
    return move_taken;
}

