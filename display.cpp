#include "display.h"

Display::Display() {
    clear();
}

void Display::clear() {
    clrscr();
}

void Display::draw_title(const char* title) {
    int len = strlen(title);
    int pad = (SCREEN_WIDTH - len - 2) / 2;

    textcolor(COLOR_TITLE);
    gotoxy(1, 1);
    cprintf("╔");
    for (int i = 0; i < pad; i++) cprintf("═");
    cprintf(" %s ", title);
    for (int i = 0; i < SCREEN_WIDTH - pad - len - 4; i++) cprintf("═");
    cprintf("╗");
    textcolor(COLOR_NORMAL);
}

void Display::draw_message(const char* text, int color) {
    int x = (SCREEN_WIDTH - strlen(text)) / 2;
    print_at(x, SCREEN_HEIGHT / 2, text, color);
}

void Display::print_at(int x, int y, const char* text, int color) {
    textcolor(color);
    gotoxy(x, y);
    cprintf("%s", text);
    textcolor(COLOR_NORMAL);
}
