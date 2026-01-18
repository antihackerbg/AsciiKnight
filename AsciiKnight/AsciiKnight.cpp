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
    bool active;
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

void checkHit(int targetY, int targetX, Enemy enemies[], int max_enemies, int& activeCount) {
    for (int i = 0; i < max_enemies; i++) {

        if (enemies[i].active) {

            if (enemies[i].x == targetX && enemies[i].y == targetY) {
                enemies[i].active = false; 
                activeCount--;             
            }
        }
    }
}

void attack(const int playerX, const int playerY, char map[HEIGHT][WIDTH], char* key, Enemy enemies[], int& activeCount) {
    if(!isAttacking){
        char top, mid, bot;
        switch (*key) {
        case 'j': 
            *key = ' ';
            top = map[playerY - 1][playerX - 1];
            mid = map[playerY][playerX - 1];
            bot = map[playerY + 1][playerX - 1];
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX - 1;
            attackPos.Y = playerY;
            if (map[playerY][playerX - 1] != '#' && top != '#' && mid != '#' && top != '=' && mid != '=') {
                checkHit(playerY - 1, playerX - 1, enemies, maxForWave, activeCount);
                checkHit(playerY, playerX - 1, enemies, maxForWave, activeCount);
                if (bot != '#' && bot != '=') {
                    map[playerY - 1][playerX - 1] = '/';
                    map[playerY][playerX - 1] = '|';
                    checkHit(playerY + 1, playerX - 1, enemies, maxForWave, activeCount);
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
            top = map[playerY - 1][playerX - 1];
            mid = map[playerY - 1][playerX];
            bot = map[playerY - 1][playerX + 1];
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX;
            attackPos.Y = playerY - 1;
            if (top != '#' && mid != '#' && bot != '#' && top != '=' && mid != '=' && bot != '=') {
                checkHit(playerY - 1, playerX - 1, enemies, maxForWave, activeCount);
                    map[playerY - 1][playerX - 1] = '\\';
                    checkHit(playerY - 1, playerX, enemies, maxForWave, activeCount);
                    map[playerY - 1][playerX] = '-';
                    checkHit(playerY - 1, playerX + 1, enemies, maxForWave, activeCount);
                    map[playerY - 1][playerX + 1] = '/';
            }
            break;
        case 'k':
            *key = ' ';
            top = map[playerY + 1][playerX - 1];
            mid = map[playerY + 1][playerX];
            bot = map[playerY + 1][playerX + 1];
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX;
            attackPos.Y = playerY + 1;
            if (top != '#' && mid != '#' && bot != '#' && top != '=' && mid != '=' && bot != '=') {
                checkHit(playerY + 1, playerX - 1, enemies, maxForWave, activeCount);
                    map[playerY + 1][playerX - 1] = '\\';
                    checkHit(playerY + 1, playerX, enemies, maxForWave, activeCount);
                    map[playerY + 1][playerX] = '_';
                    checkHit(playerY + 1, playerX + 1, enemies, maxForWave, activeCount);
                    map[playerY + 1][playerX + 1] = '/';
            }
            break;
        case 'l':
            *key = ' ';
            top = map[playerY - 1][playerX + 1];
            mid = map[playerY][playerX + 1];
            bot = map[playerY + 1][playerX + 1];
            isAttacking = 1;
            attackEnd = clock() + (CLOCKS_PER_SEC / 2);
            attackPos.X = playerX + 1;
            attackPos.Y = playerY;
            if (map[playerY][playerX + 1] != '#' && top != '#' && mid != '#' && top != '=' && mid != '=') {
                checkHit(playerY - 1, playerX + 1, enemies, maxForWave, activeCount);
                checkHit(playerY, playerX + 1, enemies, maxForWave, activeCount);
                if (bot != '#' && bot != '=') {
                    map[playerY - 1][playerX + 1] = '\\';
                    map[playerY][playerX + 1] = '|';
                    checkHit(playerY + 1, playerX + 1, enemies, maxForWave, activeCount);
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

void spawnEnemies(Enemy enemies[], int max_size,int* currentCount, char map[HEIGHT][WIDTH], COORD playerPos) {
    int enemiesToSpawn = (rand() % 4) + 2;

    int spawnedSoFar = 0;

    for (int i = 0; i < maxForWave; i++) {
        if (spawnedSoFar >= enemiesToSpawn) break;

        if (!enemies[i].active) {
            bool validPosition = false;
            int randX, randY;

            while (!validPosition) {
                randX = (rand() % (WIDTH - 2)) + 1;
                randY = (rand() % (HEIGHT - 2)) + 1;

                if (map[randY][randX] == ' ' && (randX != playerPos.X || randY != playerPos.Y)) {
                    validPosition = true;
                }
            }

            enemies[i].active = true;
            enemies[i].type = 'E';
            enemies[i].x = randX;
            enemies[i].y = randY;
            enemies[i].moveTimer = clock();

            // Place on map
            map[randY][randX] = enemies[i].type;

            spawnedSoFar++;
            currentCount++;
        }
    }
}

void moveEnemies(Enemy enemies[], int max_size, char map[HEIGHT][WIDTH], COORD playerPos) {
    clock_t now = clock();

    for (int i = 0; i < max_size; i++) {

        if (!enemies[i].active) continue;

        if (now > enemies[i].moveTimer) {

            map[enemies[i].y][enemies[i].x] = ' ';

            int nextX = enemies[i].x;
            int nextY = enemies[i].y;

            if (map[enemies[i].y + 1][enemies[i].x] != '#' && map[enemies[i].y + 1][enemies[i].x] != '=') {
                nextY++; 
            }

            else {
                if (enemies[i].x < playerPos.X) {
                    
                    if (map[enemies[i].y][enemies[i].x + 1] != '#' && map[enemies[i].y][enemies[i].x + 1] != '=') {
                        nextX++;
                    }
                }
                else if (enemies[i].x > playerPos.X) {
                    
                    if (map[enemies[i].y][enemies[i].x - 1] != '#' && map[enemies[i].y][enemies[i].x - 1] != '=') {
                        nextX--;
                    }
                }
            }

            enemies[i].x = nextX;
            enemies[i].y = nextY;
            if (nextX == playerPos.X && nextY == playerPos.Y) health--;
            map[enemies[i].y][enemies[i].x] = enemies[i].type;

            enemies[i].moveTimer = clock() + (CLOCKS_PER_SEC / 5); 
        }
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
    int activeEnemyCount = 0;
    for (int i = 0; i < maxForWave; i++) enemies[i].active = false;

    printTopRow();
    printMap(map);
    initMap(map, oldPlayer);

    while (running) {
        current = clock();
        if (health == 0) break;
        if (activeEnemyCount == 0) {
            spawnEnemies(enemies, max_wave, &activeEnemyCount, map, newPlayer);
        }

        if (_kbhit()) {
            keyPress = _getch();
        }

        moveEnemies(enemies, maxForWave, map, newPlayer);

        if (move < current) {
            move = movePlayer(oldPlayer, &newPlayer, map, &keyPress);
            attack(oldPlayer.X, oldPlayer.Y, map, &keyPress, enemies, activeEnemyCount);
            attackCleanup(map);
        }
       
        

        goToXY(0, 0);

        printTopRow();
        printMap(map);

        updateMap(map, &oldPlayer, newPlayer);
    }

    delete[] enemies;
    system("CLS");
    goToXY(0, 0);

    cout << "################################" << std::endl;
    cout << "#          GAME OVER!          #" << std::endl;
    cout << "################################" << std::endl;

    return 0;
}