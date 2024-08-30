//
// Created by huang on 7/25/2024.
//

#pragma once
#include "Tile.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
using namespace chrono;

struct Board {
    int col;
    int row;
    int mines;
    signed int count;
    vector<vector<Tile>> tiles;
    Texture hiddenTexture;
    Texture mineTexture;

    Board(int colparam, int rowparam, int minesparam, Texture &hiddenTexture, Texture &mineTexture, Texture &revealedTexture, Texture &numTexture, Texture &flagTexture) {
        col = colparam;
        row = rowparam;
        mines = minesparam;
        count = minesparam;

        //populates 2d tile vector with default tiles
        tiles.resize(row, vector<Tile>(col, Tile(false, false, 0, false, false, false, false, hiddenTexture, mineTexture, revealedTexture, numTexture, flagTexture)));
        Linker();
        RandMines();
        AdjNums();
    }

    void Linker() {
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                Tile* current = &tiles[i][j];
                if(j < col - 1) {
                    current -> right = &tiles[i][j + 1];
                }

                if(j > 0) {
                    current -> left = &tiles[i][j - 1];
                }

                if(i > 0) {
                    current -> up = &tiles[i - 1][j];
                    if (j > 0) {
                        current -> upL = &tiles[i - 1][j - 1];
                    }
                    if (j < col - 1) {
                        current -> upR = &tiles[i - 1][j + 1];
                    }
                }

                if(i < row - 1) {
                    current -> down = &tiles[i + 1][j];
                    if (j > 0) {
                        current -> downL = &tiles[i + 1][j - 1];
                    }
                    if (j < col - 1) {
                        current -> downR = &tiles[i + 1][j + 1];
                    }
                }
            }
        }
    }

    void RandMines() {
        vector<int> indices;
        for (int i = 0; i < row * col; ++i) {
            indices.push_back(i);
        }

        //makes a random number based on time and suffles the indicies to randomize order
        unsigned int base = system_clock::now().time_since_epoch().count();
        shuffle(indices.begin(), indices.end(), default_random_engine(base));

        for (int i = 0; i < mines; ++i) {
            int x = indices[i] / col;
            int y = indices[i] % col;
            tiles[x][y].mine = true;
        }
    }

    void AdjNums() {
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                Tile* current = &tiles[i][j];
                int counter = 0;

                if(current -> right != nullptr && current -> right -> mine == true) {
                    counter++;
                }
                if(current -> left != nullptr && current -> left -> mine == true) {
                    counter++;
                }
                if(current -> up != nullptr && current -> up -> mine == true) {
                    counter++;
                }
                if(current -> upL != nullptr && current -> upL -> mine == true) {
                    counter++;
                }
                if(current -> upR != nullptr && current -> upR -> mine == true) {
                    counter++;
                }
                if(current -> down != nullptr && current -> down -> mine == true) {
                    counter++;
                }
                if(current -> downL != nullptr && current -> downL -> mine == true) {
                    counter++;
                }
                if(current -> downR != nullptr && current -> downR -> mine == true) {
                    counter++;
                }

                current -> num = counter;
            }
        }
    }

    void DrawBoard(RenderWindow &window) {
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                tiles[i][j].tile.setPosition(32 * j,32 * i);
                tiles[i][j].Draw(window);
            }
        }
    }

    void Click(RenderWindow &window, bool state) {
        if(state == true) {
            if(Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i mouse = Mouse::getPosition(window);

                for(int i = 0; i < row; i++) {
                    for(int j = 0; j < col; j++) {
                        FloatRect bounds = tiles[i][j].tile.getGlobalBounds();
                        if(bounds.contains(static_cast<Vector2f>(mouse))) {
                            if (tiles[i][j].num == 0 && !tiles[i][j].flag) {
                                OpenSurr(i, j);
                            } else if (!tiles[i][j].flag) {
                                tiles[i][j].clicked = true;
                            }
                            tiles[i][j].Draw(window);
                        }
                    }
                }
            }

            if(Mouse::isButtonPressed(Mouse::Right)) {
                Vector2i mouse = Mouse::getPosition(window);

                for(int i = 0; i < row; i++) {
                    for(int j = 0; j < col; j++) {
                        FloatRect bounds = tiles[i][j].tile.getGlobalBounds();
                        if(bounds.contains(static_cast<Vector2f>(mouse))) {
                            if(tiles[i][j].clicked == false) {
                                if(!tiles[i][j].flag) {
                                    tiles[i][j].flag = true;
                                    count--;
                                    tiles[i][j].Draw(window);
                                } else {
                                    tiles[i][j].flag = false;
                                    count++;
                                    tiles[i][j].Draw(window);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void OpenSurr(int i, int j) {
        if(i < 0 || i >= row || j < 0 || j >= col || tiles[i][j].clicked || tiles[i][j].mine || tiles[i][j].flag) {
            return;
        }

        tiles[i][j].clicked = true;

        if(tiles[i][j].num != 0) {
            return;
        }

        OpenSurr(i, j - 1);
        OpenSurr(i, j + 1);
        OpenSurr(i - 1, j);
        OpenSurr(i + 1, j);
        OpenSurr(i - 1, j - 1);
        OpenSurr(i - 1, j + 1);
        OpenSurr(i + 1, j - 1);
        OpenSurr(i + 1, j + 1);
    }
};


