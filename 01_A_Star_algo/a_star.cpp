#include <stack>
#include "a_star.h"

bool a_star::is_solvable(search_node *init_node)
{
    int inv_count = inversions(init_node);
    if (_size % 2)
    {
        if (inv_count % 2)
            return false;
        else
            return true;
    }
    else
    {
        int reverse_row = _size - (_init->get_blank() / _size);
        if ((reverse_row % 2 && inv_count % 2 == 0) ||
            (reverse_row % 2 == 0 && inv_count % 2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool a_star::is_goal(search_node *test_node)
{
    int *grid = test_node->get_grid();
    for (int i = 0; i < test_node->get_size() * test_node->get_size() - 1; i++)
    {
        if (grid[i] != i + 1)
            return false;
    }
    if (grid[test_node->get_size() * test_node->get_size() - 1] != 0)
        return false;
    return true;
}

int a_star::inversions(search_node *node)
{
    int arr[_size * _size - 1] = {0};
    int *temp = node->get_grid();
    for (int i = 0, j = 0; i < _size * _size; i++)
    {
        if (temp[i] == 0)
            continue;
        arr[j++] = temp[i];
    }

    return merge_sort(arr, 0, _size * _size - 2);
}

int a_star::merge(int arr[], int left, int mid, int right)
{
    int left_pointer = left, right_pointer = mid, running = 0;
    int inv_count = 0;
    int temp[(right - left + 1)];

    while ((left_pointer < mid) && (right_pointer <= right))
    {
        if (arr[left_pointer] <= arr[right_pointer])
        {
            temp[running] = arr[left_pointer];
            ++running;
            ++left_pointer;
        }
        else
        {
            temp[running] = arr[right_pointer];
            inv_count += (mid - left_pointer);
            ++running;
            ++right_pointer;
        }
    }

    while (left_pointer < mid)
    {
        temp[running] = arr[left_pointer];
        ++running;
        ++left_pointer;
    }

    while (right_pointer <= right)
    {
        temp[running] = arr[right_pointer];
        ++running;
        ++right_pointer;
    }

    for (left_pointer = left, running = 0; left_pointer <= right;
         left_pointer++, running++)
    {
        arr[left_pointer] = temp[running];
    }

    return inv_count;
}

int a_star::merge_sort(int arr[], int left, int right)
{
    int inv_count = 0;

    if (right > left)
    {
        int mid = (right + left) / 2;
        inv_count = merge_sort(arr, left, mid);
        inv_count += merge_sort(arr, mid + 1, right);
        inv_count += merge(arr, left, mid + 1, right);
    }
    return inv_count;
}

void a_star::print_solve(search_node *goal)
{
    std::stack<search_node *> nodes;
    search_node *temp = goal;
    std::cout << "Minimum number of moves = " << temp->get_movecount_from_init()
              << "\n\n";

    while (temp)
    {
        nodes.push(temp);

        temp = temp->get_parent();
    }
    while (!nodes.empty())
    {
        temp = nodes.top();
        temp->print();
        nodes.pop();
    }
}

void a_star::print_metrices()
{
    std::cout << "Number of explored nodes: " << _explored
              << "\nNumber of expanded notes: " << _expanded << "\n";
}

search_node *a_star::find_goal_hamming(search_node *init_node)
{
    _pq.push(init_node);
    ++_expanded;
    search_node *temp = _pq.top();
    _pq.pop();
    ++_explored;
    temp->set_visited(true);

    while (!is_goal(temp))
    {
        std::vector<search_node *> neighbours = temp->get_neighbours();
        for (auto node : neighbours)
        {
            if (node->is_visited())
                continue;
            _pq.push(node);
            ++_expanded;
        }
        temp = _pq.top();
        _pq.pop();
        ++_explored;
        temp->set_visited(true);
    }

    return temp;
}

search_node *a_star::find_goal(search_node *init_node)
{
    _mpq.push(init_node);
    ++_expanded;
    search_node *temp = _mpq.top();
    _mpq.pop();
    ++_explored;
    temp->set_visited(true);

    while (!is_goal(temp))
    {
        std::vector<search_node *> neighbours = temp->get_neighbours();
        for (auto node : neighbours)
        {
            if (node->is_visited())
                continue;
            _mpq.push(node);
            ++_expanded;
        }
        temp = _mpq.top();
        _mpq.pop();
        ++_explored;
        temp->set_visited(true);
    }

    return temp;
}

/**
 * @brief solves puzzle and prints it using specified heuristics
 *
 * @param option 1 for Manhattan distance, 2 for Hamming distance
 */
void a_star::solve_algo(int option)
{
    search_node *goal;
    _expanded = _explored = 0;
    if (option == manhattan)
    {
        goal = find_goal(_init);
    }
    else
    {
        goal = find_goal_hamming(_init);
    }

    print_solve(goal);
}

a_star::a_star() : _size(0), _init(nullptr), _explored(0), _expanded(0)
{
}

a_star::a_star(int size, search_node *init)
    : _size(size), _init(init), _explored(0), _expanded(0)
{
}

a_star::~a_star()
{
    std::stack<search_node *> nodes;
    search_node *temp = _init;

    while (temp)
    {
        nodes.push(temp);

        temp = temp->get_parent();
    }
    while (!nodes.empty())
    {
        temp = nodes.top();
        nodes.pop();
        delete temp;
    }
}

void a_star::solve(int option)
{
    bool solvable = is_solvable(_init);

    if (solvable)
    {
        solve_algo(option);
    }
    else
    {
        std::cout << "Unsolvable puzzle";
    }
}

void a_star::input()
{
    int t;
    std::cin >> _size;
    _init = new search_node(nullptr, _size, 0, -1);
    for (int i = 0; i < _size * _size; i++)
    {
        std::cin >> t;
        _init->add_to_grid(t);
    }
}

int CompareHamming::hamming_count(search_node *first)
{
    int f_val = first->get_movecount_from_init();
    int *f_grid = first->get_grid();
    for (int i = 0; i < first->get_size() * first->get_size(); i++)
    {
        if (!f_grid[i])
            continue;
        if (f_grid[i] != i + 1)
            f_val++;
    }
    return f_val;
}

bool CompareHamming::operator()(search_node *first, search_node *second)
{
    int f_val = hamming_count(first), s_val = hamming_count(second);

    if (f_val > s_val)
        return true;
    return false;
}

int CompareManhattan::manhattan_count(search_node *first)
{
    int f_val = first->get_movecount_from_init();
    int *f_grid = first->get_grid();

    for (int i = 0; i < first->get_size() * first->get_size(); i++)
    {
        if (!f_grid[i])
            continue;
        int correct_row = f_grid[i] / first->get_size(),
            correct_col = f_grid[i] % first->get_size(),
            curr_row = i / first->get_size(), curr_col = i % first->get_size();

        f_val += abs(correct_col - curr_col) + abs(curr_row - correct_row);
    }

    return f_val;
}

bool CompareManhattan::operator()(search_node *first, search_node *second)
{
    int f_val = manhattan_count(first), s_val = manhattan_count(second);
    if (f_val > s_val)
        return true;
    return false;
}
