#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cmath>
#include <chrono>
#include <random>

#include "graph_simulation.h"
#include "graph_helpers.h"

//========================== I/O & Utility Helpers==============================

void start_output(std::ofstream *output_file)
{
    (*output_file) << "Name, |V|, |E|, Randomized1 Value, Greedy1 value,"
              << " SemiGreedy1 value, LocalSearch Iterations, LocalSearch Best value,"
              << " GRASP Iterations, GRASP Best value, Known Best Solution\n";
}

void read_best_solns(char *filename, int *best_soln_storage)
{
    std::string single_line;

    std::ifstream input_file(filename);

    while (getline(input_file, single_line))
    {
        // find index  of "," in single_line
        int comma_index = -1;
        for (int i = 0; i < (int)single_line.size(); i++)
        {
            if (single_line[i] == ',')
            {
                comma_index = i;
                break;
            }
        }

        // store in best_soln_storage the graph num and the solution
        int graph_num = stoi(single_line.substr(1, comma_index - 1));
        int graph_soln = stoi(single_line.substr(comma_index + 2));

        best_soln_storage[graph_num] = graph_soln;
    }
}

void read_graph(const std::filesystem::directory_entry *fileentry,
                struct graph_data *graph)
{
    int vertices = 0, edges = 0;

    std::ifstream input_file((*fileentry).path().c_str());

    input_file >> vertices >> edges;

    graph->vertices = vertices;
    graph->edges = edges;
    graph->adjacency_matrix = new int[vertices * vertices]; 
    for (int i = 0; i < vertices * vertices; i++)
    {
        graph->adjacency_matrix[i] = 0;
    }

    for (int i = 0; i < edges; i++)
    {
        int row = 0, col = 0, weight = 0;

        // 0 indexed graph matrices, so decrementing 1
        input_file >> row >> col >> weight;
        row--, col--;

        //storing in both instances of this edge
        graph->adjacency_matrix[row * vertices + col] = weight;
        graph->adjacency_matrix[col * vertices + row] = weight;
    }
}

//==============================Algo Helpers====================================

void unassigned_vertice_set_maker(std::unordered_set<int> *X,
                                  std::unordered_set<int> *Y,
                                  std::unordered_set<int> *v_prime,
                                  int vertices)
{
    for (int i = 0; i < vertices; i++)
    {
        if (X->find(i) == X->end() && Y->find(i) == Y->end())
        {
            v_prime->insert(i);
        }
    }
}

int weight_of_cut(std::unordered_set<int> *X, std::unordered_set<int> *Y,
                  graph_data *gr)
{
    int cut_weight = 0;
    if (X->size() + Y->size() != (unsigned int)gr->vertices)
        return -1;

    for (auto v1 : *X)
    {
        for (auto v2 : *Y)
        {
            cut_weight += gr->adjacency_matrix[v1 * gr->vertices + v2];
        }
    }
    
    return cut_weight;
}

int extreme_weight_graph(graph_data *gr, bool is_max = true)
{
    int extreme = 0;
    if (is_max)
    {
        extreme = INT32_MIN;
    }
    else
    {
        extreme = INT32_MAX;
    }
    for (int i = 0; i < gr->vertices; i++)
    {
        for (int j = 0; j < gr->vertices; j++)
        {
            if ((gr->adjacency_matrix[i * gr->vertices + j] > extreme && is_max) ||
                (gr->adjacency_matrix[i * gr->vertices + j] < extreme && !is_max))
            {
                extreme = gr->adjacency_matrix[i * gr->vertices + j];
            }
        }
    }
    return extreme;
}

//========================= Greedy1 Helpers & Algo =============================

std::pair<int, int> heaviest_edge_in_graph(graph_data *gr)
{
    int heaviest_edge = INT32_MIN;
    int row = -1;
    int col = -1;
    int vertices = gr->vertices;

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (gr->adjacency_matrix[i * vertices + j] >= heaviest_edge)
            {
                row = i, col = j;
                heaviest_edge = gr->adjacency_matrix[i * vertices + j];
            }
        }
    }
    
    return std::pair<int, int>(row, col);
}

int sigma_val_for(int vertex, std::unordered_set<int> *target_set, graph_data *gr)
{
    int weight = 0;

    for (auto v : *target_set)
    {
        weight += gr->adjacency_matrix[vertex * gr->vertices + v];
    }

    return weight;
}

int best_vertex_greedy1(std::unordered_set<int> *X, std::unordered_set<int> *Y,
                        graph_data *gr)
{
    int max_contribution = INT32_MIN;
    int best_vertex = 0;
    int curr_vertex = 0;

    for (; curr_vertex < gr->vertices; curr_vertex++)
    {
        if (X->find(curr_vertex) == X->end() &&
            Y->find(curr_vertex) == Y->end()) // only if not in X and Y sets
        {
            // get the larger contribution from putting current vertex in one of
            // the sets
            int curr_contribution = std::max(
                sigma_val_for(curr_vertex, X, gr),
                sigma_val_for(curr_vertex, Y, gr));

            // update greedy function result
            if (max_contribution < curr_contribution)
            {
                best_vertex = curr_vertex;
                max_contribution = curr_contribution;
            }
        }
    }

    return best_vertex;
}

int greedy1(graph_data *gr)
{
    int max_cut = 0;

    std::unordered_set<int> X;
    std::unordered_set<int> Y;

    std::pair<int, int> heaviest_edge = heaviest_edge_in_graph(gr);

    // initially we choose the heaviest edge
    X.insert(heaviest_edge.first), Y.insert(heaviest_edge.second);

    do
    {
        // choose the vertex with highest contribution to cut
        int next_best_vertex = best_vertex_greedy1(&X, &Y, gr);

        int sigma_to_Y = sigma_val_for(next_best_vertex, &Y, gr);
        int sigma_to_X = sigma_val_for(next_best_vertex, &X, gr);

        // choose which set to which this vertex would be added
        if (sigma_to_X < sigma_to_Y)
        {
            X.insert(next_best_vertex);
        }
        else
        {
            Y.insert(next_best_vertex);
        }
    } while (X.size() + Y.size() < (unsigned int)gr->vertices);

    // cut weight
    max_cut = weight_of_cut(&X, &Y, gr);

    return max_cut;
}

//======================= Randomized1 Helpers & Algo ===========================

int randomized1(graph_data *gr)
{
    int max_cut = 0;
    std::unordered_set<int> X;
    std::unordered_set<int> Y;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed); // random generator for use with urd
    std::mt19937 generator2(seed);              // mersenne twister generator
    std::uniform_real_distribution<> urd(0, 1); // uniform real distribution with range [0, 1)

    // random toss to choose X or Y for all vertices
    for (int i = 0; i < gr->vertices; i++)
    {
        if (urd(generator2) <= 0.5f)
        {
            X.insert(i);
        }
        else
        {
            Y.insert(i);
        }
    }

    // cut weight
    max_cut = weight_of_cut(&X, &Y, gr);

    return max_cut;
}

int avg_randomized1(graph_data *gr)
{
    int max_cut = 0;
    for (int i = 0; i < N_RUN; i++)
    {
        max_cut += randomized1(gr);
    }
    return round((max_cut * 1.0) / (N_RUN));
}

//================= SemiGreedyAlpha Helpers & Algo =============================

bool is_in_list(std::vector<std::pair<int, int>> *RCL, int v1, int v2)
{
    for (auto edge : *RCL)
    {
        if (edge.first == v1 && edge.second == v2)
        {
            return true;
        }
    }
    return false;
}

void semigreedy_alpha_rcl_populator(double cutoff_weight,
                                    std::vector<std::pair<int, int>> *RCL,
                                    graph_data *gr)
{
    for (int i = 0; i < gr->vertices; i++)
    {
        for (int j = 0; j < gr->vertices; j++)
        {
            if (gr->adjacency_matrix[i * gr->vertices + j] >= cutoff_weight &&
                !is_in_list(RCL, i, j))
            {
                RCL->push_back(std::pair<int, int>(i, j));
            }
        }
    }
}

void semigreedy_sigma_array_populator(std::unordered_set<int> *target_set,
                                      std::unordered_set<int> *other_set,
                                      int *sigma_val_array,
                                      struct graph_data *gr)
{
    for (int i = 0; i < gr->vertices; i++)
    {
        if (target_set->find(i) == target_set->end() &&
            other_set->find(i) == other_set->end()) // unassigned vertices
        {
            sigma_val_array[i] = sigma_val_for(i, target_set, gr);
        }
    }
}

void semigreedy_alpha_random_edge_choose(std::unordered_set<int> *X,
                                         std::unordered_set<int> *Y,
                                         std::vector<std::pair<int, int>> *RCL)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed); // random generator for use with pd
    std::mt19937 generator2(seed);              // mesenne twister generator
    std::poisson_distribution<> pd(3.6);        // poisson distribution with mean 3.6

    int index = pd(generator2) % RCL->size();

    X->insert(RCL->at(index).first);
    Y->insert(RCL->at(index).second);
}

double semigreedy_cutoff_calculator(int *sigma_to_X, int *sigma_to_Y,
                                    double alpha, int vertices,
                                    std::unordered_set<int> *v_prime)
{

    int min_weight = INT32_MAX;
    int max_weight = INT32_MIN;

    for (int i = 0; i < vertices; i++)
    {
        if (v_prime->find(i) != v_prime->end()) // unassigned vertices anly
        {
            //w_min = min( min_{all v from V'}(sigma_X(v)), min_{all v from V'}(sigma_Y(v)) ) 
            //w^max = max( max_{all v from V'}(sigma_X(v)), max_{all v from V'}(sigma_X(v)))
            min_weight = std::min(min_weight,
                                  std::min(sigma_to_X[i], sigma_to_Y[i]));
            max_weight = std::max(max_weight,
                                  std::max(sigma_to_X[i], sigma_to_Y[i]));
        }
    }

    return min_weight + alpha * (max_weight - min_weight) * 1.0;
}

void semigreedy_vertex_RCL_maker(std::unordered_set<int> *V_prime,
                                 int *sigma_to_X, int *sigma_to_Y,
                                 double cutoff, std::vector<int> *vertex_RCL)
{
    for (auto v : *V_prime)
    {
        if (std::max(sigma_to_X[v], sigma_to_Y[v]) >= cutoff)
        {
            vertex_RCL->push_back(v);
        }
    }
}

void semigreedy_random_vertex_choose(std::unordered_set<int> *X,
                                     std::unordered_set<int> *Y,
                                     std::vector<int> *vertex_rcl,
                                     struct graph_data *gr)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed); // random generator for use with pd
    std::mt19937 generator2(seed);              // mersenne twister generator
    std::poisson_distribution<> pd(3.6);        // poisson distribution with mean 3.6

    int index = pd(generator2) % vertex_rcl->size();

    if (sigma_val_for(vertex_rcl->at(index), Y, gr) >
        sigma_val_for(vertex_rcl->at(index), X, gr))
    {
        X->insert(vertex_rcl->at(index)); // vertex would be in X, as Y as target
                                          // has more contribution
    }
    else
    {
        Y->insert(vertex_rcl->at(index));
    }
}

int semi_greedy_alpha(graph_data *gr, std::unordered_set<int> *X,
                      std::unordered_set<int> *Y)
{
    int max_cut = 0;

    int max_weight = extreme_weight_graph(gr);
    int min_weight = extreme_weight_graph(gr, false);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed); // random generator for use with urd
    std::mt19937 generator2(seed);              // mersenne twister generator
    std::uniform_real_distribution<double> urd(0, 1); // uniform real distribution with range [0, 1)

    double alpha = urd(generator2);

    double cutoff_weight = min_weight + alpha * (max_weight - min_weight);

    std::vector<std::pair<int, int>> RCL_edges;

    semigreedy_alpha_rcl_populator(cutoff_weight, &RCL_edges, gr);

    semigreedy_alpha_random_edge_choose(X, Y, &RCL_edges);

    do
    {
        int sigma_to_X_from_Y[gr->vertices] = {0};
        int sigma_to_Y_from_X[gr->vertices] = {0};

        semigreedy_sigma_array_populator(X, Y, sigma_to_X_from_Y, gr);
        semigreedy_sigma_array_populator(Y, X, sigma_to_Y_from_X, gr);

        std::unordered_set<int> v_prime;
        unassigned_vertice_set_maker(X, Y, &v_prime, gr->vertices);

        cutoff_weight = semigreedy_cutoff_calculator(sigma_to_X_from_Y,
                                                     sigma_to_Y_from_X, alpha,
                                                     gr->vertices,
                                                     &v_prime);

        std::vector<int> vertex_rcl;
        semigreedy_vertex_RCL_maker(&v_prime, sigma_to_X_from_Y,
                                    sigma_to_Y_from_X, cutoff_weight, &vertex_rcl);

        semigreedy_random_vertex_choose(X, Y, &vertex_rcl, gr);

    } while (X->size() + Y->size() < (unsigned int)gr->vertices);

    max_cut = weight_of_cut(X, Y, gr);

    return max_cut;
}

int avg_semigreedy_alpha(graph_data *gr)
{
    int max_cut = 0;

    std::unordered_set<int> X;
    std::unordered_set<int> Y;

    for (int i = 0; i < N_RUN; i++)
    {
        max_cut += semi_greedy_alpha(gr, &X, &Y);
        
        X.clear();
        Y.clear();
    }
    
    return round((1.0 * max_cut) / N_RUN);
}

//======================== GRASP Helpers & Algo ================================

int local_search(std::unordered_set<int> *X, std::unordered_set<int> *Y,
                 graph_data *gr)
{
    int iterations = 0;
    bool changing = true;
    while (changing)
    {
        iterations++;
        changing = false;

        for (int i = 0; i < gr->vertices && !changing; i++)
        {
            int moving_from_X_to_Y_val = sigma_val_for(i, X, gr);
            int moving_from_Y_to_X_val = sigma_val_for(i, Y, gr);

            if (X->find(i) != X->end() &&
                moving_from_X_to_Y_val > moving_from_Y_to_X_val) // is in X, moves to Y
            {
                X->erase(X->find(i));
                Y->insert(i);
                changing = true;
            }
            else if (Y->find(i) != Y->end() &&
                     moving_from_Y_to_X_val > moving_from_X_to_Y_val) // is in Y, moves to X
            {
                Y->erase(Y->find(i));
                X->insert(i);
                changing = true;
            }
        }
    }
    return iterations;
}

GRASP_return_data GRASP(int iters, graph_data *gr)
{
    int best_weight = INT32_MIN;
    int local_search_iteration = 0;
    int local_search_best_val = 0;
    int grasp_iteration = iters;

    for (int i = 0; i < grasp_iteration; i++)
    {
        std::unordered_set<int> X;
        std::unordered_set<int> Y;

        semi_greedy_alpha(gr, &X, &Y);
        local_search_iteration += local_search(&X, &Y, gr);
        int curr_weight = weight_of_cut(&X, &Y, gr);

        local_search_best_val += curr_weight;

        if (curr_weight > best_weight)
        {
            best_weight = curr_weight;
        }
    }

    return GRASP_return_data(
        round((1.0 * local_search_iteration) / grasp_iteration),
        round((1.0 * local_search_best_val) / grasp_iteration),
        grasp_iteration, best_weight);
}
