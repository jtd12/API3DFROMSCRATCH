#ifndef DEF_GUI_HPP
#define DEF_GUI_HPP
#include<stdio.h>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <conio.h> // Pour _kbhit() et _getch()
#include <algorithm>
#include <cfloat>
#include<cmath>
#define MAXLEVEL 4
#define M_PI    3.14159265358979323846
#define SDL_MAIN_HANDLED
#include "C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"
#include"C:\library\SDL2_ttf-2.19.3\x86_64-w64-mingw32\include\SDL2\SDL_ttf.h"
#include"vector.h"

class Button {
	
    SDL_Rect rect;
	vector3d color;
	vector3d fillColor;
	bool isClicked;
	TTF_Font* font;
	std::string text;
    SDL_Color textColor;
    SDL_Texture* textTexture;
	
	public:
	Button(SDL_Rect rect_);
	Button(SDL_Rect rect_,const std::string& text_, TTF_Font* font_, SDL_Color color_);
	~Button();
    void render(SDL_Renderer* renderer);
    void renderPanel(SDL_Renderer* renderer);
    void renderText(SDL_Renderer* renderer);
    void handleEvent(SDL_Event& event);
    bool getClick();
    void setClick(bool b);
    void setColor(vector3d col);
    void setFillColor(vector3d fillCol);
    void setText(const std::string& newText,SDL_Renderer* renderer);
    void updateTexture(SDL_Renderer* renderer);
     
};

#endif
