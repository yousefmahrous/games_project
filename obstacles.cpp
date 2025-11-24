#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "obstacles.h"

using namespace std;


obstacles_Board::obstacles_Board() : Board(6, 6) {
    // Initialize all cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool obstacles_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) {
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(mark);
        }

        if (n_moves != 0 && n_moves % 2 == 0) {
            int l = 0;
            while (l < 2) {
                int xrand = rand() % 6;
                int yrand = rand() % 6;
                if (board[xrand][yrand] == blank_symbol) {
                    board[xrand][yrand] = '#';
                    l++;
                }
            }
        }
        return true;
    }
    return false;
}

bool obstacles_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
    auto check = [&](int x, int y, int dx, int dy) {
        for (int k = 0; k < 4; k++) {
            int xx = x + dx * k;
            int yy = y + dy * k;

            if (xx < 0  || xx >= 6 ||  yy < 0 || yy >= 6)
                return false;

            if (board[xx][yy] != sym)
                return false;
        }
        return true;
        };
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++) {
            if (check(i, j, 1, 0)) return true;  // vertical
            if (check(i, j, 0, 1)) return true;  // horizontal
            if (check(i, j, 1, 1)) return true;  // diag ↘
            if (check(i, j, 1, -1)) return true; // diag ↙
        }

    return false;
}

bool obstacles_Board::is_draw(Player<char>* player) {
    return (n_moves == 18 && !is_win(player));
}

bool obstacles_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

obstacles_UI::obstacles_UI() : UI<char>("Welcome to Obstacles Tic Tac Toe", 3) {}

Player<char>* obstacles_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* obstacles_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " ,Please enter your move x and y (0 to 5): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());

    return nullptr;
}