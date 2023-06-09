#include <iostream>
#include <iomanip>
#include "search_node.h"

search_node::search_node() : _parent(nullptr), _grid_size(0), _grid(nullptr), _num_moves_from_init_node(0), _move_from_parent(-1), _insertion_index(-1), _blank(-1)
{
}

search_node::search_node(
    search_node *parent, int grid_size, int num_moves_from_init_node, int move_from_parent) : _parent(parent), _grid_size(grid_size), _grid(new int[_grid_size * _grid_size]), _num_moves_from_init_node(num_moves_from_init_node), _move_from_parent(move_from_parent), _insertion_index(0), _blank(0)
{
}

search_node::~search_node()
{
    delete[] _grid;
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
        std::cout << "|";

        for (int col = 0; col < _grid_size; col++)
        {
            std::cout << std::setw(2) << _grid[pos++] << "|";
        }
        std::cout << "\n";
    }
}

std::vector<search_node *> search_node::get_neighbours()
{
    std::vector<search_node *> neighbours;

    int row = _blank / _grid_size, col = _blank % _grid_size;

    if (_move_from_parent != 0 && row - 1 >= 0)
    {
        search_node *temp = new search_node(this, _grid_size, _num_moves_from_init_node + 1, 0);

        for (int i = 0; i < _grid_size * _grid_size; i++)
        {
            temp->_grid[i] = _grid[i];
        }

        std::swap(temp->_grid[_blank], temp->_grid[_blank - _grid_size]);
        temp->_blank = _blank - _grid_size;
        neighbours.push_back(temp);
    }

    if (_move_from_parent != 1 && row + 1 <= _grid_size - 1)
    {
        search_node *temp = new search_node(this, _grid_size, _num_moves_from_init_node + 1, 0);

        for (int i = 0; i < _grid_size * _grid_size; i++)
        {
            temp->_grid[i] = _grid[i];
        }
        std::swap(temp->_grid[_blank], temp->_grid[_blank + _grid_size]);
        temp->_blank = _blank + _grid_size;
        neighbours.push_back(temp);
    }

    if (_move_from_parent != 2 && col + 1 <= _grid_size - 1)
    {
        search_node *temp = new search_node(this, _grid_size, _num_moves_from_init_node + 1, 0);

        for (int i = 0; i < _grid_size * _grid_size; i++)
        {
            temp->_grid[i] = _grid[i];
        }
        std::swap(temp->_grid[_blank], temp->_grid[_blank + 1]);
        temp->_blank = _blank + 1;
        neighbours.push_back(temp);
    }

    if (_move_from_parent != 3 && col - 1 >= 0)
    {
        search_node *temp = new search_node(this, _grid_size, _num_moves_from_init_node + 1, 0);

        for (int i = 0; i < _grid_size * _grid_size; i++)
        {
            temp->_grid[i] = _grid[i];
        }
        std::swap(temp->_grid[_blank], temp->_grid[_blank - 1]);
        temp->_blank = _blank - 1;
        neighbours.push_back(temp);
    }

    return neighbours;
}
