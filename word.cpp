#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include "word.h"

using namespace std;

Word_Board::Word_Board() : Board(3, 3) {
    
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    
    load_dictionary();
}

void Word_Board::load_dictionary() {
    ifstream file("dic.txt");
    string word;
    
    while (file >> word) {
        
        transform(word.begin(), word.end(), word.begin(), ::toupper);
        if (word.length() == 3) {
            dictionary.insert(word);
        }
    }
    file.close();
}

bool Word_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    
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

bool Word_Board::check_word(int row, int col, int dx, int dy) {
    string word = "";
    
    
    for (int i = 0; i < 3; i++) {
        int newRow = row + i * dx;
        int newCol = col + i * dy;
        
        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= columns)
            return false;
            
        if (board[newRow][newCol] == blank_symbol)
            return false;
            
        word += board[newRow][newCol];
    }
    
    
    return dictionary.count(word) > 0;
}

bool Word_Board::is_win(Player<char>* player) {
    // Check all possible starting positions for 3-letter words
    
    // Check rows (horizontal)
    for (int i = 0; i < rows; i++) {
        if (check_word(i, 0, 0, 1)) // left to right
            return true;
    }
    
    // Check columns (vertical)
    for (int j = 0; j < columns; j++) {
        if (check_word(0, j, 1, 0)) // top to bottom
            return true;
    }
    
    // Check diagonals
    if (check_word(0, 0, 1, 1)) // main diagonal (top-left to bottom-right)
        return true;
    
    if (check_word(0, 2, 1, -1)) // anti-diagonal (top-right to bottom-left)
        return true;
    
    return false;
}

bool Word_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Word_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Word_UI::Word_UI() : UI<char>("Welcome to Word Tic Tac Toe", 3) {}

Player<char>* Word_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Word_UI::get_move(Player<char>* player) {
    int x, y;
    char letter;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << ", Please enter position (x y) and letter: ";
        cin >> x >> y >> letter;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        
        letter = 'A' + (rand() % 26);
    }
    
    return new Move<char>(x, y, toupper(letter));
}
