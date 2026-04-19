/*
 * Computer Graphics Project: Interactive Living Room
 * Improved Version - Beautiful Lamp & Cute Cat
 * OpenGL + C++
 */

#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <cstdio>

using namespace std;

// ─────────────────────────────────────────────
// Global state
// ─────────────────────────────────────────────
bool lampOn = true;
float ballX = 850.0f;
float ballY = 470.0f;
float tailAngle = 0.0f;
float tailDir = 1.0f;
int clockSeconds = 0;

const int WIN_W = 1300;
const int WIN_H = 820;

// ─────────────────────────────────────────────
// Color helpers
// ─────────────────────────────────────────────
void setColor(float r, float g, float b, float alpha = 1.0f) {
    glColor4f(r, g, b, alpha);
}

void roomColor(float r, float g, float b, float darkFactor = 0.18f) {
    if (lampOn)
        setColor(r, g, b);
    else
        setColor(r * darkFactor, g * darkFactor, b * darkFactor);
}

// ─────────────────────────────────────────────
// Midpoint Circle Algorithm
// ─────────────────────────────────────────────
void putPixel(int x, int y) { glVertex2i(x, y); }

void drawMidpointCircle(int cx, int cy, int radius) {
    glBegin(GL_LINES);
    for (int row = -radius; row <= radius; ++row) {
        int maxX = (int)sqrt((double)(radius * radius - row * row));
        glVertex2i(cx - maxX, cy + row);
        glVertex2i(cx + maxX, cy + row);
    }
    glEnd();
}

void drawMidpointCircleOutline(int cx, int cy, int radius) {
    int x = 0, y = radius, d = 1 - radius;
    glBegin(GL_POINTS);
    while (x <= y) {
        putPixel(cx + x, cy + y); putPixel(cx - x, cy + y);
        putPixel(cx + x, cy - y); putPixel(cx - x, cy - y);
        putPixel(cx + y, cy + x); putPixel(cx - y, cy + x);
        putPixel(cx + y, cy - x); putPixel(cx - y, cy - x);
        if (d < 0) d += 2 * x + 3;
        else { d += 2 * (x - y) + 5; --y; }
        ++x;
    }
    glEnd();
}

// ─────────────────────────────────────────────
// Basic primitives
// ─────────────────────────────────────────────
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2, float thick = 2.0f) {
    glLineWidth(thick);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
}

// ─────────────────────────────────────────────
// ROOM BACKGROUND
// ─────────────────────────────────────────────
void drawBackground() {
    // Wall
    roomColor(0.878f, 0.867f, 0.773f);
    drawRect(0, 0, WIN_W, WIN_H);
    // Floor
    roomColor(0.60f, 0.60f, 0.60f);
    drawRect(0, 0, WIN_W, 80);
}

// ─────────────────────────────────────────────
// WALL CLOCK
// ─────────────────────────────────────────────
void drawClock() {
    int cx = 165, cy = 630, R = 58;

    // Clock face
    roomColor(1.0f, 1.0f, 1.0f);
    drawMidpointCircle(cx, cy, R);

    // Border
    for (int r = R; r > R - 5; --r) {
        roomColor(0.18f, 0.31f, 0.38f);
        drawMidpointCircleOutline(cx, cy, r);
    }

    // Hour markers
    for (int h = 0; h < 12; ++h) {
        float ang = h * 30.0f * M_PI / 180.0f;
        float x1 = cx + (R - 8) * sin(ang);
        float y1 = cy + (R - 8) * cos(ang);
        float x2 = cx + (R - 14) * sin(ang);
        float y2 = cy + (R - 14) * cos(ang);
        roomColor(0.18f, 0.31f, 0.38f);
        drawLine(x1, y1, x2, y2, 2.5f);
    }

    // Hands (fixed ~10:10)
    roomColor(0.1f, 0.1f, 0.1f);
    float hAng = (10 * 30) * M_PI / 180.0f;
    drawLine(cx, cy, cx + (R - 22) * sin(hAng), cy + (R - 22) * cos(hAng), 4.0f);

    float mAng = (2 * 30) * M_PI / 180.0f;
    drawLine(cx, cy, cx + (R - 14) * sin(mAng), cy + (R - 14) * cos(mAng), 3.0f);

    // Animated seconds hand
    float sAng = clockSeconds * 6.0f * M_PI / 180.0f;
    roomColor(0.85f, 0.15f, 0.15f);
    drawLine(cx, cy, cx + (R - 8) * sin(sAng), cy + (R - 8) * cos(sAng), 1.5f);

    // Center dot
    roomColor(0.1f, 0.1f, 0.1f);
    drawMidpointCircle(cx, cy, 4);
}

// ─────────────────────────────────────────────
// SOFA
// ─────────────────────────────────────────────
void drawSofa() {
    roomColor(0.95f, 0.75f, 0.05f);
    drawRect(30, 200, 240, 200);      // back
    drawRect(40, 150, 220, 60);       // seat

    roomColor(0.85f, 0.25f, 0.10f);
    drawRect(40, 150, 220, 20);       // stripe

    roomColor(0.15f, 0.15f, 0.15f);
    drawRect(20, 150, 260, 30);       // top rail
    drawRect(20, 150, 20, 300);       // left arm
    drawRect(260, 150, 20, 300);      // right arm

    // legs
    drawRect(40, 80, 18, 75);
    drawRect(240, 80, 18, 75);
}

// ─────────────────────────────────────────────
// BEAUTIFUL CAT
// ─────────────────────────────────────────────
void drawCat() {
    float bx = 185.0f, by = 205.0f;

    // Body (dark gray)
    roomColor(0.22f, 0.22f, 0.25f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(bx, by + 32);
    for (int i = 0; i <= 40; ++i) {
        float a = i * 9.0f * M_PI / 180.0f;
        glVertex2f(bx + 38 * cos(a), by + 32 + 26 * sin(a));
    }
    glEnd();

    // White belly
    roomColor(0.92f, 0.90f, 0.85f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(bx, by + 25);
    for (int i = 0; i <= 36; ++i) {
        float a = i * 10.0f * M_PI / 180.0f;
        glVertex2f(bx + 24 * cos(a), by + 22 + 18 * sin(a));
    }
    glEnd();

    // Head
    roomColor(0.22f, 0.22f, 0.25f);
    drawMidpointCircle((int)(bx + 32), (int)(by + 68), 26);

    // White snout
    roomColor(0.92f, 0.88f, 0.82f);
    drawMidpointCircle((int)(bx + 37), (int)(by + 55), 13);

    // Ears
    roomColor(0.22f, 0.22f, 0.25f);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx + 12, by + 85); glVertex2f(bx + 18, by + 105); glVertex2f(bx + 31, by + 88);
    glVertex2f(bx + 45, by + 85); glVertex2f(bx + 55, by + 104); glVertex2f(bx + 62, by + 87);
    glEnd();

    // Pink inner ears
    roomColor(0.95f, 0.65f, 0.70f);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx + 17, by + 89); glVertex2f(bx + 22, by + 98); glVertex2f(bx + 29, by + 90);
    glVertex2f(bx + 48, by + 89); glVertex2f(bx + 53, by + 98); glVertex2f(bx + 59, by + 90);
    glEnd();

    // Eyes
    roomColor(0.95f, 0.85f, 0.15f);
    drawMidpointCircle((int)(bx + 22), (int)(by + 70), 7);
    drawMidpointCircle((int)(bx + 45), (int)(by + 70), 7);

    roomColor(0.08f, 0.08f, 0.10f);
    drawMidpointCircle((int)(bx + 23), (int)(by + 69), 4);
    drawMidpointCircle((int)(bx + 44), (int)(by + 69), 4);

    // Eye shine
    roomColor(1.0f, 1.0f, 1.0f, 0.9f);
    drawMidpointCircle((int)(bx + 20), (int)(by + 73), 2);
    drawMidpointCircle((int)(bx + 41), (int)(by + 73), 2);

    // Nose
    roomColor(0.95f, 0.45f, 0.50f);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx + 33, by + 57);
    glVertex2f(bx + 29, by + 52);
    glVertex2f(bx + 37, by + 52);
    glEnd();

    // Whiskers
    roomColor(0.95f, 0.95f, 0.95f);
    drawLine(bx + 8, by + 57, bx + 24, by + 59, 1.1f);
    drawLine(bx + 8, by + 53, bx + 24, by + 54, 1.1f);
    drawLine(bx + 8, by + 61, bx + 23, by + 63, 1.1f);

    drawLine(bx + 55, by + 57, bx + 40, by + 59, 1.1f);
    drawLine(bx + 55, by + 53, bx + 40, by + 54, 1.1f);
    drawLine(bx + 56, by + 61, bx + 41, by + 63, 1.1f);

    // ── Smooth Tail ──
    float tx = bx - 32.0f, ty = by + 28.0f;
    float rad = tailAngle * M_PI / 180.0f;
    float tx2 = tx - 62 * cos(rad);
    float ty2 = ty + 58 * sin(rad);

    roomColor(0.20f, 0.20f, 0.23f);
    glLineWidth(9.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; ++i) {
        float t = i / 20.0f;
        float px = (1 - t) * (1 - t) * tx + 2 * (1 - t) * t * (tx - 38) + t * t * tx2;
        float py = (1 - t) * (1 - t) * ty + 2 * (1 - t) * t * (ty + 28) + t * t * ty2;
        glVertex2f(px, py);
    }
    glEnd();
    glLineWidth(1.0f);

    // Tail tip
    roomColor(0.45f, 0.45f, 0.48f);
    drawMidpointCircle((int)tx2, (int)ty2, 7);
}

// ─────────────────────────────────────────────
// IMPROVED LAMP with beautiful light
// ─────────────────────────────────────────────
void drawLamp() {
    // Pole
    roomColor(0.18f, 0.18f, 0.18f);
    drawRect(330, 80, 12, 430);

    // Base
    roomColor(0.18f, 0.18f, 0.18f);
    drawRect(315, 78, 42, 12);

    // Shade
    if (lampOn)
        setColor(0.95f, 0.78f, 0.45f);
    else
        setColor(0.85f * 0.18f, 0.72f * 0.18f, 0.40f * 0.18f);

    glBegin(GL_TRIANGLES);
    glVertex2f(275, 415);
    glVertex2f(400, 415);
    glVertex2f(337.5f, 525);
    glEnd();

    if (lampOn) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Light cone on sofa
        setColor(1.0f, 0.85f, 0.45f, 0.25f);
        glBegin(GL_TRIANGLES);
        glVertex2f(337, 415);
        glVertex2f(160, 190);
        glVertex2f(480, 190);
        glEnd();

        // Soft radial glow
        for (int i = 1; i <= 18; ++i) {
            float r = i * 22.0f;
            float alpha = 0.085f * (1.0f - i / 18.0f) * (i < 8 ? 1.8f : 1.0f);
            setColor(1.0f, 0.92f, 0.65f, alpha);
            drawMidpointCircle(337, 405, (int)r);
        }

        // Bright center
        for (int i = 1; i <= 8; ++i) {
            float alpha = 0.15f * (1.0f - i / 8.0f);
            setColor(1.0f, 0.98f, 0.75f, alpha);
            drawMidpointCircle(337, 400, i * 12);
        }

        glDisable(GL_BLEND);
    }
}

// ─────────────────────────────────────────────
// TV UNIT (unchanged for brevity - same as original)
// ─────────────────────────────────────────────
void drawTVUnit() {
    float ux = 420.0f, uy = 0.0f;

    roomColor(0.92f, 0.72f, 0.05f);
    drawRect(ux + 100, uy + 80, 180, 65);
    drawRect(ux + 310, uy + 80, 180, 65);

    roomColor(0.15f, 0.15f, 0.15f);
    drawRect(ux + 168, uy + 108, 40, 8);
    drawRect(ux + 378, uy + 108, 40, 8);

    roomColor(0.15f, 0.15f, 0.15f);
    drawRect(ux + 90, uy + 80, 10, 580);
    drawRect(ux + 600, uy + 80, 10, 580);
    drawRect(ux + 90, uy + 655, 520, 10);

    roomColor(0.92f, 0.72f, 0.05f);
    drawRect(ux + 100, uy + 450, 200, 200);
    roomColor(0.15f, 0.15f, 0.15f);
    drawRect(ux + 186, uy + 545, 28, 8);

    roomColor(0.94f, 0.94f, 0.88f);
    drawRect(ux + 300, uy + 560, 300, 14);

    // Decorations (same as original)
    roomColor(0.95f, 0.75f, 0.15f);
    drawRect(ux + 330, uy + 574, 20, 35);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ux + 340, uy + 614);
    for (int i = 0; i <= 18; i++) {
        float a = i * 20.0f * M_PI / 180.0f;
        glVertex2f(ux + 340 + 12 * cos(a), uy + 614 + 8 * sin(a));
    }
    glEnd();

    roomColor(0.85f, 0.40f, 0.10f);
    drawMidpointCircle((int)(ux + 430), (int)(uy + 582), 14);

    roomColor(0.70f, 0.15f, 0.10f);
    drawRect(ux + 480, uy + 574, 22, 30);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ux + 491, uy + 608);
    for (int i = 0; i <= 18; i++) {
        float a = i * 20.0f * M_PI / 180.0f;
        glVertex2f(ux + 491 + 13 * cos(a), uy + 608 + 6 * sin(a));
    }
    glEnd();

    // Plant
    roomColor(0.80f, 0.45f, 0.20f);
    drawRect(ux + 170, uy + 648, 40, 30);
    roomColor(0.25f, 0.55f, 0.20f);
    glBegin(GL_TRIANGLES);
    glVertex2f(ux + 190, uy + 678); glVertex2f(ux + 165, uy + 730); glVertex2f(ux + 182, uy + 730);
    glVertex2f(ux + 190, uy + 678); glVertex2f(ux + 200, uy + 735); glVertex2f(ux + 215, uy + 720);
    glVertex2f(ux + 190, uy + 678); glVertex2f(ux + 178, uy + 748); glVertex2f(ux + 200, uy + 745);
    glEnd();

    // Bookshelf (simplified - you can keep full original if you want)
    roomColor(0.15f, 0.15f, 0.15f);
    drawRect(ux + 90, uy + 350, 210, 10);
    drawRect(ux + 90, uy + 200, 210, 10);

    float bkx = ux + 105;
    float bookColors[6][3] = {
        {0.55f,0.25f,0.15f},{0.35f,0.55f,0.25f},{0.25f,0.35f,0.60f},
        {0.65f,0.15f,0.15f},{0.25f,0.55f,0.45f},{0.70f,0.55f,0.15f}
    };
    for (int i = 0; i < 6; ++i) {
        roomColor(bookColors[i][0], bookColors[i][1], bookColors[i][2]);
        drawRect(bkx + i * 25, uy + 360, 22, 80);
    }

    roomColor(0.92f, 0.72f, 0.05f);
    drawRect(ux + 610, uy + 80, 160, 575);
    roomColor(0.15f, 0.15f, 0.15f);
    drawRect(ux + 678, uy + 360, 28, 8);
}

// ─────────────────────────────────────────────
// TV + Moving Ball
// ─────────────────────────────────────────────
void drawTV() {
    float ux = 420.0f;

    roomColor(0.12f, 0.12f, 0.12f);
    drawRect(ux + 295, 145, 310, 230);

    if (lampOn)
        setColor(0.62f, 0.62f, 0.65f);
    else
        setColor(0.08f, 0.08f, 0.10f);
    drawRect(ux + 305, 155, 290, 210);

    if (lampOn) {
        setColor(0.80f, 0.80f, 0.85f, 0.35f);
        glBegin(GL_TRIANGLES);
        glVertex2f(ux + 370, 340); glVertex2f(ux + 430, 360); glVertex2f(ux + 580, 160);
        glVertex2f(ux + 430, 360); glVertex2f(ux + 580, 160); glVertex2f(ux + 560, 155);
        glEnd();
    }

    roomColor(0.12f, 0.12f, 0.12f);
    drawRect(ux + 430, 80, 30, 70);
    drawRect(ux + 400, 78, 90, 16);

    // Moving Ball
    float sx1 = ux + 315, sy1 = 165, sx2 = ux + 585, sy2 = 355;
    if (ballX < sx1 + 10) ballX = sx1 + 10;
    if (ballX > sx2 - 10) ballX = sx2 - 10;
    if (ballY < sy1 + 10) ballY = sy1 + 10;
    if (ballY > sy2 - 10) ballY = sy2 - 10;

    setColor(0.20f, 0.20f, 0.25f, 0.5f);
    drawMidpointCircle((int)ballX + 4, (int)ballY - 4, 13);

    setColor(0.95f, 0.25f, 0.25f);
    drawMidpointCircle((int)ballX, (int)ballY, 14);
    setColor(1.0f, 0.70f, 0.70f);
    drawMidpointCircle((int)(ballX - 4), (int)(ballY + 4), 5);
}

// ─────────────────────────────────────────────
// DISPLAY
// ─────────────────────────────────────────────
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawBackground();
    drawSofa();
    drawCat();
    drawLamp();
    drawTVUnit();
    drawTV();
    drawClock();

    // HUD
    glColor3f(0.2f, 0.2f, 0.2f);
    glRasterPos2f(10, WIN_H - 20);
    const char* msg1 = "L = Toggle Lamp | Arrow Keys = Move Ball on TV";
    for (const char* c = msg1; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    if (lampOn) setColor(0.85f, 0.65f, 0.10f);
    else setColor(0.80f, 0.30f, 0.30f);
    glRasterPos2f(10, WIN_H - 38);
    const char* msg2 = lampOn ? "Lamp: ON" : "Lamp: OFF";
    for (const char* c = msg2; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    glutSwapBuffers();
}

// ─────────────────────────────────────────────
// TIMER
// ─────────────────────────────────────────────
void timer(int) {
    tailAngle += 1.8f * tailDir;
    if (tailAngle > 38.0f) tailDir = -1.0f;
    if (tailAngle < -38.0f) tailDir = 1.0f;

    time_t t = time(nullptr);
    struct tm* tm_info = localtime(&t);
    clockSeconds = tm_info->tm_sec;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ─────────────────────────────────────────────
// KEYBOARD
// ─────────────────────────────────────────────
void keyboard(unsigned char key, int, int) {
    if (key == 'l' || key == 'L') lampOn = !lampOn;
    if (key == 27) exit(0);
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    const float step = 8.0f;
    if (key == GLUT_KEY_LEFT) ballX -= step;
    if (key == GLUT_KEY_RIGHT) ballX += step;
    if (key == GLUT_KEY_UP) ballY += step;
    if (key == GLUT_KEY_DOWN) ballY -= step;
    glutPostRedisplay();
}

// ─────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CG Project - Interactive Living Room");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);

    glPointSize(1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
