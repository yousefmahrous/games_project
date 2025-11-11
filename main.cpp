#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include "BoardGame_Classes.h"
#include "misere.h"

using namespace std;

void games() {
    cout << "1. Misere Tic Tac Toe" << endl;
    cout << "0. Exit" << endl;
}

void game_torun() {
    char choice;
    cin >> choice;
    switch (choice) {
    case '0':
        exit(0);
        break;
    case '1':
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