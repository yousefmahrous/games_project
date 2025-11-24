#include <iostream>
#include <iomanip>
#include <cctype>
#include <deque>
#include <utility>
#include "infinit.h"

using namespace std;

infinit_Board::infinit_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool infinit_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3 || board[x][y] != blank_symbol) {
        return false;
    }

    n_moves++;
    board[x][y] = toupper(mark);

    index.push_back({ x, y });

    if (n_moves >= 4 && (n_moves - 1) % 3 == 0) {
        if (!index.empty()) {
            board[index.front().first][index.front().second] = blank_symbol;
            index.pop_front();
        }
    }

    return true;
}

bool infinit_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    // Check Rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == sym && board[i][1] == sym && board[i][2] == sym)
            return true;
    }

    // Check Columns
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == sym && board[1][i] == sym && board[2][i] == sym)
            return true;
    }

    // Check Diagonals
    if ((board[0][0] == sym && board[1][1] == sym && board[2][2] == sym) ||
        board[0][2] == sym && board[1][1] == sym && board[2][0] == sym) {
        return true;
    }

    return false;
}

bool infinit_Board::is_draw(Player<char>* player) {

    return (n_moves >= INT_MAX && !is_win(player));

}

bool infinit_Board::game_is_over(Player<char>* player) {

    return is_win(player) || is_draw(player);

}

infinit_UI::infinit_UI() : UI<char>("Welcome to Infinite Tic Tac Toe", 3) {}

Player<char>* infinit_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* infinit_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << ", Please enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else {
        x = rand() % 3;
        y = rand() % 3;
    }

    return new Move<char>(x, y, player->get_symbol());
}