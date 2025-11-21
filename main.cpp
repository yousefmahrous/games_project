#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include "BoardGame_Classes.h"
#include "misere.h"
#include "fourinfour.h"

using namespace std;

void games() {
    cout << "1. Misere Tic Tac Toe" << endl;
    cout << "2. 4 x 4 Tic-Tac-Toe" << endl;
    cout << "0. Exit" << endl;
}

void game_torun() {
    char choice;
    cin >> choice;
    if (choice == '0') {
        exit(0);
    }
    else if (choice == '1') {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new misere_UI();

        Board<char>* misere_board = new misere_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> misere_game(misere_board, players, game_ui);

        misere_game.run();

        delete misere_board;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == '2') {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new fourinfour_UI();

        Board<char>* fourinfour_board = new fourinfour_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> fourinfour_game(fourinfour_board, players, game_ui);

        fourinfour_game.run();

        delete fourinfour_board;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
}

int main() {
    cout << "Hello in FCAI games!" << endl;
    cout << "Please choose the game to run : " << endl;
    while (true) {
        games();
        game_torun();
    }
    return 0;
}