#pragma once
#include <unordered_set>

/**
 * @brief   1 indexed known best solutions array for ease of use
 */
#define TOTAL_GRAPHS 55
/**
 * @brief   total num of runs for randomized and semi greedy algos
 */
#define N_RUN 10
/**
 * @brief   total iterations in GRASP
 */
#define GRASP_ITERATIONS 35

/**
 * @brief   graph data struct, members:
 *          1. int vertices
 *          2. int edges
 *          3. int graph_num(file number)
 *          4. int* adjacency_matrix
 */
struct graph_data
{
    int vertices;
    int edges;
    int graph_num;
    int *adjacency_matrix;

    graph_data() : vertices(0), edges(0), graph_num(0), adjacency_matrix(nullptr)
    {
    }
    graph_data(int v, int e) : vertices(v), edges(e), graph_num(0)
    {
        adjacency_matrix = new int[vertices * vertices];
    }
    graph_data(int v, int e, int *matrix)
    {
        graph_data(v, e);
        adjacency_matrix = matrix;
    }
    ~graph_data()
    {
        delete[] adjacency_matrix;
    }
};

/**
 * @brief   GRASP algo return data struct, members:
 *          1. int local_search_iter_count
 *          2. int local_search_best_val
 *          3. int grasp_iter_count
 *          4. int grasp_best_val
 */
struct GRASP_return_data
{
    /* data */
    int local_search_iter_count;
    int local_search_best_val;
    int grasp_iter_count;
    int grasp_best_val;

    GRASP_return_data(int local_iter, int local_best, int grasp_iter,
                      int grasp_best)
    {
        local_search_iter_count = local_iter;
        local_search_best_val = local_best;
        grasp_iter_count = grasp_iter;
        grasp_best_val = grasp_best;
    }
};

/**
 * @brief   prints csv header to output file
 * @param   ofstream*   pointer to output file stream
 */
void start_output(std::ofstream*);

/**
 * @brief   read from file at the char* location to the int* array,
 *          used to store known best results of benchmark graphs
 * @param   char* location:  relative filepath
 * @param   int* best_solns_storage: 1 index array to store the best solutions
 */
void read_best_solns(char *, int *);

/**
 * @brief   read from files at the directory_entry* location to the int* array,
 *          used to weight data of benchmark graphs
 * @param   directory_entry* fileentry: the graph file
 * @param   graph_data*  struct graph_data holding adjacency matrix
 */
void read_graph(const std::filesystem::directory_entry *, struct graph_data *);

/**
 * @brief   greedy max cut algo, chooses edge with highest greedy function value
 * @param   graph_data*    graph_data struct holding adjacency matrix
 * @return  max_cut value
 */
int greedy1(struct graph_data *);

/**
 * @brief   randomized max cut algo, simply tosses to put a vertex in one of the cuts
 * @param   graph_data*    graph_data struct holding adjacency matrix
 * @return  max_cut value
 */
int randomized1(struct graph_data *);

/**
 * @brief   average randomized1 runs N_RUN times
 * @param   graph_data*    graph_data struct holding adjacency matrix
 * @return  rounded to nearest integer average max_cut value
 */
int avg_randomized1(struct graph_data *);

/**
 * @brief   semi greedy max cut algo, chooses edge from a restricted edge list,
 *          with cutoff alpha
 * @param   graph_data*    graph_data struct holding adjacency matrix
 * @param   std::unordered_set<int>*    vertices set X
 * @param   std::unordered_set<int>*    vertices set Y
 * @return  max_cut value
 */
int semi_greedy_alpha(struct graph_data *, std::unordered_set<int> *,
                      std::unordered_set<int> *);

/**
 * @brief   average over N_RUN runs of semigreedy_alpha
 * @param   graph_data*    graph_data struct holding adjacency matrix
 * @return  average max_cut value
 */
int avg_semigreedy_alpha(struct graph_data *);

/**
 * @brief   GRASP max cut algo, does local search on semi_greedy_alpha construction
 * @param   int GRASP maximum interation count
 * @param   graph_data*    graph_data struct holding adjacency matrix
 * @return  struct holding averaged data for local search and grasp results
 */
struct GRASP_return_data GRASP(int, struct graph_data *);