/*
 * Project: Node Runner
 * Environment: Dev-C++ (TDM-GCC 64-bit)
 * Features: Player Movement, Chasing AI (Devils), Static Traps (Bombs)
 */

#include <windows.h> // Required for Dev-C++
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// --- CONFIGURATION ---
#define CELL_SIZE 30
#define ROWS 15
#define COLS 15
#define ENEMY_SPEED 400 // Lower is faster (milliseconds)

// --- GAME STATE ---
int gameState = 0; // 0=Playing, 1=Won, 2=Dead

// --- ENTITIES ---
struct Entity {
    int x, y;
    // CONSTRUCTOR ADDED TO FIX THE RED LINE ERROR
    Entity(int _x, int _y) {
        x = _x;
        y = _y;
    }
};

Entity player(1, 1); // Start pos
Entity goal(13, 13); // Goal pos

// Enemies (The Devils)
vector<Entity> enemies;

// --- THE MAP ---
// 0=Path, 1=Wall, 2=Bomb
int map[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,2,0,0,0,1,0,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,1,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,2,0,1}, // 2 is a Bomb
    {1,0,1,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,2,1,0,1,1,1,1,0,1,1,1,1,0,1}, // 2 is a Bomb
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,0,1,0,1,1,2,1}, // 2 is a Bomb
    {1,0,0,0,2,0,0,1,0,0,0,0,0,0,1}, // 2 is a Bomb
    {1,0,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,0,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void init() {
    glClearColor(0.1, 0.1, 0.1, 1.0); // Dark background
    gluOrtho2D(0, COLS * CELL_SIZE, ROWS * CELL_SIZE, 0); // Origin top-left

    // FIXED SYNTAX FOR DEV-C++
    // Spawn Enemies (x, y)
    enemies.push_back(Entity(13, 1));
    enemies.push_back(Entity(1, 13));
    enemies.push_back(Entity(7, 7));
}

// Unit 2: Drawing Primitives
void drawSquare(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2i(x * CELL_SIZE, y * CELL_SIZE);
        glVertex2i((x + 1) * CELL_SIZE, y * CELL_SIZE);
        glVertex2i((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
        glVertex2i(x * CELL_SIZE, (y + 1) * CELL_SIZE);
    glEnd();

    // Outline
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
        glVertex2i(x * CELL_SIZE, y * CELL_SIZE);
        glVertex2i((x + 1) * CELL_SIZE, y * CELL_SIZE);
        glVertex2i((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
        glVertex2i(x * CELL_SIZE, (y + 1) * CELL_SIZE);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameState == 0) {
        // 1. Draw Static Map
        for(int y=0; y<ROWS; y++) {
            for(int x=0; x<COLS; x++) {
                if(map[y][x] == 1) drawSquare(x, y, 0.5, 0.5, 0.5);      // Wall (Grey)
                else if(map[y][x] == 2) drawSquare(x, y, 0.3, 0.0, 0.0); // Bomb (Dark Red)
                else drawSquare(x, y, 0.9, 0.9, 0.9);                     // Path (White)
            }
        }

        // 2. Draw Goal
        drawSquare(goal.x, goal.y, 0.0, 1.0, 0.0); // Green

        // 3. Draw Player
        drawSquare(player.x, player.y, 0.0, 0.0, 1.0); // Blue

        // 4. Draw Enemies
        for(size_t i=0; i<enemies.size(); i++) {
            drawSquare(enemies[i].x, enemies[i].y, 1.0, 0.0, 0.0); // Bright Red Devil
        }
    }
    else if (gameState == 1) {
        // Win Screen
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else if (gameState == 2) {
        // Lose Screen
        glClearColor(0.5, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glutSwapBuffers();
}

// Check if a move is valid (Wall check)
bool isValid(int x, int y) {
    if(x < 0 || x >= COLS || y < 0 || y >= ROWS) return false;
    if(map[y][x] == 1) return false; // Wall
    return true;
}

// Unit 3: AI Logic (Simple Pathfinding)
void moveEnemies(int value) {
    if(gameState != 0) return;

    for(size_t i=0; i<enemies.size(); i++) {
        int dx = player.x - enemies[i].x;
        int dy = player.y - enemies[i].y;

        int nextX = enemies[i].x;
        int nextY = enemies[i].y;

        // Try to move along X axis first if distance is greater
        if(abs(dx) > abs(dy)) {
            if(dx > 0) nextX++; else nextX--;
            if(!isValid(nextX, nextY)) {
                // X blocked, try Y
                nextX = enemies[i].x; // reset
                if(dy != 0) { if(dy > 0) nextY++; else nextY--; }
            }
        } else {
            // Try Y axis
            if(dy > 0) nextY++; else nextY--;
            if(!isValid(nextX, nextY)) {
                // Y blocked, try X
                nextY = enemies[i].y; // reset
                if(dx != 0) { if(dx > 0) nextX++; else nextX--; }
            }
        }

        // Apply Move if valid (Enemies can walk over bombs/each other)
        if(isValid(nextX, nextY)) {
            enemies[i].x = nextX;
            enemies[i].y = nextY;
        }

        // Check Collision with Player
        if(enemies[i].x == player.x && enemies[i].y == player.y) {
            gameState = 2; // Dead
            cout << "CAUGHT BY DEVIL!" << endl;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(ENEMY_SPEED, moveEnemies, 0); // Recursive timer
}

void keyboard(unsigned char key, int x, int y) {
    if (gameState != 0) return;

    int nextX = player.x;
    int nextY = player.y;

    if(key == 'w' || key == 'W') nextY--;
    if(key == 's' || key == 'S') nextY++;
    if(key == 'a' || key == 'A') nextX--;
    if(key == 'd' || key == 'D') nextX++;

    if(isValid(nextX, nextY)) {
        player.x = nextX;
        player.y = nextY;

        // Check Bomb (Trap)
        if(map[player.y][player.x] == 2) {
            gameState = 2; // Dead
            cout << "STEPPED ON A BOMB!" << endl;
        }

        // Check Goal
        if(player.x == goal.x && player.y == goal.y) {
            gameState = 1; // Won
            cout << "YOU ESCAPED!" << endl;
        }

        // Check Enemy Collision (Running into them)
        for(size_t i=0; i<enemies.size(); i++) {
            if(enemies[i].x == player.x && enemies[i].y == player.y) {
                gameState = 2;
                cout << "RAN INTO DEVIL!" << endl;
            }
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(COLS * CELL_SIZE, ROWS * CELL_SIZE);
    glutCreateWindow("Node Runner");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(ENEMY_SPEED, moveEnemies, 0); // Start AI Timer

    cout << "Blue = Player | Green = Goal" << endl;
    cout << "Red Squares = Devils (They chase you)" << endl;
    cout << "Dark Red Tiles = Bombs (Don't step on them!)" << endl;

    glutMainLoop();
    return 0;
}
