#pragma once
#include <unordered_set>
#include <vector>

/**
 * @brief   make set of unassigned vertices
 * @param   unordered_set<int>* vertices set X
 * @param   unordered_set<int>* vertices set Y
 * @param   unordered_set<int>* unassigned vertice set v_prime
 * @param   int vertices
 */
void unassigned_vertice_set_maker(std::unordered_set<int> *,
                                  std::unordered_set<int> *,
                                  std::unordered_set<int> *,
                                  int);

/**
 * @brief  calculate the weight of given cut for given graph
 * @param  unordered_set<int>* vertices set X
 * @param  unordered_set<int>* vertices set Y
 * @param  graph_data* struct holding graph adjacency matrix
 * @return sum of crossing edges' weight
 */
int weight_of_cut(std::unordered_set<int> *, std::unordered_set<int> *,
                  struct graph_data *);

/**
 * @brief   return extreme(maximum/minimum) weight in graph
 * @param   graph_data* the struct holding the adjacency matrix
 * @param   bool    maximum or minimum weight, default maximum
 * @return  int maximum or minimum weight
 */
int extreme_weight_graph(struct graph_data *, bool);

/**
 * @brief   find and return two vertices with heaviest edge between them
 * @param   graph_data* the struct holding the adjacent matrix
 * @return  std::pair<int,int>   the edge
 */
std::pair<int, int> heaviest_edge_in_graph(struct graph_data *);

/**
 * @brief   return the added weight to cut if vertex was inserted in the other set
 * @param   int vertex to be checked if it was added to the other set
 * @param   unordered_set<int>* target_set to which the edges from given vertex
 *          would add to cut weight
 * @param   graph_data* struct holding adjacency_matrix
 * @return  sum of weights of all edges crossing the cut from the vertex to the
 *          target set
 */
int sigma_val_for(int, std::unordered_set<int> *, graph_data *);

/**
 * @brief   returns the current best vertex to add to one Set in greedy1
 * @param   unordered_set<int>* set X
 * @param   unordered_set<int>* set Y
 * @param   graph_data* struct holding adjacency_matrix
 * @return  int best vertex to choose now
 */
int best_vertex_greedy1(std::unordered_set<int> *, std::unordered_set<int> *,
                        graph_data *);

/**
 * @brief   is given edge in rcl
 * @param   std::vector<std::pair<int,int>>* RCL
 * @param   int vertex 1
 * @param   int vertex 2
 * @return  is in RCL
 */
bool is_in_list(std::vector<std::pair<int, int>> *, int, int);

/**
 * @brief   populates given RCL for given graph considering the cutoff weight
 * @param   double  cutoff weight
 * @param   std::vector<std::pair<int,int>>*   RCL
 * @param   graph_data* struct holding the adjacency matrix
 */
void semigreedy_alpha_rcl_populator(double, std::vector<std::pair<int, int>> *,
                                    struct graph_data *);

/**
 * @brief   populate sigma values for vertices not in the sets yet
 * @param   std::unordered_set<int>*    target set
 * @param   std::unordered_set<int>*    the other set
 * @param   int*   sigma value for edges to target Set from the other set
 * @param   graph_data* struct holding the adjacency matrix
 */
void semigreedy_sigma_array_populator(std::unordered_set<int> *,
                                      std::unordered_set<int> *, int *,
                                      struct graph_data *);

/**
 * @brief   randomly choose an edge from given RCL and insert to vertex sets
 * @param   std::unordered_set<int>*    set X
 * @param   std::unordered_set<int>*    set Y
 * @param   std::vector<std::pair<int,int>>*   RCL
 */
void semigreedy_alpha_random_edge_choose(std::unordered_set<int> *,
                                         std::unordered_set<int> *,
                                         std::vector<std::pair<int, int>> *);

/**
 * @brief   semigreedy cutoff weight calculation
 * @param   int*    sigma value array to set X
 * @param   int*    sigma value array to set Y
 * @param   double  alpha
 * @param   int  vertices
 * @param   std::unordered_set<int>*    set of unassigned vertices
 * @return  cutoff weight
 */
double semigreedy_cutoff_calculator(int *, int *, double, int,
                                    std::unordered_set<int> *);

/**
 * @brief   randomly choose a vertex from the vertex RCL and insert to the sets
 * @param   std::unordered_set<int>*    set X
 * @param   std::unordered_set<int>*    set Y
 * @param   graph_data*    graph_data struct holding adjacency matrix
 * @param   std::vector<int>*   vertex RCL
 */
void semigreedy_random_vertex_choose(std::unordered_set<int> *,
                                     std::unordered_set<int> *,
                                     std::vector<int> *,
                                     struct graph_data *);

/**
 * @brief   make vertex RCL using cutoff weight and sigma function values
 * @param   std::unordered_set<int>*    set of unassigned vertices
 * @param   int*    sigma value array to set X
 * @param   int*    sigma value array to set Y
 * @param   double  cutoff weight
 * @param   std::vector<int>* vertex rcl
 * @return  vertex RCL
 */
void semigreedy_vertex_RCL_maker(std::unordered_set<int> *,
                                 int *, int *, double, std::vector<int> *);

/**
 * @brief   improves given max cut if possible, changing the sets
 * @param   std::unordered_set<int>* vertices set X
 * @param   std::unordered_set<int>* vertices set Y
 * @param   graph_data* struct holding adjacency matrix
 * @return  the iteration count
 */
int local_search(std::unordered_set<int> *, std::unordered_set<int> *,
                  struct graph_data *);