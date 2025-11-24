#ifndef infinit_CLASSES_H
#define infinit_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>
#include <utility>
using namespace std;

class infinit_Board : public Board<char> {
private:
    char blank_symbol = '.';
    deque<pair<int, int>> index;

public:
    infinit_Board();
    bool update_board(Move<char>* move);
    bool is_lose(Player<char>* player) { return false; };
    bool is_win(Player<char>* player);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};

class infinit_UI : public UI<char> {
public:
    infinit_UI();
    ~infinit_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Move<char>* get_move(Player<char>* player);
};

#endif