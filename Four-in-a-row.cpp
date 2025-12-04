#include <iostream>
#include <iomanip>
#include <cctype>
#include "Four-in-a-row.h"

using namespace std;

Four_in_a_row_Board::Four_in_a_row_Board() : Board(6, 7) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

int Four_in_a_row_Board::get_lowest_empty_row(int col) {
    // Start from the bottom row and go up
    for (int row = rows - 1; row >= 0; --row) {
        if (board[row][col] == blank_symbol) {
            return row;
        }
    }
    return -1; // Column is full
}

bool Four_in_a_row_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char mark = move->get_symbol();

    // Validate column
    if (col < 0 || col >= columns) {
        return false;
    }

    // Find the lowest empty row in this column
    int row = get_lowest_empty_row(col);
    if (row < 0) {
        return false; 
    }

    // Check if this is an undo move (mark == 0)
    if (mark == 0) {
        // For undo, we need to find the highest occupied cell in this column
        for (int r = 0; r < rows; ++r) {
            if (board[r][col] != blank_symbol) {
                board[r][col] = blank_symbol;
                n_moves--;
                return true;
            }
        }
        return false;
    }

    board[row][col] = toupper(mark);
    n_moves++;
    return true;
}

bool Four_in_a_row_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns - 3; ++col) {
            if (board[row][col] == sym &&
                board[row][col + 1] == sym &&
                board[row][col + 2] == sym &&
                board[row][col + 3] == sym) {
                return true;
            }
        }
    }

    for (int row = 0; row < rows - 3; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (board[row][col] == sym &&
                board[row + 1][col] == sym &&
                board[row + 2][col] == sym &&
                board[row + 3][col] == sym) {
                return true;
            }
        }
    }

    for (int row = 0; row < rows - 3; ++row) {
        for (int col = 0; col < columns - 3; ++col) {
            if (board[row][col] == sym &&
                board[row + 1][col + 1] == sym &&
                board[row + 2][col + 2] == sym &&
                board[row + 3][col + 3] == sym) {
                return true;
            }
        }
    }

    for (int row = 0; row < rows - 3; ++row) {
        for (int col = 3; col < columns; ++col) {
            if (board[row][col] == sym &&
                board[row + 1][col - 1] == sym &&
                board[row + 2][col - 2] == sym &&
                board[row + 3][col - 3] == sym) {
                return true;
            }
        }
    }

    return false;
}

bool Four_in_a_row_Board::is_lose(Player<char>* player) {
    return false;
}

bool Four_in_a_row_Board::is_draw(Player<char>* player) {
    return (n_moves == 42 && !is_win(player));
}

bool Four_in_a_row_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Four_in_a_row_UI::Four_in_a_row_UI() : UI<char>("Welcome to Four-in-a-row", 8) {}

Player<char>* Four_in_a_row_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Four_in_a_row_UI::get_move(Player<char>* player) {
    int col;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << ", please enter a column (0 to 6): ";
        cin >> col;
        return new Move<char>(0, col, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        col = rand() % player->get_board_ptr()->get_columns();
        return new Move<char>(0, col, player->get_symbol());
    }

    return nullptr;
}
