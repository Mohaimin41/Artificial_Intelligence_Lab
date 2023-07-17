#include "game.h"

using namespace std;

int main()
{
    double *weights = new double[4];
    player *p1, *p2;
    
    cout << "Player 1: ";
    cout << "AI? (y/n): ";
    char c;
    cin >> c;
    if (c == 'y')
    {
        cout << "Heuristic: (1 to 4): ";
        char h;
        cin >> h;

        for (int i = 0; i < 4; i++)
        {
            cout << "W" << i + 1<< " :\n";
            double t;
            cin >> t;
            weights[i] = t;
        }

        p1 = new player((char *)"AI_1", false, true, HEURISTICS(h - '0'));
        p1->set_weights(weights);
    }
    else
    {

        p1 = new player((char *)"human_1", ALGO_DEPTH);
    }


    cout << "Player 2: ";
    cout << "AI? (y/n): ";
    cin >> c;
    if (c == 'y')
    {
        cout << "Heuristic: (1 to 4): ";
        char h;
        cin >> h;

        for (int i = 0; i < 4; i++)
        {
            cout << "W" << i + 1<< " :\n";
            double t;
            cin >> t;
            weights[i] = t;
        }

        p2 = new player((char *)"AI_2", false, false, HEURISTICS(h - '0'));
        p2->set_weights(weights);
    }
    else
    {

        p2 = new player((char *)"human_2", ALGO_DEPTH);
    }
    game *g = new game(p1, p2);
    // g->show();
    g->run();
}