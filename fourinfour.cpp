#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "fourinfour.h"

using namespace std;

bool is_computer_move = false;
int computer_xnew, computer_ynew;

fourinfour_Board::fourinfour_Board() : Board(4, 4) {
    // Initialize all cells
    for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) board[0][i] = 'O';
        else board[0][i] = 'X';
    }
    for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) board[3][i] = 'X';
        else board[3][i] = 'O';
    }
    for (int i = 1; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool fourinfour_Board::update_board(Move<char>* move) {
    int xold = move->get_x();
    int yold = move->get_y();
    char mark = move->get_symbol();

    // Check if the selected position contains the player's token
    if (xold < 0 || xold >= rows || yold < 0 || yold >= columns ||
        board[xold][yold] != mark) {
        cout << "Invalid move! You don't have a token at position (" << xold << "," << yold << ")" << endl;
        return false;
    }

    int xnew, ynew;

    if (is_computer_move) {
        // Computer move - use the stored new position
        xnew = computer_xnew;
        ynew = computer_ynew;
        is_computer_move = false; // Reset for next move
    }
    else {
        // Human move - get new position from input
        cout << "Enter the new position (row column): ";
        cin >> xnew >> ynew;
    }

    // Validate the new position
    if (xnew < 0 || xnew >= rows || ynew < 0 || ynew >= columns) {
        cout << "Invalid position! Position must be between 0-3 for both row and column." << endl;
        return false;
    }

    if (board[xnew][ynew] != blank_symbol) {
        cout << "Invalid move! Target position is not empty." << endl;
        return false;
    }

    // Check if the move is adjacent
    bool isAdjacent = (abs(xnew - xold) == 1 && yold == ynew) ||
        (abs(ynew - yold) == 1 && xold == xnew);

    if (!isAdjacent) {
        cout << "Invalid move! You can only move to adjacent positions (up, down, left, right)." << endl;
        return false;
    }

    // Perform the move
    board[xold][yold] = blank_symbol;
    board[xnew][ynew] = mark;
    n_moves++;

    return true;
}

bool fourinfour_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    // Check Horizontally
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            if (board[i][j] == sym && board[i][j + 1] == sym && board[i][j + 2] == sym) {
                return true;
            }
        }
    }

    // Check Vertically
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == sym && board[i + 1][j] == sym && board[i + 2][j] == sym) {
                return true;
            }
        }
    }

    // Check Diagonal (top left - bottom right)
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (board[i][j] == sym && board[i + 1][j + 1] == sym && board[i + 2][j + 2] == sym) {
                return true;
            }
        }
    }

    // Check Diagonal (top right - bottom left)
    for (int i = 0; i < 2; i++) {
        for (int j = 2; j < 4; j++) {
            if (board[i][j] == sym && board[i + 1][j - 1] == sym && board[i + 2][j - 2] == sym) {
                return true;
            }
        }
    }

    return false;
}

bool fourinfour_Board::is_draw(Player<char>* player) {
    return n_moves >= 50;
}

bool fourinfour_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

fourinfour_UI::fourinfour_UI() : UI<char>("Welcome to 4 x 4 Tic Tac Toe", 3) {}

Player<char>* fourinfour_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* fourinfour_UI::get_move(Player<char>* player) {
    // Show whose turn it is
    cout << "=== " << player->get_name() << "'s turn (" << player->get_symbol() << ") ===" << endl;

    // Show current board
    display_board_matrix(player->get_board_ptr()->get_board_matrix());

    if (player->get_type() == PlayerType::HUMAN) {
        int xold, yold;
        cout << player->get_name() << ", enter the position of your token to move: ";
        cin >> xold >> yold;

        // Show what the human is doing
        cout << player->get_name() << " selected token at (" << xold << "," << yold << ")" << endl;

        return new Move<char>(xold, yold, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {

        Board<char>* board_ptr = player->get_board_ptr();
        char symbol = player->get_symbol();
        vector<vector<char>> board_matrix = board_ptr->get_board_matrix();

        // Find all computer tokens
        vector<pair<int, int>> computer_tokens;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board_matrix[i][j] == symbol) {
                    computer_tokens.push_back({ i, j });
                }
            }
        }

        if (computer_tokens.empty()) {
            return new Move<char>(0, 0, symbol);
        }

        // Try to find a valid move
        for (int attempts = 0; attempts < 100; attempts++) {
            // Pick a random computer token
            int token_index = rand() % computer_tokens.size();
            int xold = computer_tokens[token_index].first;
            int yold = computer_tokens[token_index].second;

            // Define possible directions
            int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

            // Shuffle directions for randomness
            for (int i = 0; i < 4; i++) {
                int swapWith = rand() % 4;
                int tempX = directions[i][0];
                int tempY = directions[i][1];
                directions[i][0] = directions[swapWith][0];
                directions[i][1] = directions[swapWith][1];
                directions[swapWith][0] = tempX;
                directions[swapWith][1] = tempY;
            }

            // Try each direction
            for (int d = 0; d < 4; d++) {
                int xnew = xold + directions[d][0];
                int ynew = yold + directions[d][1];

                if (xnew >= 0 && xnew < 4 && ynew >= 0 && ynew < 4 &&
                    board_matrix[xnew][ynew] == '.') {

                    // Store the computer's move
                    is_computer_move = true;
                    computer_xnew = xnew;
                    computer_ynew = ynew;

                    cout << "Computer selected token at (" << xold << "," << yold << ")" << endl;
                    cout << "Computer moves to (" << xnew << "," << ynew << ")" << endl;

                    return new Move<char>(xold, yold, symbol);
                }
            }
        }

        // Fallback
        return new Move<char>(computer_tokens[0].first, computer_tokens[0].second, symbol);
    }

    return nullptr;
}