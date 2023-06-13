#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>
#include "search_node.h"

#define manhattan 30
#define hamming 31


class CompareHamming
{
private:
    int hamming_count(search_node *);

public:
    bool operator()(search_node *, search_node *);
};

class CompareManhattan
{
private:
    int manhattan_count(search_node *);

public:
    bool operator()(search_node *, search_node *);
};
class a_star
{
private:
    int _size;
    search_node *_init;
    int _explored;
    int _expanded;
    std::priority_queue<search_node *, std::vector<search_node *>, CompareHamming> _pq;
    std::priority_queue<search_node *, std::vector<search_node *>, CompareManhattan> _mpq;
    bool is_solvable(search_node *);
    bool is_goal(search_node *);
    int inversions(search_node *);
    int merge(int[], int, int, int);
    int merge_sort(int[], int, int);
    void print_solve(search_node *);
    void print_metrices();
    search_node *find_goal_hamming(search_node *);
    search_node *find_goal(search_node *);
    void solve_algo(int);

public:
    a_star();
    a_star(int, search_node *);
    ~a_star();
    void solve(int);
    void input();
};
