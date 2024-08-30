//
// Created by huang on 7/25/2024.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
using namespace sf;

struct Tile {
    Sprite tile;

    Tile* up = nullptr;
    Tile* down = nullptr;
    Tile* left = nullptr;
    Tile* right = nullptr;
    Tile* upL = nullptr;
    Tile* upR = nullptr;
    Tile* downL = nullptr;
    Tile* downR = nullptr;

    bool mine;
    bool flag;
    int num;
    bool clicked;
    bool debugTile;
    bool pauseplayTile;
    bool lose;
    Texture hiddenTexture;
    Texture mineTexture;
    Texture revealedTexture;
    Texture numTexture;
    Texture flagTexture;

    Tile(bool mineparam, bool flagparam, int numparam, bool clickedparam, bool debugparam, bool pauseplayparam, bool loseparam, Texture &hiddenTxtr, Texture &mineTxtr, Texture &revealedTxtr, Texture &numTxtr, Texture &flagTxtr) {
        mine = mineparam;
        flag = flagparam;
        num = numparam;
        clicked = clickedparam;
        debugTile = debugparam;
        pauseplayTile = pauseplayparam;
        lose = loseparam;

        hiddenTexture = hiddenTxtr;
        mineTexture = mineTxtr;
        revealedTexture = revealedTxtr;
        numTexture = numTxtr;
        flagTexture = flagTxtr;

        //loads all textures
        hiddenTexture.loadFromFile("files/images/tile_hidden.png");
        revealedTexture.loadFromFile("files/images/tile_revealed.png");
        mineTexture.loadFromFile("files/images/mine.png");
        flagTexture.loadFromFile("files/images/flag.png");
    }

    void Draw(RenderWindow &window) {
        tile.setTexture(hiddenTexture);
        window.draw(tile);

        if(debugTile) {
            tile.setTexture(mineTexture);
            window.draw(tile);
        }

        if(pauseplayTile) {
            tile.setTexture(revealedTexture);
            window.draw(tile);
        }

        if(!pauseplayTile) {
            if(flag == true) {
                tile.setTexture(flagTexture);
                window.draw(tile);
            }

            if(debugTile) {
                tile.setTexture(mineTexture);
                window.draw(tile);
            }

            if(clicked == true && flag == false || lose == true) {
                if(mine == true) {
                    tile.setTexture(revealedTexture);
                    window.draw(tile);
                    tile.setTexture(mineTexture);
                    window.draw(tile);
                    if(flag == true) {
                        tile.setTexture(revealedTexture);
                        window.draw(tile);
                        tile.setTexture(flagTexture);
                        window.draw(tile);
                        tile.setTexture(mineTexture);
                        window.draw(tile);
                    }
                } else {
                    if(num > 0) {
                        numTexture.loadFromFile("files/images/number_" + to_string(num) + ".png");
                        tile.setTexture(revealedTexture);
                        window.draw(tile);
                        tile.setTexture(numTexture);
                        window.draw(tile);

                    } else if(num == 0) {
                        tile.setTexture(revealedTexture);
                        window.draw(tile);
                    }
                }
            }
        }
    }
};
