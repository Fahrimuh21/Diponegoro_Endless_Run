#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h> // <-- WAJIB buat printf
// Posisi player
float playerX = 0.0f;
float playerY = 0.0f;
float velocityY = 0.0f;
bool isJumping = false;

// Obstacle
float obstacleZ = -50.0f;

// Game state
bool gameOver = false;
int score = 0;

// ===== DRAW OBJECT =====
void drawPlayer() {
    glPushMatrix();
    glTranslatef(playerX, playerY, 5.0f);
    glColor3f(0, 0, 1);
    glutSolidCube(2);
    glPopMatrix();
}

void drawObstacle() {
    glPushMatrix();
    glTranslatef(0, 0, obstacleZ);
    glColor3f(1, 0, 0);
    glutSolidCube(2);
    glPopMatrix();
}

void drawGround() {
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_QUADS);
    glVertex3f(-10, -2, 10);
    glVertex3f(10, -2, 10);
    glVertex3f(10, -2, -100);
    glVertex3f(-10, -2, -100);
    glEnd();
}

// ===== DISPLAY =====
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera third person
    gluLookAt(0, 5, 15, 0, 0, 0, 0, 1, 0);

    drawGround();
    drawPlayer();
    drawObstacle();

    glutSwapBuffers();
}

// ===== UPDATE GAME =====
void update(int value) {
    if (!gameOver) {
        // Gravity
        if (isJumping) {
            playerY += velocityY;
            velocityY -= 0.01f;

            if (playerY <= 0) {
                playerY = 0;
                isJumping = false;
            }
        }

        // Gerak obstacle
        obstacleZ += 0.5f;

        // Reset obstacle
        if (obstacleZ > 10) {
            obstacleZ = -50;
            score++;
        }

        // Collision detection
        if (fabs(playerX) < 1.5 && fabs(obstacleZ - 5) < 1.5 && playerY < 1) {
            gameOver = true;
            printf("Game Over! Score: %d\n", score);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ===== INPUT =====
void keyboard(int key, int x, int y) {
    if (gameOver) return;

    if (key == GLUT_KEY_LEFT) {
        playerX -= 2;
    }
    if (key == GLUT_KEY_RIGHT) {
        playerX += 2;
    }
}

void keyboardNormal(unsigned char key, int x, int y) {
    if (key == ' ' && !isJumping) {
        isJumping = true;
        velocityY = 0.2f;
    }
}

// ===== INIT =====
void init() {
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1, 1, 200);
    glMatrixMode(GL_MODELVIEW);
}

// ===== MAIN =====
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Endless Runner 3D - Undip");

    init();

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(keyboardNormal);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
