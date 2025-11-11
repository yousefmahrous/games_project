#ifndef misere_CLASSES_H
#define misere_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

class misere_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:

    misere_Board();

    bool update_board(Move<char>* move);

    bool is_lose(Player<char>* player);

    bool is_win(Player<char>*) { return false; };

    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);
};


class misere_UI : public UI<char> {
public:
    misere_UI();

    ~misere_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);


    virtual Move<char>* get_move(Player<char>* player);
};

#endif
