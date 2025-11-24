#include <iostream>
#include <vector>
#include <cstdlib>
#include "NumericalTTT.h"
using namespace std;


NumericalTTT_Board::NumericalTTT_Board(int rows, int cols)
    : Board<int>(rows, cols),n_moves(0) {

    for (int i = 0; i < 10; i++) used[i] = false;
}

Player<int>** NumericalTTT_UI::setup_players() {
    Player<int>** players = new Player<int>*[2];

    cout << "\n=== Player 1 (ODD numbers: 1, 3, 5, 7, 9) ===\n";
    string name1;
    cout << "Enter Player 1 name: ";
    cin >> name1;
    cout << "Choose Player 1 type:\n1. Human\n2. Computer\n> ";
    int choice1; cin >> choice1;
    PlayerType type1 = (choice1 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;
    players[0] = create_player(name1, 0, type1);  

    cout << "\n=== Player 2 (EVEN numbers: 2, 4, 6, 8) ===\n";
    string name2;
    if (type1 == PlayerType::COMPUTER) {
        cout << "Enter Player 2 name (or press Enter for 'Computer'): ";
        cin.ignore();
        getline(cin, name2);
        if (name2.empty()) name2 = "Computer";
    }
    else {
        cout << "Enter Player 2 name: ";
        cin >> name2;
    }
    cout << "Choose Player 2 type:\n1. Human\n2. Computer\n> ";
    int choice2; cin >> choice2;
    PlayerType type2 = (choice2 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;
    players[1] = create_player(name2, 0, type2);

    cout << "\nGame starting...\n";
    cout << players[0]->get_name() << " (Odd)  vs  " << players[1]->get_name() << " (Even)\n\n";

    return players;
}
bool NumericalTTT_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int num = move->get_symbol();

    bool odd_player = (n_moves % 2 == 0);
    bool error = false;
    string errors = "HAS ERROR! please fix the following\n";
    
    if ((odd_player && num % 2 == 0) || (!odd_player && num % 2 == 1)) {
        if (odd_player)
            errors += "Error: Player 1 can only play ODD numbers (1,3,5,7,9)\n";
        else
            errors += "Error: Player 2 can only play EVEN numbers (2,4,6,8)\n";
        error = true;
    }
    if (num < 1 || num > 9) {
        errors += "Invalid number : You must enter a number between 1 and 9.\n";
        error = true;
    }
    if (used[num]) {
        errors += "Error: Number " + to_string(num) + " has already been played!\n";
        errors += "Choose a number that hasn't been used yet.\n";
        error = true;
    }
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        errors += "Invalid Position : Row and column must be between 0 and 2.\n";
        error = true;
    }
    if (board[x][y] != 0) { 
        errors += "Invalid cell :The cell is already taken!\n";
        error = true;
    }
 
   

    

    if (error) {
        cout << errors << endl;
        return false; }

    board[x][y] = num;
    used[num] = true;
    n_moves++;
    return true;
}

bool NumericalTTT_Board::is_win(Player<int>* player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] && board[i][1] && board[i][2] &&
            board[i][0] + board[i][1] + board[i][2] == 15) return true;
        if (board[0][i] && board[1][i] && board[2][i] &&
            board[0][i] + board[1][i] + board[2][i] == 15) return true;
    }
    if (board[0][0] && board[1][1] && board[2][2] &&
        board[0][0] + board[1][1] + board[2][2] == 15) return true;
    if (board[0][2] && board[1][1] && board[2][0] &&
        board[0][2] + board[1][1] + board[2][0] == 15) return true;

    return false;
}

bool NumericalTTT_Board::is_draw(Player<int>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool NumericalTTT_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

NumericalTTT_UI::NumericalTTT_UI()
    : UI<int>("Welcome to Numerical Tic-Tac-Toe!", 3) {}

Player<int>* NumericalTTT_UI::create_player(string& name, int, PlayerType type) {
   
    return new Player<int>(name, 0, type);
}
Move<int>* NumericalTTT_UI::get_move(Player<int>* player) {
    NumericalTTT_Board* b = static_cast<NumericalTTT_Board*>(player->get_board_ptr());
    auto matrix = b->get_board_matrix(); 

    int x, y, num;
    int moves_count = 0; 

    cout << "\n   0   1   2\n\n";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (matrix[i][j] != 0)
                moves_count++;

    for (int i = 0; i < 3; i++) {
        cout << i << " ";
        for (int j = 0; j < 3; j++) {
            int val = matrix[i][j];
            if (val == 0) cout << " . ";
            else cout << " " << val << " ";
            if (j < 2) cout << "|";
        }
        cout << "\n";
        if (i < 2) cout << "  -----------\n";
    }
    cout << "\n";

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name()
            << ", Please enter your number and position (num x y): ";
        cin >> num >> x >> y;
    }
    else {  
        vector<int> available;

        
        bool odd_turn = (moves_count % 2 == 0); 

        for (int n = 1; n <= 9; n++) {
            bool number_used = false;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (matrix[i][j] == n)
                        number_used = true;

            if (!number_used) {
                if ((odd_turn && n % 2 == 1) || (!odd_turn && n % 2 == 0))
                    available.push_back(n);
            }
        }

        num = available[rand() % available.size()];

        do {
            x = rand() % 3;
            y = rand() % 3;
        } while (matrix[x][y] != 0);

        cout << "Computer plays " << num << " at (" << x << ", " << y << ")\n";
    }

    return new Move<int>(x, y, num);
}
