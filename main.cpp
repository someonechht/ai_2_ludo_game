#include <iostream>
#include <numeric>
#include <thread>
#include "game.h"
#include "test_game.h"
#include "iplayer.h"
#include "player_random.h"
#include "neural_network.h"
#include "custom_player.h"

using namespace std;

void test_agent(custom_player player_0, int games, double& score, bool& job_finished)
{
    player_random player_1, player_2, player_3;    

    //Play a game of Ludo
    game g(&player_0, &player_1, &player_2, &player_3);
    std::array<int,4> wins = {0, 0, 0, 0};


    g.play_game();
    wins[g.get_winner()]++;
    //Play many games of Ludo
    for(int i = 0; i < games; i++)
    {
        g.reset();
        g.set_first(i%4); //alternate who starts the game
        g.play_game();
        wins[g.get_winner()]++;
    }
    double winrate = double(wins[0])/double(std::accumulate(wins.begin(), wins.end(), 0));
    score = winrate;
    job_finished = true;
}
double eval_agent(custom_player& player_0, int games)
{
    player_random player_1, player_2, player_3;    

    //Play a game of Ludo
    game g(&player_0, &player_1, &player_2, &player_3);
    std::array<int,4> wins = {0, 0, 0, 0};


    g.play_game();
    wins[g.get_winner()]++;
    //Play many games of Ludo
    for(int i = 0; i < games; i++)
    {
        g.reset();
        g.set_first(i%4); //alternate who starts the game
        g.play_game();
        wins[g.get_winner()]++;
    }
    double winrate = double(wins[0])/double(std::accumulate(wins.begin(), wins.end(), 0));
    return winrate;
}

void test_brain(std::string path, int games)
{
    custom_player agent;
    agent.brain.load(path);
    double winrate;
    winrate = eval_agent(agent, games);
    std::cout << "winrate for " << path << " :" << winrate << std::endl;

}

int main()
{
    std::vector<std::thread> threads;
    int population = 20; //number of agents is defined herre
    ofstream datafile, weight_file;
    std::array<bool, 24> job_finished; // number of threads is defined here
    job_finished.fill(true);
    std::string test = "_var_2";

    // custom_player player;
    std::vector<std::unique_ptr<custom_player>> agents;
    std::vector<double> agents_score;
    for (size_t i = 0; i < population; i++)
    {
        agents.push_back(std::make_unique<custom_player>());
    }
    
    agents_score.resize(population);
    for (auto& a : agents)
        a->randomize_brain();

    double winrate = 0, highest_winrate = 0;
    int gen = 0;
    datafile.open("evolution_data"+test+".csv");
    datafile << "generation; best; average; worst" << std::endl;
    weight_file.open("weights"+test+".csv");
    custom_player best_agent, overall_best_agent;

    while (gen <= 500) // number of itterations to run (or other exit condition)
    {
        
        bool threads_available = false;
        int available_thread = 0;
        // spawn threads to test agents
        int agent_trained = 0;
        for (int i = 0; i < agents.size(); i++)
        {
            //waiting for jobs/threads to be available
            while (!threads_available)
            {
                for(int j = 0; j < job_finished.size(); j++)
                {
                    if (job_finished[j])
                    {
                        available_thread = j;
                        job_finished[j] = false;
                        threads_available = true;
                        break;
                    }
                }
            }
            
            threads.emplace_back(std::thread(test_agent, *agents[i].get(),200, std::ref(agents_score[i]), std::ref(job_finished[available_thread])));
            threads_available = false;
            agent_trained++;
        }

        // wait for all thread to finish
        for (auto& th : threads) 
            th.join();
        
        // get best agent
        int best_idx = std::max_element(agents_score.begin(),agents_score.end()) - agents_score.begin();
        // get worst score
        double worst = *std::min_element(agents_score.begin(), agents_score.end());
        // get average score
        double avg = std::accumulate(agents_score.begin(), agents_score.end(), 0.0) / double(agents_score.size());

        // copy brain of higest scoring agent to best_agent variable
        for (size_t i = 0; i < agents[best_idx]->brain.synapse_weight.size(); i++)
            *best_agent.brain.synapse_weight[i] = *agents[best_idx]->brain.synapse_weight[i];
        
        if (highest_winrate < agents_score[best_idx])
        {
            highest_winrate = agents_score[best_idx];
            for (size_t i = 0; i < agents[best_idx]->brain.synapse_weight.size(); i++)
                *overall_best_agent.brain.synapse_weight[i] = *agents[best_idx]->brain.synapse_weight[i];
        }
        

        std::cout << "generation " << gen 
            << " best: " << agents_score[best_idx] 
            << " avg: " << avg 
            << " worst: " << worst 
            << std::endl;
        // generate new agent based on previous best_agent
        for (auto& agent : agents)
        {
            // copy previous best_agents brain over
            for (size_t i = 0; i < agent->brain.synapse_weight.size(); i++)
                *agent->brain.synapse_weight[i] = *best_agent.brain.synapse_weight[i];
            
            // randomize the brain with a Delta_w
            agent->randomize_brain();
        }

        // write best agents weight to file
        for (auto weight : best_agent.brain.synapse_weight)
        {
            weight_file << (*weight) << "; ";
        }
        weight_file << std::endl;
        

        threads.clear();
        gen++;
        datafile << gen << "; " 
            << agents_score[best_idx] << "; "
            << avg << "; "
            << worst << std::endl;
    }
    datafile.close();
    best_agent.brain.save("best_nn"+test+".txt");
    overall_best_agent.brain.save("overall_best"+test+".txt");

    
    std::cout << "End of main" << std::endl;
    return 0;
}


