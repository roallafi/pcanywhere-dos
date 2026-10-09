#ifndef DISPLAY_H
#define DISPLAY_H

#include <conio.h>
#include <string.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define COLOR_NORMAL 7
#define COLOR_TITLE 14
#define COLOR_STATUS 1

class Display {
public:
    Display();
    void clear();
    void draw_title(const char* title);
    void draw_message(const char* text, int color);
    void print_at(int x, int y, const char* text, int color);
};

#endif
