//
// Created by huang on 7/25/2024.
//

#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cctype>
#include <string>
#include "Board.h"
#include <fstream>
#include <map>
using namespace std;
using namespace sf;

struct Windows {
    int col;
    int row;
    int mines;
    string name;

    Windows(int colparam, int rowparam, int minesparam, string nameparam) {
        col = colparam;
        row = rowparam;
        mines = minesparam;
        name = nameparam;
    }

    void setText(Text &text, float x, float y) {
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        text.setPosition(Vector2f(x, y));
    }

    bool WelcomeWin() {
        int width = col * 32;
        int height = (row * 32) + 100;
        // string name;

        RenderWindow window(VideoMode(width, height), "Minesweeper");
        Font font;
        font.loadFromFile("files/font.ttf");

        //set up for all texts
        Text textWelcome;
        textWelcome.setFont(font);
        textWelcome.setString("WELCOME TO MINESWEEPER!");
        textWelcome.setCharacterSize(24);
        textWelcome.setFillColor(Color::White);
        textWelcome.setStyle(Text::Bold | Text::Underlined);

        Text textEnter;
        textEnter.setFont(font);
        textEnter.setString("Enter your name:");
        textEnter.setCharacterSize(20);
        textEnter.setFillColor(Color::White);
        textEnter.setStyle(Text::Bold);

        Text textName;
        textName.setFont(font);
        textName.setCharacterSize(18);
        textName.setFillColor(Color::Yellow);
        textName.setStyle(Text::Bold);

        setText(textWelcome, width/2, (height/2) - 150);
        setText(textEnter, width/2, (height/2) - 75);
        textName.setString(name + "|");
        setText(textName, width/2, (height/2) - 45);

        while(window.isOpen()) {
            Event event;
            while(window.pollEvent(event)) {
                if(event.type == Event::Closed) {
                    window.close();
                }
                if (event.type == Event::TextEntered) {
                    if(event.text.unicode < 128) {
                        if(isalpha(event.text.unicode) && name.length() < 10) {
                            if(name.length() == 0) {
                                name += toupper(static_cast<char>(event.text.unicode));
                            } else {
                                name += tolower(static_cast<char>(event.text.unicode));
                            }
                            textName.setString(name + "|");
                            setText(textName, width/2, (height/2) - 45);

                        } else if(event.text.unicode == 8) {
                            if(name.length() > 0) {
                                name.pop_back();
                                textName.setString(name + "|");
                                setText(textName, width/2, (height/2) - 45);
                            }

                        } else if(event.text.unicode == 13) {
                            if(name.length() > 0) {
                                window.close();
                                return true;
                            }
                        }
                    }
                }
            }
            window.clear(Color::Blue);
            window.draw(textWelcome);
            window.draw(textEnter);
            window.draw(textName);
            window.display();
        }
    }

    void GameWin() {
        int width = col * 32;
        int height = (row * 32) + 100;

        high_resolution_clock::time_point start = high_resolution_clock::now();
        high_resolution_clock::time_point stop;
        int stoppedTime = 0;
        int currentTime = 0;
        int finalTime = 0;

        //all textures
        Texture hiddenTexture;
        Texture mineTexture;
        Texture revealedTexture;
        Texture numTexture;
        Texture flagTexture;
        Texture debugTexture;
        Texture happyTexture;
        Texture loseTexture;
        Texture winTexture;
        Texture leaderboardTexture;
        Texture pauseTexture;
        Texture playTexture;
        debugTexture.loadFromFile("files/images/debug.png");
        happyTexture.loadFromFile("files/images/face_happy.png");
        loseTexture.loadFromFile("files/images/face_lose.png");
        winTexture.loadFromFile("files/images/face_win.png");
        leaderboardTexture.loadFromFile("files/images/leaderboard.png");
        pauseTexture.loadFromFile("files/images/pause.png");
        playTexture.loadFromFile("files/images/play.png");

        //all sprites
        Sprite debug;
        Sprite happy;
        Sprite lose;
        Sprite win;
        Sprite leaderboard;
        Sprite playpause;

        RenderWindow window(VideoMode(width, height), "Minesweeper");
        Board board(col, row, mines, hiddenTexture, mineTexture, revealedTexture, numTexture, flagTexture);

        debug.setTexture(debugTexture);
        debug.setPosition((col * 32) - 304, 32 * (row + 0.5f));
        happy.setTexture(happyTexture);
        happy.setPosition((col/2.0f * 32) - 32, 32 * (row + 0.5f));
        lose.setTexture(loseTexture);
        lose.setPosition((col/2.0f * 32) - 32, 32 * (row + 0.5f));
        win.setTexture(winTexture);
        win.setPosition((col/2.0f * 32) - 32, 32 * (row + 0.5f));
        leaderboard.setTexture(leaderboardTexture);
        leaderboard.setPosition((col * 32) - 176, 32 * (row + 0.5f));
        playpause.setTexture(pauseTexture);
        playpause.setPosition((col * 32) - 240, 32 * (row + 0.5f));
        Sprite currentState = happy;
        bool playpauseState = true;
        bool boardState = true;
        bool winState;
        bool loseState = false;

        while(window.isOpen()) {
            Event event;
            while(window.pollEvent(event)) {
                if(event.type == Event::Closed) {
                    window.close();
                }

                // defines win and lose state
                winState = Victory(board);
                Defeat(window, board, loseState);
                board.Click(window, boardState);

                if(winState) {
                    for(int i = 0; i < row; i++) {
                        for(int j = 0; j < col; j++) {
                            if(board.tiles[i][j].mine == true) {
                                board.tiles[i][j].flag = true;
                                board.tiles[i][j].debugTile = false;
                            }
                        }
                    }

                    if(boardState == true) {
                        currentState = win;
                        stop = high_resolution_clock::now();
                        finalTime = duration_cast<chrono::seconds>(stop - start).count() - stoppedTime;
                        board.DrawBoard(window);
                        window.display();
                        LeaderboardWin(board, playpauseState, winState, finalTime);
                    }
                    boardState = false;
                    board.Click(window, boardState);
                }

                else if(loseState) {
                    for(int i = 0; i < row; i++) {
                        for(int j = 0; j < col; j++) {
                            if(board.tiles[i][j].mine == true) {
                                board.tiles[i][j].lose = true;
                            }
                        }
                    }
                    if(boardState == true) {
                        currentState = lose;
                        stop = high_resolution_clock::now();
                        finalTime = duration_cast<chrono::seconds>(stop - start).count() - stoppedTime;
                    }
                    boardState = false;
                    board.Click(window, boardState);
                }

                if(Mouse::isButtonPressed(Mouse::Left)) {
                    // debug button implementation
                    Vector2i debugMouse = Mouse::getPosition(window);
                    FloatRect debugBounds = debug.getGlobalBounds();
                    if(debugBounds.contains(static_cast<Vector2f>(debugMouse)) && boardState && !winState && !loseState) {
                        DebugButton(board, winState, loseState);
                    }

                    // reset button implementation
                    Vector2i resetMouse = Mouse::getPosition(window);
                    FloatRect resetBounds = currentState.getGlobalBounds();
                    if(resetBounds.contains(static_cast<Vector2f>(resetMouse))) {
                        string col2;
                        string row2;
                        string mines2;
                        ifstream file;
                        file.open("files/config.cfg");
                        getline(file, col2, '\n');
                        getline(file, row2, '\n');
                        getline(file, mines2, '\n');
                        file.close();
                        //resets board
                        Board newBoard(stoi(col2), stoi(row2), stoi(mines2), hiddenTexture, mineTexture, revealedTexture, numTexture, flagTexture);
                        board = newBoard;
                        //resets buttons
                        currentState = happy;
                        boardState = true;
                        loseState = false;
                        winState = false;
                        //resets play/pause
                        playpauseState = true;
                        playpause.setTexture(pauseTexture);
                        playpause.setPosition((col * 32) - 240, 32 * (row + 0.5f));
                        window.draw(playpause);
                        //resets time
                        start = high_resolution_clock::now();
                        stoppedTime = 0;
                        currentTime = 0;
                        finalTime = 0;
                    }

                    // play/pause button implementation
                    Vector2i playpauseMouse = Mouse::getPosition(window);
                    FloatRect playpauseBounds = playpause.getGlobalBounds();
                    if(playpauseBounds.contains(static_cast<Vector2f>(playpauseMouse)) && !winState && !loseState) {
                        if(!playpauseState) {
                            high_resolution_clock::time_point now = high_resolution_clock::now();
                            stoppedTime += duration_cast<chrono::seconds>(now - stop).count();
                        } else {
                            stop = high_resolution_clock::now();
                        }
                        playpauseState = !playpauseState;
                        boardState = !boardState;
                        PlayPause(board);
                        // if(playpauseState) {
                        //     boardState = false;
                        // }
                    }

                    // leaderboard button implementation
                    Vector2i leaderMouse = Mouse::getPosition(window);
                    FloatRect leaderBounds = leaderboard.getGlobalBounds();
                    if(leaderBounds.contains(static_cast<Vector2f>(leaderMouse))) {
                        if(playpauseState == true) {
                            //not paused & leaderboard
                            playpauseState = false;
                            PlayPause(board);
                            board.DrawBoard(window);
                            stop = high_resolution_clock::now();
                            currentTime = duration_cast<chrono::seconds>(stop - start).count() - stoppedTime;
                            if(boardState == false) {
                                currentTime = finalTime;
                            }
                            Timer(currentTime, window);
                            window.display();
                            LeaderboardWin(board, playpauseState, winState, finalTime);
                            high_resolution_clock::time_point now = high_resolution_clock::now();
                            stoppedTime += duration_cast<chrono::seconds>(now - stop).count();
                        } else {
                            //paused & leaderboard
                            playpauseState = true;
                            LeaderboardWin(board, playpauseState, winState, finalTime);
                            playpauseState = false;
                        }
                    }
                }
            }
            window.clear(Color::White);
            board.DrawBoard(window);
            Counter(board, window, winState);
            if(playpauseState && !loseState && !winState) {
                currentTime = duration_cast<chrono::seconds>(high_resolution_clock::now() - start).count() - stoppedTime;
            } else if(loseState || winState) {
                currentTime = finalTime;
            }
            Timer(currentTime, window);
            window.draw(debug);
            window.draw(currentState);
            window.draw(leaderboard);
            if(!playpauseState) {
                playpause.setTexture(playTexture);
            }
            else {
                playpause.setTexture(pauseTexture);
            }
            window.draw(playpause);
            window.display();
        }
    }

    bool LeaderboardWin(Board &board, bool &playpauseState, bool winState, int finalTime) {
        map<string, string> people;
        string times;
        string names;
        string time;

        ofstream filewrite;
        filewrite.open("files/leaderboard.txt", ios::app);

        if(winState == true) {
            if(finalTime != 0) {
                int sec = finalTime % 60;
                int min = finalTime / 60;
                string secFixed;
                string minFixed;
                if(min < 10) {
                    minFixed = "0" + to_string(min);
                } else {
                    minFixed = to_string(min);
                }
                if(sec < 10) {
                    secFixed = "0" + to_string(sec);
                } else {
                    secFixed = to_string(sec);
                }
                time = minFixed + ':' + secFixed;

                filewrite << '\n' << time + ", " + name;
            }
        }
        filewrite.close();

        ifstream fileread;
        fileread.open("files/leaderboard.txt");
        string temptime;
        while(!fileread.eof()) {
            getline(fileread, times, ',');
            getline(fileread, names, '\n');
            names = names.substr(1);
            people[times] = names;
        }

        for(auto i = people.begin(); i != people.end(); i++) {
            if(i -> second == name) {
                people[times] = name + '*';
            }
        }

        int width = col * 16;
        int height = (row * 16) + 50;

        RenderWindow window(VideoMode(width, height), "Minesweeper");
        Font font;
        font.loadFromFile("files/font.ttf");

        Text textLead;
        textLead.setFont(font);
        textLead.setString("LEADERBOARD");
        textLead.setCharacterSize(20);
        textLead.setFillColor(Color::White);
        textLead.setStyle(Text::Bold | Text::Underlined);

        Text textContent;
        textContent.setFont(font);
        textContent.setCharacterSize(18);
        textContent.setFillColor(Color::White);
        textContent.setStyle(Text::Bold);

        string content;
        int j = 1;
        for (auto i = people.begin(); j < 6; i++) {
            content += to_string(j) + ".\t" + i -> first + "\t" + i -> second + "\n\n";
            j++;
        }
        textContent.setString(content);

        setText(textLead, width/2, (height/2) - 120);
        setText(textContent, width/2, (height/2) + 20);
        while(window.isOpen()) {
            Event event;
            while(window.pollEvent(event)) {
                if(event.type == Event::Closed) {
                    window.close();
                    if(playpauseState == false) {
                        PlayPause(board);
                        board.DrawBoard(window);
                        playpauseState = true;
                    }

                    // if(winState == true) {
                    //     cout << playpauseState << endl;
                    //     PlayPause(board);
                    // }

                }
            }
            window.clear(Color::Blue);
            window.draw(textLead);
            window.draw(textContent);
            window.display();
        }
    }

    bool Victory(Board &board) {
        int amountOpened = 0;
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                if(board.tiles[i][j].clicked == true && board.tiles[i][j].mine == false) {
                    amountOpened++;
                }
            }
        }
       if(col * row == board.mines + amountOpened) {
            return true;
       }
        return false;
    }

    void Defeat(RenderWindow &window, Board &board, bool &state) {
        if(Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mouse = Mouse::getPosition(window);
            for(int i = 0; i < row; i++) {
                for(int j = 0; j < col; j++) {
                    FloatRect bounds = board.tiles[i][j].tile.getGlobalBounds();
                    if(bounds.contains(static_cast<Vector2f>(mouse))) {
                        if(board.tiles[i][j].mine == true && !board.tiles[i][j].flag) {
                            state = true;
                        }
                    }
                }
            }
        }
    }

    void DebugButton(Board &board, bool winState, bool loseState) {
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                if(board.tiles[i][j].mine == true) {
                    board.tiles[i][j].debugTile = !board.tiles[i][j].debugTile;
                }
            }
        }
    }

    void PlayPause(Board &board) {
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                board.tiles[i][j].pauseplayTile = !board.tiles[i][j].pauseplayTile;
            }
        }
    }

    void Counter(Board &board, RenderWindow &window, bool winState) {
        Texture digitsTexture;
        digitsTexture.loadFromFile("files/images/digits.png");
        Sprite counter;
        counter.setTexture(digitsTexture);
        signed int count[3];
        signed int temp;
        if(winState == true) {
            temp = 0;
        } else {
            temp = board.count;
        }

        if(temp < 0) {
            counter.setTextureRect(IntRect(21 * 10, 0, 21, 32));
            counter.setPosition(12, 32 * (row + 0.5) + 16);
            window.draw(counter);
            temp = abs(board.count);
        }

        count[2] = temp % 10;
        temp /= 10;
        count[1] = temp % 10;
        temp /= 10;
        count[0] = temp % 10;

        counter.setTextureRect(IntRect(21 * count[0], 0, 21, 32));
        counter.setPosition(33, 32 * (row + 0.5) + 16);
        window.draw(counter);
        counter.setTextureRect(IntRect(21 * count[1], 0, 21, 32));
        counter.setPosition(54, 32 * (row + 0.5) + 16);
        window.draw(counter);
        counter.setTextureRect(IntRect(21 * count[2], 0, 21, 32));
        counter.setPosition(75, 32 * (row + 0.5) + 16);
        window.draw(counter);
    }

    void Timer(int time, RenderWindow &window) {
        Texture digitsTexture;
        digitsTexture.loadFromFile("files/images/digits.png");
        Sprite timer;
        timer.setTexture(digitsTexture);
        int clock[4];
        int sec = time % 60;
        int min = time / 60;

        clock[3] = sec % 10;
        sec /= 10;
        clock[2] = sec % 10;
        clock[1] = min % 10;
        min /= 10;
        clock[0] = min % 10;

        timer.setTextureRect(IntRect(21 * clock[0], 0, 21, 32));
        timer.setPosition((col * 32) - 97, 32 * (row + 0.5) + 16);
        window.draw(timer);
        timer.setTextureRect(IntRect(21 * clock[1], 0, 21, 32));
        timer.setPosition((col * 32) - 76, 32 * (row + 0.5) + 16);
        window.draw(timer);
        timer.setTextureRect(IntRect(21 * clock[2], 0, 21, 32));
        timer.setPosition((col * 32) - 54, 32 * (row + 0.5) + 16);
        window.draw(timer);
        timer.setTextureRect(IntRect(21 * clock[3], 0, 21, 32));
        timer.setPosition((col * 32) - 33, 32 * (row + 0.5) + 16);
        window.draw(timer);
    }
};