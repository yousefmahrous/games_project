#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <functional>
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
    cout << "=== " << player->get_name() << "'s turn (" << player->get_symbol() << ") ===" << endl;
    display_board_matrix(player->get_board_ptr()->get_board_matrix());

    if (player->get_type() == PlayerType::HUMAN) {
        int xold, yold;
        cout << player->get_name() << ", enter the position of your token to move: ";
        cin >> xold >> yold;
        cout << player->get_name() << " selected token at (" << xold << "," << yold << ")" << endl;
        return new Move<char>(xold, yold, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        Board<char>* board_ptr = player->get_board_ptr();
        char symbol = player->get_symbol();
        vector<vector<char>> board_matrix = board_ptr->get_board_matrix();
        char opp_symbol = (symbol == 'X') ? 'O' : 'X';

        auto check_win = [](vector<vector<char>>& b, char sym) -> bool {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 2; j++) {
                    if (b[i][j] == sym && b[i][j + 1] == sym && b[i][j + 2] == sym) return true;
                }
            }
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (b[i][j] == sym && b[i + 1][j] == sym && b[i + 2][j] == sym) return true;
                }
            }
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if (b[i][j] == sym && b[i + 1][j + 1] == sym && b[i + 2][j + 2] == sym) return true;
                }
            }
            for (int i = 0; i < 2; i++) {
                for (int j = 2; j < 4; j++) {
                    if (b[i][j] == sym && b[i + 1][j - 1] == sym && b[i + 2][j - 2] == sym) return true;
                }
            }
            return false;
            };

        auto evaluate = [&](vector<vector<char>>& b) -> int {
            if (check_win(b, symbol)) return 1000;
            if (check_win(b, opp_symbol)) return -1000;

            int score = 0;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                    if (b[i][j] == symbol && b[i][j + 1] == symbol) score += 10;
                    if (b[i][j] == opp_symbol && b[i][j + 1] == opp_symbol) score -= 15;
                }
            }
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 4; j++) {
                    if (b[i][j] == symbol && b[i + 1][j] == symbol) score += 10;
                    if (b[i][j] == opp_symbol && b[i + 1][j] == opp_symbol) score -= 15;
                }
            }
            return score;
            };

        function<int(vector<vector<char>>&, int, bool)> minimax =
            [&](vector<vector<char>>& b, int depth, bool is_maximizing) -> int {

            if (check_win(b, symbol)) return 1000 - depth;
            if (check_win(b, opp_symbol)) return -1000 + depth;
            if (depth >= 3) return evaluate(b);

            char current_player = is_maximizing ? symbol : opp_symbol;
            int best_score = is_maximizing ? -9999 : 9999;

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (b[i][j] == current_player) {
                        int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };

                        for (int d = 0; d < 4; d++) {
                            int ni = i + dirs[d][0];
                            int nj = j + dirs[d][1];

                            if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4 && b[ni][nj] == '.') {
                                b[i][j] = '.';
                                b[ni][nj] = current_player;

                                int score = minimax(b, depth + 1, !is_maximizing);

                                b[ni][nj] = '.';
                                b[i][j] = current_player;

                                if (is_maximizing) {
                                    best_score = max(best_score, score);
                                }
                                else {
                                    best_score = min(best_score, score);
                                }
                            }
                        }
                    }
                }
            }

            return best_score;
            };

        int best_score = -9999;
        int best_xold = -1, best_yold = -1, best_xnew = -1, best_ynew = -1;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board_matrix[i][j] == symbol) {
                    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };

                    for (int d = 0; d < 4; d++) {
                        int ni = i + dirs[d][0];
                        int nj = j + dirs[d][1];

                        if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4 && board_matrix[ni][nj] == '.') {
                            board_matrix[i][j] = '.';
                            board_matrix[ni][nj] = symbol;

                            int score = minimax(board_matrix, 0, false);

                            board_matrix[ni][nj] = '.';
                            board_matrix[i][j] = symbol;

                            if (score > best_score) {
                                best_score = score;
                                best_xold = i;
                                best_yold = j;
                                best_xnew = ni;
                                best_ynew = nj;
                            }
                        }
                    }
                }
            }
        }

        if (best_xold != -1) {
            is_computer_move = true;
            computer_xnew = best_xnew;
            computer_ynew = best_ynew;

            cout << "Computer selected token at (" << best_xold << "," << best_yold << ")" << endl;
            cout << "Computer moves to (" << best_xnew << "," << best_ynew << ")" << endl;

            return new Move<char>(best_xold, best_yold, symbol);
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board_matrix[i][j] == symbol) {
                    return new Move<char>(i, j, symbol);
                }
            }
        }
    }

    return nullptr;
}
