#include "game.h"

void game::init_board()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (!(i % 7))
            _board[i] = 0;
        else
            _board[i] = 4;
    }
}

bool game::is_end()
{
    bool ended = true;
    for (int i = 1; i < 7; i++)
    {
        if (_board[i])
            ended = false;
    }
    if (ended)
        return true;
    ended = true;
    for (int i = 8; i < 14; i++)
    {
        if (_board[i])
            ended = false;
    }
    return ended;
}

int game::get_move(int player_num)
{

    if (_curr_player == P1)
    {
        return _p1->make_move(P1, _board, _p2);
    }
    else
    {
        return _p2->make_move(P2, _board, _p1);
    }
}

int game::perform_move(int chosen_pit)
{
    // do the move
    int curr_pit = chosen_pit;
    while (_board[chosen_pit]--)
    {
        // wrap over for counter clockwise rotation
        curr_pit = (curr_pit + 1) % BOARD_SIZE;
        // skip 0th cell
        if (!curr_pit)
            ++curr_pit;
        _board[curr_pit]++;

        if (curr_pit == P1_STORAGE)
        {
            _p1->add_storage_stones(1);
        }
        else if (curr_pit == P2_STORAGE)
        {
            _p2->add_storage_stones(1);
        }
    }
    // this cell goes negative, turn it back to 0
    _board[chosen_pit] = 0;

    return curr_pit;
}

void game::capture(int last_pit)
{
    if (_curr_player == P1)
    {
        if (last_pit < P1_STORAGE && _board[last_pit] == 1 && _board[BOARD_SIZE - 1 - last_pit])
        {
            _board[P1_STORAGE] += _board[last_pit] +
                                  _board[BOARD_SIZE - 1 - last_pit];
            _p1->add_storage_stones(
                _board[last_pit] + _board[BOARD_SIZE - 1 - last_pit]);

            _p1->add_captured_stones(_board[BOARD_SIZE - 1 - last_pit]);

            _board[BOARD_SIZE - 1 - last_pit] = _board[last_pit] = 0;
        }
    }
    else
    {
        if (last_pit > P1_STORAGE && last_pit < P2_STORAGE && _board[last_pit] == 1 && _board[BOARD_SIZE - 1 - last_pit])
        {
            _board[P2_STORAGE] += _board[last_pit] +
                                  _board[BOARD_SIZE - 1 - last_pit];
            _p2->add_storage_stones(
                _board[last_pit] + _board[BOARD_SIZE - 1 - last_pit]);

            _p2->add_captured_stones(_board[BOARD_SIZE - 1 - last_pit]);

            _board[BOARD_SIZE - 1 - last_pit] = _board[last_pit] = 0;
        }
    }
}

int game::next_player(int curr_pit)
{

    // check if last stone was put in storage
    if (curr_pit == P1_STORAGE && _curr_player == P1)
    {
        _p1->add_extra_moves(1);
        return P1;
    }
    else if (curr_pit == P2_STORAGE && _curr_player == P2)
    {
        _p2->add_extra_moves(1);
        return P2;
    }
    else
    {
        // capture if possible
        capture(curr_pit);
        // return opposite player num
        return 1 - _curr_player;
    }
}

void game::end_cleanup()
{
    int p1_playing_stones = 0, p2_playing_stones = 0;
    for (int i = 1; i < 7; i++)
        p1_playing_stones += _board[i], p2_playing_stones += _board[i + 7];

    _p2->add_storage_stones(p2_playing_stones);
    _p1->add_storage_stones(p1_playing_stones);

    for (int i = 0; i < 15; i++)
    {
        if (!(i % 7))
            continue;
        _board[i] = 0;
    }
}

void game::decl_win()
{
    player *winner, *loser;
    if (_p1->get_storage_stones() == _p2->get_storage_stones())
    {
        std::cout << "\t\t\t\tGame drawn\n";
    }
    else
    {
        if (_p1->get_storage_stones() > _p2->get_storage_stones())
            winner = _p1, loser = _p2;
        else if (_p1->get_storage_stones() < _p2->get_storage_stones())
            winner = _p2, loser = _p1;
        std::cout << "\t\t\t\t" << winner->get_name() << " wins against "
                  << loser->get_name() << "\n";
        std::cout << "\t\t\t\tScore: " << winner->get_storage_stones() << " to "
                  << loser->get_storage_stones() << "\n";
    }
}

game::game() : _board(new int[BOARD_SIZE])
{
    init_board();
}

game::game(player *p1, player *p2) : _board(new int[BOARD_SIZE]), _p1(p1), _p2(p2),
                                     _curr_player(P1)
{
    init_board();
}

game::~game()
{
    delete[] _board;
    delete _p1;
    delete _p2;
}

void game::run()
{
    while (!is_end())
    {
        show();
        int mv = get_move(_curr_player);
        int curr_pit = perform_move(mv);
        _curr_player = next_player(curr_pit);
    }
    end_cleanup();
    show();
    decl_win();
}

void game::show()
{
    std::cout << "\t\t\t\t";
    for (int i = 0; i < 25; i++)
        std::cout << "*";
    std::cout << "\n\t\t\t\t*  |";

    for (int i = BOARD_SIZE - 2; i >= 8; i--)
        std::cout << std::setw(2) << _board[i] << "|";
    std::cout << "  *\n\t\t\t\t*" << std::setw(2) << _board[BOARD_SIZE - 1] << "|";

    for (int i = 1; i < 18; i++)
        std::cout << " ";

    std::cout << "|" << std::setw(2) << _board[7] << "*\n\t\t\t\t*  |";

    for (int i = 1; i < 7; i++)
        std::cout << std::setw(2) << _board[i] << "|";
    std::cout << "  *\n\t\t\t\t";
    for (int i = 0; i < 25; i++)
        std::cout << "*";
    std::cout << "\n";
}

int player::get_user_input(int player_num)
{
    int cell = 0, start = 1, end = 6;

    while (!(cell >= start && cell <= end))
    {
        std::cout << "\t\t\t\tPLAYER: " << player_num << " HUMAN: " << _name
                  << " \n\t\t\t\t(choose a cell between " << start << " and "
                  << end << ")\n\t\t\t\t";
        std::cin >> cell;
    }

    if (player_num)
    {
        cell = (BOARD_SIZE - 1 - cell);
    }
    return cell;
}

player::player() : _storage_stones(0), _captured_stones(0),
                   _extra_moves_earned(0), _name(nullptr), max(false), hum(true),
                   weights(new double[WEIGHT_COUNT]), player_max_depth(12)
{
}

player::player(const player &src)
{
    this->_storage_stones = src._storage_stones;
    this->_captured_stones = src._captured_stones;
    this->h = src.h;
    this->hum = src.hum;
    this->max = src.max;
    this->_extra_moves_earned = src._extra_moves_earned;
    this->weights = new double[4];
    for (int i = 0; i < 4; i++)
        this->weights[i] = src.weights[i];
}

player::player(char *name) : _storage_stones(0), _captured_stones(0),
                             _extra_moves_earned(0), _name(name), max(false),
                             hum(true), weights(new double[WEIGHT_COUNT]),
                             player_max_depth(12)
{
}

player::player(char *name, int depth)
    : _storage_stones(0), _captured_stones(0),
      _extra_moves_earned(0), _name(name), max(false),
      hum(true), weights(new double[WEIGHT_COUNT]),
      player_max_depth(depth)
{
}

player::player(char *name, bool is_human, bool is_max_player, HEURISTICS _h)
    : _storage_stones(0), _captured_stones(0),
      _extra_moves_earned(0), _name(name), max(is_max_player),
      hum(is_human), weights(new double[WEIGHT_COUNT]), h(_h),
      player_max_depth(ALGO_DEPTH)
{
}

player::~player()
{
    delete _name;
    delete[] weights;
}

int player::make_move(int player_num, int *board, player *opponent)
{
    if (hum)
    {
        return get_user_input(player_num);
    }
    else
    {
        std::cout << "PLAYER " << player_num << " AI: " << _name << " making move:\n";
        int alpha = INT32_MIN, beta = INT32_MAX;
        int chosen_cell = -1;

        struct state *curr_game_state = new state(this, opponent, board);
        curr_game_state->is_max = max, curr_game_state->curr_player = player_num;

        std::vector<struct state *> children = generate_children(curr_game_state);

        for (int move = 1; move <= children.size(); move++)
        {
            int value = minmax_prune(children[move - 1], 0, h, alpha, beta);
            if (max)
            {
                // update alpha
                if (alpha < value)
                {
                    chosen_cell = children[move - 1]->chosen_cell ;
                    alpha = value;
                }
            }
            else
            {
                if (beta > value)
                {
                    chosen_cell = children[move - 1]->chosen_cell ;
                    beta = value;
                }
            }
        }
        std::cout << chosen_cell << "\n";
        // delete curr_game_state;
        return chosen_cell;
    }
}

int player::get_storage_stones()
{
    return _storage_stones;
}

void player::set_storage_stones(int storage_stones)
{
    _storage_stones = storage_stones;
}

void player::add_storage_stones(int new_stones)
{
    _storage_stones += new_stones;
}

int player::get_captured_stones()
{
    return _captured_stones;
}

void player::set_captured_stones(int captured_stones)
{
    _captured_stones = captured_stones;
}

void player::add_captured_stones(int new_stones)
{
    _captured_stones += new_stones;
}

int player::get_extra_moves()
{
    return _extra_moves_earned;
}

void player::set_extra_moves(int extra_moves)
{
    _extra_moves_earned = extra_moves;
}

void player::add_extra_moves(int new_moves)
{
    _extra_moves_earned += new_moves;
}

char *player::get_name()
{
    return _name;
}

void player::set_name(char *name)
{
    _name = name;
}

bool player::is_max()
{
    return max;
}

void player::set_is_max(bool is_max)
{
    max = is_max;
}

bool player::is_hum()
{
    return hum;
}

void player::set_is_hum(bool is_hum)
{
    hum = is_hum;
}

double *player::get_weights()
{
    return weights;
}

void player::set_weights(double *given_weights)
{
    for (int i = 0; i < WEIGHT_COUNT; i++)
    {
        weights[i] = given_weights[i];
    }
}

int player::get_max_depth()
{
    return player_max_depth;
}

void player::set_max_depth(int depth)
{
    player_max_depth = depth;
}

std::vector<struct state *> generate_children(struct state *game_state)
{

    std::vector<struct state *> children;
    bool is_done[7] = {0};
    int start = 1, end = 6;
    if (game_state->curr_player)
        start += 7, end += 7;

    // for (int i = start; i <= end; i++)
    // {
    //     if (is_capture(game_state, i))
    //     {
    //         children.push_back(perform_move(game_state, i));

    //         is_done[i % 7] = true;
    //     }
    // }
    for (int i = start; i <= end; i++)
    {
        // if (!is_done[i % 7])
        if (game_state->board[i])
            children.push_back(perform_move(game_state, i));
    }
    return children;
}

bool is_capture(struct state *game_state, int start)
{
    int end = (start + game_state->board[start]) % BOARD_SIZE;
    if (!end)
        end++;
    if (!game_state->board[end] && game_state->board[BOARD_SIZE - 1 - end])
        return true;
    return false;
}

bool is_ended(struct state *curr_game_state)
{
    bool ended = true;
    for (int i = 1; i < 7; i++)
    {
        if (curr_game_state->board[i])
            ended = false;
    }
    if (ended)
        return true;
    ended = true;
    for (int i = 8; i < 14; i++)
    {
        if (curr_game_state->board[i])
            ended = false;
    }
    return ended;
}

struct state *perform_move(struct state *game_state, int start)
{
    player *p1 = new player(*(game_state->p1)), *p2 = new player(*(game_state->p2));
    // struct state *temp = new state(game_state->p1, game_state->p2, new int[15]);
    struct state *temp = new state(p1, p2, new int[15]);
    temp->chosen_cell = start;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        temp->board[i] = game_state->board[i];
    }

    int curr_pit = start;

    while (temp->board[start]--)
    {
        // wrap over for counter clockwise rotation
        curr_pit = (curr_pit + 1) % BOARD_SIZE;
        // skip 0th cell
        if (!curr_pit)
            ++curr_pit;
        temp->board[curr_pit]++;
    }
    // this cell goes negative, turn it back to 0
    temp->board[start] = 0;

    // if capture move increase captured count
    if (is_capture(temp, start))
    {
        int captured = temp->board[curr_pit] + temp->board[BOARD_SIZE - 1 - curr_pit];
        temp->board[curr_pit] = temp->board[BOARD_SIZE - 1 - curr_pit] = 0;
        if (game_state->curr_player)
        {
            temp->p2->add_captured_stones(captured);
            temp->board[P2_STORAGE] += captured;
        }
        else
        {
            temp->p1->add_captured_stones(captured);
            temp->board[P1_STORAGE] += captured;
        }

        temp->curr_player = 1 - game_state->curr_player;
        temp->is_max = game_state->is_max ? MIN : MAX;
    }
    else
    {
        if (curr_pit == P1_STORAGE && game_state->curr_player == P1)
        {
            temp->curr_player = game_state->curr_player;
            temp->is_max = game_state->is_max;
            temp->p1->add_extra_moves(1);
        }
        else if (curr_pit == P2_STORAGE && game_state->curr_player == P2)
        {
            temp->curr_player = game_state->curr_player;
            temp->is_max = game_state->is_max;
            temp->p2->add_extra_moves(1);
        }
        else
        {
            temp->curr_player = 1 - game_state->curr_player;
            temp->is_max = game_state->is_max ? MIN : MAX;
        }
    }

    return temp;
}

int minmax_prune(struct state *games_state, int depth, HEURISTICS h, int alpha, int beta)
{
    if (depth == ALGO_DEPTH || is_ended(games_state))
    {
        switch (h)
        {
        case H1:
            /* code */
            return h1(games_state);
            break;
        case H2:
            return h2(games_state);
            break;
        case H3:
            return h3(games_state);
            break;
        case H4:
            return h4(games_state);
            break;
        default:
            return 0;
            break;
        }
    }
    else
    {
        int value = 0;
        std::vector<struct state *> children = generate_children(games_state);
        for (int child = 0; child < children.size(); child++)
        {
            value = minmax_prune(children[child], depth + 1, h, alpha, beta);
            // update alpha value for max node
            if (games_state->is_max && alpha < value)
                alpha = value;
            // update beta value for min node
            else if (!games_state->is_max && beta > value)
                beta = value;

            // stop recursive calling on further children,
            // max parent won't take decreasing beta value anyway
            // similarly min parent won't take increasing alpha value
            if (alpha >= beta)
                break;
        }

        return value;
    }
}

inline int h1(struct state *game_state)
{
    int my_storage = !game_state->curr_player ? P1_STORAGE : P2_STORAGE,
        opponent_storage = !game_state->curr_player ? P2_STORAGE : P1_STORAGE;
    return game_state->board[my_storage] - game_state->board[opponent_storage];
}

inline int h2(struct state *game_state)
{
    double w1 = 0, w2 = 0;
    if (!game_state->curr_player)
        w1 = game_state->p1->get_weights()[0], w2 = game_state->p1->get_weights()[1];
    else
        w1 = game_state->p2->get_weights()[0], w2 = game_state->p2->get_weights()[1];

    int my_side_count = 0, opponent_side_count = 0;

    for (int i = 1; i < BOARD_SIZE / 2; i++)
    {
        if (!game_state->curr_player)
            opponent_side_count += game_state->board[BOARD_SIZE - i - 1];
        else
            opponent_side_count += game_state->board[i];
    }

    my_side_count = 48 - game_state->board[P1_STORAGE] - game_state->board[P2_STORAGE] - opponent_side_count;

    return w1 * h1(game_state) + w2 * (my_side_count - opponent_side_count);
}

inline int h3(struct state *game_state)
{
    double w3 = 0;
    int additional_move_earned = 0;
    if (!game_state->curr_player)
        w3 = game_state->p1->get_weights()[2], additional_move_earned = game_state->p1->get_extra_moves();
    else
        w3 = game_state->p2->get_weights()[2], additional_move_earned = game_state->p2->get_extra_moves();

    return h2(game_state) + w3 * (additional_move_earned);
}

inline int h4(struct state *game_state)
{
    double w4 = 0;
    int captured_stones = 0;
    if (!game_state->curr_player)
        w4 = game_state->p1->get_weights()[3], captured_stones = game_state->p1->get_captured_stones();
    else
        w4 = game_state->p2->get_weights()[3], captured_stones = game_state->p2->get_captured_stones();

    return h3(game_state) + w4 * (captured_stones);
}
