#include <iostream>
#include <iomanip>
#include <vector>

#define BOARD_SIZE 15
#define P1_STORAGE 7
#define P2_STORAGE 14
#define P1 0
#define P2 1
#define WEIGHT_COUNT 4
#define MAX true
#define MIN false
#define ALGO_DEPTH 6

enum HEURISTICS
{
    H1,
    H2,
    H3,
    H4
};
class player
{
private:
    int _storage_stones;
    int _captured_stones;
    int _extra_moves_earned;
    char *_name;
    bool max;
    bool hum;
    double *weights;
    HEURISTICS h;
    int player_max_depth;

    /**
     * @brief return the cell user chooses to play
     * @param int the player num
     */
    int get_user_input(int);

public:
    player();
    player(const player&);
    player(char *);
    player(char *, int);
    player(char *, bool, bool, HEURISTICS);
    ~player();
    /**
     * @brief return the cell chosen to be played by AI or human player
     *
     * @param int the player num
     * @param int* the game board array
     */
    int make_move(int, int *, player *);
    int get_storage_stones();
    void set_storage_stones(int);
    void add_storage_stones(int);
    int get_captured_stones();
    void set_captured_stones(int);
    /**
     * @brief add to stones captured by this player with the capturing rule
     *
     */
    void add_captured_stones(int);
    int get_extra_moves();
    void set_extra_moves(int);
    /**
     * @brief add to the number of extra moves this player has won
     *
     */
    void add_extra_moves(int);
    char *get_name();
    void set_name(char *);
    bool is_max();
    /**
     * @brief set whether AI player is MAX or MIN, true for MAX
     *
     */
    void set_is_max(bool);
    bool is_hum();
    /**
     * @brief Set whether the player is AI
     *
     */
    void set_is_hum(bool);
    double *get_weights();
    void set_weights(double *);
    int get_max_depth();
    void set_max_depth(int);
};

struct state
{
    player *p1;
    player *p2;
    int *board;
    int curr_player;
    bool is_max;
    int chosen_cell;
    state(player *_p1, player *_p2, int *_board)
    {
        p1 = _p1, p2 = _p2, board = _board;
    }
    ~state()
    {
        delete[] board;
    }
};

std::vector<struct state *> generate_children(struct state *);
bool is_capture(struct state *, int);
bool is_ended(struct state *);
struct state *perform_move(struct state *, int);
/**
     @brief  do alpha beta pruning search
     @param  game_state    current game state struct
     @param  depth  current depth
     @param  h  Heuristics to use
     @param  alpha  current alpha value
     @param  beta   current beta value
     @return int    alpha or beta value
     */
int minmax_prune(struct state *, int, HEURISTICS, int, int);
/* heuristics */
inline int h1(struct state *);
inline int h2(struct state *);
inline int h3(struct state *);
inline int h4(struct state *);

void set_global_depth(int);
int get_global_depth();

class game
{
private:
    int *_board;
    player *_p1;
    player *_p2;
    int _curr_player;
    /**
     * @brief game inititating setup
     *
     */
    void init_board();
    /**
     * @brief  returns the cell chosen to be played by player
     * @param int player_num, used to verify cell range,
     * 0 for player 1, 1 for player 2
     */
    int get_move(int);
    /**
     * @brief performs move in board, returning the cell number of last moved
     * stone
     */
    int perform_move(int);
    /**
     * @brief perform capturing if possible, updates player stone counts too
     */
    void capture(int);
    /**
     * @brief return the next player num, accounting for extra move rule
     *
     * @return int 0 for player 1, 1 for player 2
     */
    int next_player(int);
    /**
     * @brief check if any side has 6 empty cells, return true if so
     */
    bool is_end();
    /**
     * @brief moves all stones to respective storage at end of game
     */
    void end_cleanup();
    /**
     * @brief prints winner with stone counts
     */
    void decl_win();

public:
    game();
    game(player *p1, player *p2);
    ~game();
    void run();

    void show();
};
