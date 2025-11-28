#include <iostream>
#include <iomanip>
#include <cctype>
#include <map>
#include "pyramid.h"

using namespace std;

pyramid_Board::pyramid_Board() : Board(3, 5) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = ' ';
    board[0][2] = board[1][1] = board[1][2] = board[1][3] = board[2][0] = board[2][1] = board[2][2] = board[2][3]
        = board[2][4] = blank_symbol;
}
void pyramid_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << endl;

    // Row 0: 1 cell at position 2
    cout << "            ---" << endl;
    cout << "           | " << matrix[0][2] << " |" << endl;
    cout << "        -----------" << endl;

    // Row 1: 3 cells at positions 1, 2, 3
    cout << "       | " << matrix[1][1] << " | " << matrix[1][2] << " | " << matrix[1][3] << " |" << endl;
    cout << "    -------------------" << endl;

    // Row 2: 5 cells at positions 0, 1, 2, 3, 4
    cout << "   | " << matrix[2][0] << " | " << matrix[2][1] << " | "
        << matrix[2][2] << " | " << matrix[2][3] << " | " << matrix[2][4] << " |" << "\n";
    cout << "  -----------------------" << endl;

    // Show column indices for reference
    cout << "     0   1   2   3   4" << endl << endl;
}

bool pyramid_Board::update_board(Move<char>* move) {
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
        return true;
    }
    return false;
}

bool pyramid_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check Rows
    if (board[2][0] == sym && board[2][1] == sym && board[2][2] == sym)
        return true;

    // [2,1] [2,2] [2,3]
    if (board[2][1] == sym && board[2][2] == sym && board[2][3] == sym)
        return true;

    // [2,2] [2,3] [2,4]
    if (board[2][2] == sym && board[2][3] == sym && board[2][4] == sym)
        return true;

    // Row 1 (middle) has 1 horizontal win:
    // [1,1] [1,2] [1,3]
    if (board[1][1] == sym && board[1][2] == sym && board[1][3] == sym)
        return true;


    // Check Columns

    // Left column: [2,0] [1,1] [0,2]
    if (board[2][0] == sym && board[1][1] == sym && board[0][2] == sym)
        return true;

    // Center column: [2,2] [1,2] [0,2]
    if (board[2][2] == sym && board[1][2] == sym && board[0][2] == sym)
        return true;

    // Right column: [2,4] [1,3] [0,2]
    if (board[2][4] == sym && board[1][3] == sym && board[0][2] == sym)
        return true;


    // Check diagonals
    if ((all_equal(board[0][2], board[1][3], board[2][4]) && board[1][3] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool pyramid_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool pyramid_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

pyramid_UI::pyramid_UI() : UI<char>("Weclome to Pyramid Tic Tac Toe", 3) {}

Player<char>* pyramid_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* pyramid_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " ,Please enter your move x and y: ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
