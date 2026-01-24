/*
 * Project: Node Runner
 * Environment: Dev-C++ (TDM-GCC 64-bit)
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
#define ENEMY_SPEED 400 

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

Entity player(1, 1); 
Entity goal(13, 13); 

// Enemies 
vector<Entity> enemies;

// --- THE MAP ---
// 0=Path, 1=Wall, 2=Bomb
int map[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,2,0,0,0,1,0,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,1,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,2,0,1},
    {1,0,1,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,2,1,0,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,0,1,0,1,1,2,1},
    {1,0,0,0,2,0,0,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,0,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void init() {
    glClearColor(0.1, 0.1, 0.1, 1.0); 
    gluOrtho2D(0, COLS * CELL_SIZE, ROWS * CELL_SIZE, 0); 
    
    // FIXED SYNTAX FOR DEV-C++
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
        for(int y=0; y<ROWS; y++) {
            for(int x=0; x<COLS; x++) {
                if(map[y][x] == 1) drawSquare(x, y, 0.5, 0.5, 0.5); 
                else if(map[y][x] == 2) drawSquare(x, y, 0.3, 0.0, 0.0); 
                else drawSquare(x, y, 0.9, 0.9, 0.9); 
            }
        }
        drawSquare(goal.x, goal.y, 0.0, 1.0, 0.0); 
        drawSquare(player.x, player.y, 0.0, 0.0, 1.0); 
        for(size_t i=0; i<enemies.size(); i++) {
            drawSquare(enemies[i].x, enemies[i].y, 1.0, 0.0, 0.0); 
        }
    } 
    else if (gameState == 1) {
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else if (gameState == 2) {
        glClearColor(0.5, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glutSwapBuffers();
}

bool isValid(int x, int y) {
    if(x < 0 || x >= COLS || y < 0 || y >= ROWS) return false;
    if(map[y][x] == 1) return false; 
    return true;
}

void moveEnemies(int value) {
    if(gameState != 0) return;

    for(size_t i=0; i<enemies.size(); i++) {
        int dx = player.x - enemies[i].x;
        int dy = player.y - enemies[i].y;
        int nextX = enemies[i].x;
        int nextY = enemies[i].y;

        if(abs(dx) > abs(dy)) {
            if(dx > 0) nextX++; else nextX--;
            if(!isValid(nextX, nextY)) { 
                nextX = enemies[i].x; 
                if(dy != 0) { if(dy > 0) nextY++; else nextY--; }
            }
        } else {
            if(dy > 0) nextY++; else nextY--;
            if(!isValid(nextX, nextY)) {
                nextY = enemies[i].y; 
                if(dx != 0) { if(dx > 0) nextX++; else nextX--; }
            }
        }

        if(isValid(nextX, nextY)) {
            enemies[i].x = nextX;
            enemies[i].y = nextY;
        }

        if(enemies[i].x == player.x && enemies[i].y == player.y) {
            gameState = 2; 
        }
    }
    glutPostRedisplay();
    glutTimerFunc(ENEMY_SPEED, moveEnemies, 0); 
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
        if(map[player.y][player.x] == 2) gameState = 2; 
        if(player.x == goal.x && player.y == goal.y) gameState = 1; 
        for(size_t i=0; i<enemies.size(); i++) {
             if(enemies[i].x == player.x && enemies[i].y == player.y) gameState = 2;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(COLS*CELL_SIZE, ROWS*CELL_SIZE);
    glutCreateWindow("Node Runner");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(ENEMY_SPEED, moveEnemies, 0); 
    glutMainLoop();
    return 0;
}
