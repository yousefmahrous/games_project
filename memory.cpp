#include <iostream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <algorithm>
#include "memory.h"

using namespace std;

memory_Board::memory_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool memory_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {

        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }
    return false;
}

bool memory_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool memory_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool memory_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

memory_UI::memory_UI() : UI<char>("Welcome to Memory Tic Tac Toe", 3) {
    game_ended = false;
}

Player<char>* memory_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

// Making strategic moves
vector<pair<int, int>> get_available_moves(Board<char>* board) {
    vector<pair<int, int>> moves;
    for (int i = 0; i < board->get_rows(); i++) {
        for (int j = 0; j < board->get_columns(); j++) {
            if (board->get_cell(i, j) == '.') {
                moves.push_back({ i, j });
            }
        }
    }
    return moves;
}

// check if move creates a win
bool would_win(Board<char>* board, int x, int y, char symbol) {
    // Create a simple check
    vector<vector<char>> temp_board(3, vector<char>(3, '.'));

    // Copy current board
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp_board[i][j] = board->get_cell(i, j);
        }
    }

    // Make the move
    temp_board[x][y] = symbol;

    // Check rows
    for (int i = 0; i < 3; i++) {
        if (temp_board[i][0] == symbol &&
            temp_board[i][1] == symbol &&
            temp_board[i][2] == symbol) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (temp_board[0][j] == symbol &&
            temp_board[1][j] == symbol &&
            temp_board[2][j] == symbol) {
            return true;
        }
    }

    // Check diagonals
    if (temp_board[0][0] == symbol &&
        temp_board[1][1] == symbol &&
        temp_board[2][2] == symbol) {
        return true;
    }

    if (temp_board[0][2] == symbol &&
        temp_board[1][1] == symbol &&
        temp_board[2][0] == symbol) {
        return true;
    }

    return false;
}

Move<char>* memory_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << ", Please enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else {
        Board<char>* board = player->get_board_ptr();
        char ai_symbol = player->get_symbol();
        char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

        // Get all available moves
        auto available_moves = get_available_moves(board);

        // Strategy 1: Try to win
        for (auto move : available_moves) {
            if (would_win(board, move.first, move.second, ai_symbol)) {
                x = move.first;
                y = move.second;
                return new Move<char>(x, y, player->get_symbol());
            }
        }

        // Strategy 2: Try to block opponent
        for (auto move : available_moves) {
            if (would_win(board, move.first, move.second, opponent_symbol)) {
                x = move.first;
                y = move.second;
                return new Move<char>(x, y, player->get_symbol());
            }
        }

        // Strategy 3: Take center if available
        if (board->get_cell(1, 1) == '.') {
            x = 1; y = 1;
            return new Move<char>(x, y, player->get_symbol());
        }

        // Strategy 4: Take corners
        vector<pair<int, int>> corners = { {0, 0}, {0, 2}, {2, 0}, {2, 2} };
        for (auto corner : corners) {
            if (board->get_cell(corner.first, corner.second) == '.') {
                x = corner.first;
                y = corner.second;
                return new Move<char>(x, y, player->get_symbol());
            }
        }

        // Strategy 5: Take any available move
        if (!available_moves.empty()) {
            x = available_moves[0].first;
            y = available_moves[0].second;
        }
        else {
            x = rand() % 3;
            y = rand() % 3;
        }
    }

    return new Move<char>(x, y, player->get_symbol());
}

void memory_UI::display_board_matrix(const vector<vector<char>>& matrix) const {

    cout << "\n    ";
    for (int j = 0; j < 3; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * 3, '-') << endl;

    if (game_ended) {
        display_revealed_board(matrix);
        return;
    }

    for (int i = 0; i < 3; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < 3; ++j) {
            if (matrix[i][j] == '.') {
                cout << setw(cell_width) << '.' << " |";
            }
            else {
                cout << setw(cell_width) << '#' << " |";
            }
        }
        cout << "\n   " << string((cell_width + 2) * 3, '-') << endl;
    }
    cout << endl;

}

void memory_UI::display_revealed_board(const vector<vector<char>>& matrix) const {

    cout << "\n    ";
    for (int j = 0; j < 3; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * 3, '-') << endl;

    // Show actual board with marks
    for (int i = 0; i < 3; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < 3; ++j) {
            cout << setw(cell_width) << matrix[i][j] << " |";
        }
        cout << "\n   " << string((cell_width + 2) * 3, '-') << endl;
    }
    cout << endl;

}