#include <vector>
#include <iostream>
#include <iomanip>
#include "search_node.h"

class a_star
{
private:
    /* data */
    int _size;
    search_node* _init;
    std::vector<search_node*> _closed_list;
    //std::priority_queue<search_node*> _pq;
    /* methods */
    bool is_in_closed_list(search_node*);
    bool solvable();
    int inversions(search_node*);
public:
    a_star();
    a_star(int, search_node*, std::vector<search_node*>);
    ~a_star();
    void solve();
    void input();
};