#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "fivexfive.h"

using namespace std;

fivexfive_Board::fivexfive_Board() : Board(5, 5) {
  
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool fivexfive_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

  
    if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != blank_symbol) {
        return false;
    }

    
    n_moves++;
    board[x][y] = toupper(mark);

    return true;
}

int fivexfive_Board::count_three_in_a_row(char symbol) {
    int count = 0;
    symbol = toupper(symbol);

    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol && 
                board[i][j+1] == symbol && 
                board[i][j+2] == symbol) {
                count++;
            }
        }
    }

    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == symbol && 
                board[i+1][j] == symbol && 
                board[i+2][j] == symbol) {
                count++;
            }
        }
    }

    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol && 
                board[i+1][j+1] == symbol && 
                board[i+2][j+2] == symbol) {
                count++;
            }
        }
    }

    
    for (int i = 0; i < 3; i++) {
        for (int j = 2; j < 5; j++) {
            if (board[i][j] == symbol && 
                board[i+1][j-1] == symbol && 
                board[i+2][j-2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

int fivexfive_Board::get_score(char symbol) {
    return count_three_in_a_row(symbol);
}

bool fivexfive_Board::is_win(Player<char>* player) {

    return false;
}

bool fivexfive_Board::is_draw(Player<char>* player) {
    
    if (n_moves >= 24) {
      
        player1_score = count_three_in_a_row('X');
        player2_score = count_three_in_a_row('O');
        
        
        cout << "\n=== GAME OVER ===" << endl;
        cout << "Player X score: " << player1_score << endl;
        cout << "Player O score: " << player2_score << endl;
        
        return (player1_score == player2_score);
    }
    return false;
}

bool fivexfive_Board::game_is_over(Player<char>* player) {
    if (n_moves >= 24) {
        
        player1_score = count_three_in_a_row('X');
        player2_score = count_three_in_a_row('O');
        
        return true;
    }
    return false;
}

fivexfive_UI::fivexfive_UI() : UI<char>("Welcome to 5 x 5 Tic Tac Toe", 3) {
    cout << "Goal: Get the most three-in-a-row sequences!" << endl;
    cout << "Game ends after 24 moves (when only 1 square remains empty)" << endl;
}

Player<char>* fivexfive_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* fivexfive_UI::get_move(Player<char>* player) {
    int x, y;
    
    
    fivexfive_Board* board_ptr = static_cast<fivexfive_Board*>(player->get_board_ptr());
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol() << "), enter your move (row column) [0-4]: ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
      
        do {
            x = rand() % 5;
            y = rand() % 5;
        } while (board_ptr->get_board_matrix()[x][y] != '.');
        
        cout << "Computer (" << player->get_symbol() << ") plays at (" << x << ", " << y << ")" << endl;
    }

    return new Move<char>(x, y, player->get_symbol());
}
