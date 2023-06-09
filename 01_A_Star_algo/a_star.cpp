#include "a_star.h"

bool a_star::is_in_closed_list(search_node *)
{
    return false;
}

bool a_star::solvable()
{
    return false;
}

int a_star::inversions(search_node *)
{
    return 0;
}

a_star::a_star():_size(0), _init(nullptr), _closed_list(0)
{
}

a_star::a_star(int size, search_node * init, std::vector<search_node *> closed_list): _size(size), _init(init), _closed_list(closed_list)//_pq()
{
}

a_star::~a_star()
{

}

void a_star::solve()
{

}

void a_star::input()
{
    int t;
    std::cin >> _size;
    _init = new search_node(nullptr, _size, 0, -1);
    for (int i = 0; i < _size*_size; i++) {
        std::cin >> t;
        _init->add_to_grid(t);
    }
}
