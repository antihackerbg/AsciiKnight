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
int jumpCount = 0;
short velocityY = 0;
short velocityX = 0;

/*
clock_t gravity(int* playerX, int* playerY, const char map[HEIGHT][WIDTH]) {
    if (map[*playerY - 1][*playerX] != '#' && map[*playerY - 1][*playerX] !='=' && velocityY < 0) {
        *playerY -= 1;
        velocityY++;
    }
    else if (map[*playerY + 1][*playerX] != '#' && map[*playerY + 1][*playerX] != '=') {
        *playerY += 1;
    }
    return (clock() + (CLOCKS_PER_SEC / 10));
}
*/

clock_t movePlayer(int* playerX, int* playerY, const char map[HEIGHT][WIDTH]) {
    
    if (map[(*playerY + 1)][*playerX] == '#' || map[(*playerY + 1)][*playerX] == '=') jumpCount = 0;

    int nextX = *playerX, nextY = *playerY;

    if (_kbhit()) {
        char key = _getch();
        

        if (key == 'w') if (jumpCount < 2) {
            jumpCount++;
            if(map[nextY - 1][nextX] != '=' && map[nextY - 1][nextX] != '#' && map[nextY - 2][nextX] != '=' && map[nextY - 2][nextX] != '#'){
                velocityY = -3;
            }
            else if (map[nextY - 1][nextX] != '=' && map[nextY - 1][nextX] != '#') {
                velocityY = -3;
            }
        };
        if (key == 'a') velocityX = -2;
        if (key == 'd') velocityX = 2;

        
    }

    if (map[*playerY - 1][*playerX] != '#' && map[*playerY - 1][*playerX] != '=' && velocityY < 0) {
        nextY = *playerY - 1;
        velocityY++;
    }
    else if (map[*playerY + 1][*playerX] != '#' && map[*playerY + 1][*playerX] != '=') {
        nextY = *playerY + 1;
    }

    if (map[*playerY][*playerX - 1] != '#' && map[*playerY][*playerX - 1] != '=' && velocityX < 0) {
        nextX = *playerX - 1;
        velocityX++;
    }
    else if (map[*playerY][*playerX + 1] != '#' && map[*playerY][*playerX + 1] != '=' && velocityX > 0) {
        nextX = *playerX + 1;
        velocityX--;
    }


    if (map[nextY][nextX] != '#') {
        *playerX = nextX;
        *playerY = nextY;
    }

    return (clock() + (CLOCKS_PER_SEC / 10));
}

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
    clock_t start = clock();
    clock_t length = seconds * CLOCKS_PER_SEC;
    while (clock() - start < length) {

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
    HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;

    //Code to make cursor invisible
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);


    char map[HEIGHT][WIDTH];
    int playerX = 40;
    int playerY = 17;
    //double current = (double)time(NULL);
    //double activateGravity = current + 0.2;

    clock_t current;
    clock_t move = clock() + (CLOCKS_PER_SEC / 10);

    printTopRow();
    printMap(map);


    while (running) {
        current = clock();

        //movement(&playerX, &playerY, map);

        if (move < current) {
           move = movePlayer(&playerX, &playerY, map);
        }
       
        updateMap(map, playerX, playerY);

        goToXY(0, 0);

        printTopRow();
        printMap(map);
    }

    return 0;
}