#include <vector>

class search_node
{
private:
    /* data */
    search_node *_parent;
    int _grid_size;
    int *_grid;
    int _num_moves_from_init_node;
    /**
     * @brief direction of moves: 
     * 0 = UP,
     * 1 = DOWN,
     * 2 = LEFT,
     * 3 = RIGHT
     */
    int _move_from_parent;
    int _insertion_index;
    int _blank;

public:
    search_node();
    search_node(search_node* , int , int , int );
    ~search_node();
    void add_to_grid(int);
    void print();
    std::vector<search_node*> get_neighbours();
};
