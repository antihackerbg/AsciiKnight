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

struct Enemy {
    char type;
    u_short x;
    u_short y;
    clock_t moveTimer; 
};

const u_short max_wave = 10;
const int HEIGHT = 20;
const int WIDTH = 120;
const u_short MAX_HEALTH = 5;

u_short wave = 1;
bool running = 1;
u_short health = 5;
int jumpCount = 0;
short velocityY = 0;
short velocityX = 0;
u_short maxForWave = 4;
bool isAttacking = false;
clock_t attackEnd = 0;
COORD attackPos = { 0, 0 };

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

void attack(const int playerX, const int playerY, char map[HEIGHT][WIDTH], char* key) {
    if(!isAttacking){
    
        switch (*key) {
        case 'j': 
            *key = ' ';
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX - 1;
            attackPos.Y = playerY;
            if (map[playerY][playerX - 1] != '#') {
                if (map[playerY + 1][playerX] != '#') {
                    map[playerY - 1][playerX - 1] = '/';
                    map[playerY][playerX - 1] = '|';
                    map[playerY + 1][playerX - 1] = '\\';
                }
                else {
                    map[playerY - 1][playerX - 1] = '/';
                    map[playerY][playerX - 1] = '|';
                }
            }
            break;
        case 'i':
            *key = ' ';
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX;
            attackPos.Y = playerY - 1;
            if (map[playerY - 1][playerX] != '#' && map[playerY - 1][playerX] != '=' && map[playerY - 1][playerX - 1] != '=' && map[playerY - 1][playerX + 1] != '=') {
                    map[playerY - 1][playerX - 1] = '\\';
                    map[playerY - 1][playerX] = '-';
                    map[playerY - 1][playerX + 1] = '/';
            }
            break;
        case 'k':
            *key = ' ';
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX;
            attackPos.Y = playerY + 1;
            if (map[playerY + 1][playerX] != '#') {
                    map[playerY + 1][playerX - 1] = '\\';
                    map[playerY + 1][playerX] = '_';
                    map[playerY + 1][playerX + 1] = '/';
            }
            break;
        case 'l':
            *key = ' ';
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX + 1;
            attackPos.Y = playerY;
            if (map[playerY][playerX - 1] != '#') {
                if (map[playerY + 1][playerX] != '#') {
                    map[playerY - 1][playerX + 1] = '\\';
                    map[playerY][playerX + 1] = '|';
                    map[playerY + 1][playerX + 1] = '/';
                }
                else {
                    map[playerY - 1][playerX + 1] = '\\';
                    map[playerY][playerX + 1] = '|';
                }
            }
            break;
        }
    }
}

void attackCleanup(char map[HEIGHT][WIDTH]) {
    if (isAttacking) {
        if (clock() > attackEnd) {
            if (map[attackPos.Y][attackPos.X] == '|' || map[attackPos.Y][attackPos.X] == '-' || map[attackPos.Y][attackPos.X] == '_') {
                map[attackPos.Y][attackPos.X] = ' ';
            }
            if (map[attackPos.Y - 1][attackPos.X] == '/' || map[attackPos.Y - 1][attackPos.X] == '\\') {
                map[attackPos.Y - 1][attackPos.X] = ' ';
            }
            if (map[attackPos.Y + 1][attackPos.X] == '\\' || map[attackPos.Y + 1][attackPos.X] == '/') {
                map[attackPos.Y + 1][attackPos.X] = ' ';
            }
            if (map[attackPos.Y][attackPos.X + 1] == '\\' || map[attackPos.Y][attackPos.X + 1] == '/') {
                map[attackPos.Y][attackPos.X + 1] = ' ';
            }
            if (map[attackPos.Y][attackPos.X - 1] == '\\' || map[attackPos.Y][attackPos.X - 1] == '/') {
                map[attackPos.Y][attackPos.X - 1] = ' ';
            }
            isAttacking = false;
        }
    }
}

clock_t movePlayer(const COORD currentPlayer, COORD* newPlayer, const char map[HEIGHT][WIDTH], char* key) {
    
    if (map[(currentPlayer.Y + 1)][currentPlayer.X] == '#' || map[(currentPlayer.Y + 1)][currentPlayer.X] == '=') jumpCount = 0;

    int nextX = currentPlayer.X, nextY = currentPlayer.Y;
        if (*key == 'w') if (jumpCount < 2) {
            *key = ' ';
            jumpCount++;
            if(map[nextY - 2][nextX] != '=' && map[nextY - 2][nextX] != '#'){
                velocityY = -3;
            }
            else if (map[nextY - 1][nextX] != '=' && map[nextY - 1][nextX] != '#') {
                velocityY = -3;
            }
        };
        if (*key == 'a') {
            velocityX = -2;
            *key = ' ';
        }
        if (*key == 'd') {
            velocityX = 2;
            *key = ' ';
        }

    if (map[currentPlayer.Y - 1][currentPlayer.X] != '#' && map[currentPlayer.Y - 1][currentPlayer.X] != '=' && velocityY < 0) {
        nextY = currentPlayer.Y - 1;
        velocityY++;
    }
    else if (map[currentPlayer.Y + 1][currentPlayer.X] != '#' && map[currentPlayer.Y + 1][currentPlayer.X] != '=') {
        nextY = currentPlayer.Y + 1;
    }

    if (map[currentPlayer.Y][currentPlayer.X - 1] != '#' && map[currentPlayer.Y][currentPlayer.X - 1] != '=' && velocityX < 0) {
        nextX = currentPlayer.X - 1;
        velocityX++;
    }
    else if (map[currentPlayer.Y][currentPlayer.X + 1] != '#' && map[currentPlayer.Y][currentPlayer.X + 1] != '=' && velocityX > 0) {
        nextX = currentPlayer.X + 1;
        velocityX--;
    }


    if (map[nextY][nextX] != '#') {
        (*newPlayer).X = nextX;
        (*newPlayer).Y = nextY;
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

void initMap(char map[HEIGHT][WIDTH], COORD playerStart) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == (HEIGHT - 1) || j == 0 || j == (WIDTH - 1)) {
                map[i][j] = '#';
            }
            else if (i == 16 && j > 10 && j < 30) {
                map[i][j] = '=';
            }
            else {
                map[i][j] = ' ';
            }
        }
    }
    map[playerStart.Y][playerStart.X] = '@';
}

void updateMap(char map[HEIGHT][WIDTH], COORD* oldCoords, COORD newCoords) {
    map[(*oldCoords).Y][(*oldCoords).X] = ' ';
    map[newCoords.Y][newCoords.X] = '@';
    *oldCoords = newCoords;
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

    srand(time(NULL));
    char map[HEIGHT][WIDTH];
    COORD oldPlayer;
    oldPlayer.X = 40;
    oldPlayer.Y = 17;
    char keyPress = ' ';
    COORD newPlayer = oldPlayer;
    clock_t current;
    clock_t move = clock() + (CLOCKS_PER_SEC / 10);
    Enemy* enemies = new Enemy[maxForWave];

    printTopRow();
    printMap(map);
    initMap(map, oldPlayer);

    while (running) {
        current = clock();

        if (_kbhit()) {
            keyPress = _getch();
        }
        if (move < current) {
            move = movePlayer(oldPlayer, &newPlayer, map, &keyPress);
            attack(oldPlayer.X, oldPlayer.Y, map, &keyPress);
            attackCleanup(map);
        }
       
        

        goToXY(0, 0);

        printTopRow();
        printMap(map);

        updateMap(map, &oldPlayer, newPlayer);
    }

    return 0;
}