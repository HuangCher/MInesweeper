#include "Windows.h"
#include <fstream>

int main() {
    //reads in the config values
    string col;
    string row;
    string mines;
    ifstream file;
    file.open("files/config.cfg");
    getline(file, col, '\n');
    getline(file, row, '\n');
    getline(file, mines, '\n');
    file.close();


    file.close();
    Windows welcome(stoi(col), stoi(row), stoi(mines), "");
    bool result = welcome.WelcomeWin();

    if(result == true) {
        Windows game(stoi(col), stoi(row), stoi(mines), welcome.name);
        game.GameWin();
    }
    return 0;
}


