/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Alexander Basamakov
* @idnumber 0MI0600579
* @compiler VC
*
*
*
*/

#include <iostream>
#include <ctime>     
#include <conio.h>   
#include <windows.h> 
#include <cstdlib>

using std::cin;
using std::cout;

const int HEIGHT = 20;
const int WIDTH = 120;
const u_short MAX_HEALTH = 5;
bool running = 1;
u_short health = 1;

void printTopRow() {
    char healthBar[MAX_HEALTH + 3];
    healthBar[0] = '[';
    healthBar[MAX_HEALTH + 1] = ']';
    healthBar[MAX_HEALTH + 2] = 0;
    for (int i = 1; i <= health; i++) {
        healthBar[i] = '%';
    }
    for (int i = health + 1; i < MAX_HEALTH + 1; i++) {
        healthBar[i] = ' ';
    }
    char info[] = "Controls(a/d - move; w - jump, double jump; i/j/k/l - attack)";
    cout << healthBar << "        " << info << std::endl;
}

void sleep(int seconds) {
    time_t current = time(NULL);
    time_t finished = current + seconds;
    while (current < finished) {
        current = time(NULL);
    }
}

void goToXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void updateMap(char map[HEIGHT][WIDTH], int playerX, int playerY) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == (HEIGHT - 1) || j == 0 || j == (WIDTH - 1)) {
                map[i][j] = '#';
            }
            else {
                map[i][j] = ' ';
            }
        }
    }
    map[playerY][playerX] = '@';
}

void printMap(char map[HEIGHT][WIDTH]) {
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << map[i][j];
        }
        cout << std::endl;
    }

}

int main()
{
    char map[HEIGHT][WIDTH];
    int playerX = 40;
    int playerY = 10;


    while (running) {
        sleep(1);
        updateMap(map, playerX, playerY);

        goToXY(0, 0);

        printTopRow();
        printMap(map);
    }

    return 0;
}