#ifndef memory_CLASSES_H
#define memory_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>
using namespace std;

class memory_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    memory_Board();
    bool update_board(Move<char>* move);
    bool is_lose(Player<char>* player) { return false; }
    bool is_win(Player<char>*);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};


class memory_UI : public UI<char> {
private:
    bool game_ended = false;

public:
    memory_UI();
    ~memory_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);

    // Override the display function to show hidden board during game
    void display_board_matrix(const vector<vector<char>>& matrix) const override;

    // Function to display revealed board at the end
    void display_revealed_board(const vector<vector<char>>& matrix) const;

    // Function to mark game as ended
    void set_game_ended(bool ended) { game_ended = ended; }
};

#endif