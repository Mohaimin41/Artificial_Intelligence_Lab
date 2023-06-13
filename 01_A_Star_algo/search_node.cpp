#include <iostream>
#include <iomanip>
#include "search_node.h"

search_node *search_node::gen_neighbour(int move)
{
    int goal_blank_pos = -1;
    search_node *temp;
    try
    {
        temp = new search_node(this, _grid_size, _num_moves_from_init_node + 1, move);
    }
    catch (const std::bad_alloc &e)
    {
        std::cout << "Allocation failed @line 11 in search_node.cpp: " << e.what()
                  << '\n';
    }

    for (int i = 0; i < _grid_size * _grid_size; i++)
    {
        temp->_grid[i] = _grid[i];
    }

    if (move == up_move)
    {
        goal_blank_pos = _blank - _grid_size;
    }
    else if (move == down_move)
    {
        goal_blank_pos = _blank + _grid_size;
    }
    else if (move == left_move)
    {
        goal_blank_pos = _blank - 1;
    }
    else if (move == right_move)
    {
        goal_blank_pos = _blank + 1;
    }

    std::swap(temp->_grid[_blank], temp->_grid[goal_blank_pos]);
    temp->_blank = goal_blank_pos;

    return temp;
}

search_node::search_node() : _parent(nullptr), _grid_size(0), _grid(nullptr),
                             _num_moves_from_init_node(0), _move_from_parent(no_move),
                             _insertion_index(-1), _blank(-1), _visited(false)
{
}

search_node::search_node(const search_node &node)
    : _parent(node._parent), _grid_size(node._grid_size),
      _num_moves_from_init_node(node._num_moves_from_init_node),
      _move_from_parent(node._move_from_parent), _insertion_index(node._insertion_index),
      _blank(node._blank), _visited(node._visited)
{
    _grid = new int[_grid_size * _grid_size];
    for (int i = 0; i < _grid_size * _grid_size; i++)
        _grid[i] = node._grid[i];
}

search_node::search_node(const search_node *node)
    : _parent(node->_parent), _grid_size(node->_grid_size),
      _num_moves_from_init_node(node->_num_moves_from_init_node),
      _move_from_parent(node->_move_from_parent), _insertion_index(node->_insertion_index),
      _blank(node->_blank), _visited(node->_visited)
{
    _grid = new int[_grid_size * _grid_size];
    for (int i = 0; i < _grid_size * _grid_size; i++)
        _grid[i] = node->_grid[i];
}

search_node::search_node(search_node *parent, int grid_size, int num_moves_from_init_node, int move_from_parent)
    : _parent(parent), _grid_size(grid_size), _grid(new int[_grid_size * _grid_size]),
      _num_moves_from_init_node(num_moves_from_init_node),
      _move_from_parent(move_from_parent), _insertion_index(0), _blank(0), _visited(false)
{
}

search_node::~search_node()
{
    delete[] _grid;
}

int *search_node::get_grid()
{
    return _grid;
}

void search_node::add_to_grid(int val)
{
    _grid[_insertion_index++] = val;
    if (val == 0)
    {
        _blank = _insertion_index - 1;
    }
}

void search_node::print()
{
    int pos = 0;
    for (int row = 0; row < _grid_size; row++)
    {

        for (int col = 0; col < _grid_size; col++)
        {
            std::cout << std::left << std::setw(1) << _grid[pos++] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::vector<search_node *> search_node::get_neighbours()
{
    std::vector<search_node *> neighbours;

    int row = _blank / _grid_size, col = _blank % _grid_size;

    if (_move_from_parent != down_move && row - 1 >= 0)
    {
        neighbours.push_back(gen_neighbour(up_move));
    }

    if (_move_from_parent != up_move && row + 1 <= _grid_size - 1)
    {
        neighbours.push_back(gen_neighbour(down_move));
    }

    if (_move_from_parent != right_move && col - 1 >= 0)
    {
        neighbours.push_back(gen_neighbour(left_move));
    }

    if (_move_from_parent != left_move && col + 1 <= _grid_size - 1)
    {
        neighbours.push_back(gen_neighbour(right_move));
    }

    return neighbours;
}

search_node *search_node::get_parent()
{
    return _parent;
}

bool search_node::is_visited()
{
    return _visited;
}

void search_node::set_visited(bool val)
{
    _visited = val;
}

int search_node::get_blank()
{
    return _blank;
}

int search_node::get_movecount_from_init()
{
    return _num_moves_from_init_node;
}

int search_node::get_size()
{
    return _grid_size;
}
