#include <vector>

#define up_move 0
#define down_move 1
#define left_move 2
#define right_move 3
#define no_move -1

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
    bool _visited;
    search_node *gen_neighbour(int);

public:
    search_node();
    search_node(const search_node &);
    search_node(const search_node *);
    search_node(search_node *, int, int, int);
    ~search_node();
    int *get_grid();
    void add_to_grid(int);
    void print();
    std::vector<search_node *> get_neighbours();
    search_node *get_parent();
    bool is_visited();
    void set_visited(bool);
    int get_blank();
    int get_movecount_from_init();
    int get_size();
};
