#include <iostream>
#include <iomanip>
#include <cctype>
#include "SUS.h"

using namespace std;

SUS_Board::SUS_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {

        char actual_mark = (mark == 'X') ? 'S' : 'U';
        board[x][y] = actual_mark;
        n_moves++;
        last_player_symbol = mark;
        check_sus_sequences();
        return true;
    }
    return false;
}

void SUS_Board::check_sus_sequences()
{
    vector<vector<pair<int, int>>> all_sequences = {
        {{0,0}, {0,1}, {0,2}}, {{1,0}, {1,1}, {1,2}}, {{2,0}, {2,1}, {2,2}},
        {{0,0}, {1,0}, {2,0}}, {{0,1}, {1,1}, {2,1}}, {{0,2}, {1,2}, {2,2}},
        {{0,0}, {1,1}, {2,2}}, {{0,2}, {1,1}, {2,0}}
    };

    for (auto& sequence : all_sequences) {
        bool already_counted = false;
        for (auto& counted_seq : counted_sequences) {
            if (counted_seq == sequence) {
                already_counted = true;
                break;
            }
        }
        if (already_counted) continue;

        char c1 = board[sequence[0].first][sequence[0].second];
        char c2 = board[sequence[1].first][sequence[1].second];
        char c3 = board[sequence[2].first][sequence[2].second];

        if (c1 == 'S' && c2 == 'U' && c3 == 'S') {
            if (last_player_symbol == 'X') {
                sus_counter1++;
            }
            else if (last_player_symbol == 'O') {
                sus_counter2++;
            }
            counted_sequences.push_back(sequence);
        }
    }
}

void SUS_Board::display_board() {
    cout << "\nCurrent Board:\n";
    cout << "  0 1 2\n";
    for (int i = 0; i < rows; i++) {
        cout << i << " ";
        for (int j = 0; j < columns; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Scores - Player 1: " << sus_counter1 << " | Player 2: " << sus_counter2 << endl;
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves == 9) {
        if (player->get_symbol() == 'X') {
            return sus_counter1 > sus_counter2;
        }
        else {
            return sus_counter2 > sus_counter1;
        }
    }
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && sus_counter1 == sus_counter2);
}

bool SUS_Board::is_lose(Player<char>* player) {
    if (n_moves == 9) {
        if (player->get_symbol() == 'X') {
            return sus_counter1 < sus_counter2;
        }
        else {
            return sus_counter2 < sus_counter1;
        }
    }
    return false;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    if (n_moves == 9) {
        cout << "\nFinal Scores:\n";
        cout << "Player 1: " << sus_counter1 << " points\n";
        cout << "Player 2: " << sus_counter2 << " points\n";
        return true;
    }
    return false;
}

SUS_UI::SUS_UI() : UI<char>("Welcome to SUS!", 3) {}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " ,Please enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
