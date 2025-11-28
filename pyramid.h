#ifndef pyramid_CLASSES_H
#define pyramid_CLASSES_H

#include "BoardGame_Classes.h"

using namespace std;

class pyramid_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:

    pyramid_Board();

    bool update_board(Move<char>* move);

    bool is_lose(Player<char>* player) { return false; }

    bool is_win(Player<char>*);

    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);

};

class pyramid_UI : public UI<char> {
public:
    pyramid_UI();
    ~pyramid_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Move<char>* get_move(Player<char>* player);

    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif
