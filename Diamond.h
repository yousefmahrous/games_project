#ifndef Diamond_CLASSES_H
#define Diamond_CLASSES_H

#include "BoardGame_Classes.h"
 
#include <vector>
using namespace std;

class Diamond_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:

    Diamond_Board();

    bool update_board(Move<char>* move);

    bool is_lose(Player<char>* player) { return false; }

    bool is_win(Player<char>*);

    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);

    bool check_line(int x, int y, int dir, int length, char sym);

};

class Diamond_UI : public UI<char> {
private:
    char blank_symbol = '.';
public:
    Diamond_UI();
    ~Diamond_UI() {};


    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Move<char>* get_move(Player<char>* player);

    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif
