#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 20
#define SNAKE_INITIAL_SIZE 3

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point position;
    Point direction;
} SnakeSegment;

typedef struct {
    Point position;  
} Food;

int windowWidth = 600;
int windowHeight = 600;
int gridSize = GRID_SIZE;

SnakeSegment snake[GRID_SIZE * GRID_SIZE];
int snakeSize = SNAKE_INITIAL_SIZE;
int direction = 1;  // 0: up, 1: right, 2: down, 3: left

Food food;

void init() {
    glClearColor(0.0, 0.0, 1.0, 1.0); // blue 
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, windowWidth, 0, windowHeight); // to make fit 
}

void drawSquare(int x, int y) {
    glBegin(GL_QUADS);
    glVertex2i(x * gridSize, y * gridSize);
    glVertex2i((x + 1) * gridSize, y * gridSize);
    glVertex2i((x + 1) * gridSize, (y + 1) * gridSize);
    glVertex2i(x * gridSize, (y + 1) * gridSize);
    glEnd();
}

void drawSnake() {
    glColor3f(0.0, 1.0, 0.0);  // Snake color: Green
    for (int i = 0; i < snakeSize; ++i) {
        drawSquare(snake[i].position.x, snake[i].position.y);
    }
}

void drawFood() {
    glColor3f(1.0, 0.0, 0.0);  // Food color: Red
    drawSquare(food.position.x, food.position.y);
}

void generateFood() {
    food.position.x = rand() % (windowWidth / gridSize);
    food.position.y = rand() % (windowHeight / gridSize);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSnake();
    drawFood();
    glutSwapBuffers();
}

void update(int value) {
    glutTimerFunc(100, update, 0);

    // Move the snake
    for (int i = snakeSize - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }

    // Update the head of the snake based on the current direction
    switch (direction) {
    case 0:  // Up
        snake[0].position.y += 1;
        break;
    case 1:  // Right
        snake[0].position.x += 1;
        break;
    case 2:  // Down
        snake[0].position.y -= 1;
        break;
    case 3:  // Left
        snake[0].position.x -= 1;
        break;
    }

    // Check for collisions with the walls
    if (snake[0].position.x < 0 || snake[0].position.x >= windowWidth / gridSize ||
        snake[0].position.y < 0 || snake[0].position.y >= windowHeight / gridSize) {
        exit(0);  // Game over
    }

    // Check for collisions with itself
    for (int i = 1; i < snakeSize; ++i) {
        if (snake[0].position.x == snake[i].position.x &&
            snake[0].position.y == snake[i].position.y) {
            exit(0);  // Game over
        }
    }

    // Check for collisions with food
    if (snake[0].position.x == food.position.x && snake[0].position.y == food.position.y) {
        // Snake eats the food
        snakeSize++;
        generateFood();
    }
    glutPostRedisplay();
}

void keyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        if (direction != 2)  // Avoid moving opposite direction
            direction = 0;
        break;
    case GLUT_KEY_DOWN:
        if (direction != 0)
            direction = 2;
        break;
    case GLUT_KEY_RIGHT:
        if (direction != 3)
            direction = 1;
        break;
    case GLUT_KEY_LEFT:
        if (direction != 1)
            direction = 3;
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Mark's Team Game");

    init();
    generateFood();

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(100, update, 0);

    glutMainLoop();

    return 0;
}
