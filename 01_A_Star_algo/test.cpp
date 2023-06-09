#include <iostream>
#include "search_node.h"

using namespace std;

int main()
{
    search_node *t = new search_node(nullptr, 3, 0, -1);
    for (int i = 0; i < 9; i++)
    {
        int k;
        cin >> k;
        t->add_to_grid(k);
    }
    t->print();
    cout << "\n";

    std::vector<search_node *> sons = t->get_neighbours();

    for (auto n : sons)
    {
        n->print();
        cout << "\n";
    }

    
    return 0;
}