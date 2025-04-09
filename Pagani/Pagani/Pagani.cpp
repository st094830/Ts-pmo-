#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
void usleep(__int64 usec) {
    HANDLE timer;
    LARGE_INTEGER ft;
    ft.QuadPart = -(10 * usec);
    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif

using namespace std;

float A = 0, B = 0, C = 0;
float cube_param = 20;
int width = 100, height = 74; 
float zBuffer[100 * 74];
char buffer[100 * 74];
int backgroundASCII = ' ';
int distfromCam = 100;
float K1 = 20;  






float calculateX(float i, float j, float k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
        j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(float i, float j, float k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
        j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
        i * cos(B) * sin(C);
}

float calculateZ(float i, float j, float k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculatePoint(float x, float y, float z, int ch) {
    float x_ = calculateX(x, y, z);
    float y_ = calculateY(x, y, z);
    float z_ = calculateZ(x, y, z) + distfromCam;
    float ooz = 1 / z_;
    int xp = (int)(width / 2 + x_ * K1 * ooz);
    int yp = (int)(height / 2 - y_ * K1 * ooz);
    int idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

void drawTVScreen(float i, float j) {
    float screenSize = cube_param*2 * 0.8;

    float xPos = (i + screenSize) / (2 * screenSize);
    float yPos = (j*2 + screenSize) / (2 * screenSize);

    if (xPos > 0.6 && yPos > 0.6) {
        calculatePoint(i, j*2, -cube_param*2, '█');
    }

    else if (xPos < 0.4 && yPos < 0.4) {
        calculatePoint(i, j*2, -cube_param*2, '.');
    }

    else {
        calculatePoint(i, j*2, -cube_param*2, '▒');
    }

    if (j < -screenSize * 0.9 && j > -screenSize && abs(i) < screenSize * 0.3) {
        calculatePoint(i, j*2, -cube_param*2, 'o');
    }
}


int main() {
    cout << "\x1b[2J";  
    while (true) {
        cout << "\x1b[H"; 

        memset(buffer, backgroundASCII, width * height);
        memset(zBuffer, 0, width * height * 4);

        for (float i = -cube_param; i < cube_param; i += 0.6) {
            for (float j = -cube_param; j < cube_param; j += 0.6) {
                if (i > -cube_param * 0.8 && i < cube_param * 0.8 &&
                    j > 0 && j < cube_param * 0.8) {
                    drawTVScreen(i, j);
                }
                else {
                    calculatePoint(i, j * 2, -cube_param * 2, '@'); 
                }
                
                calculatePoint(cube_param, j*2, i*2, '$');
                calculatePoint(-cube_param, j*2, i*2, '~');
                calculatePoint(i, j*2, cube_param*2, '#');
                calculatePoint(i, -cube_param*2, j*2, ';');
                calculatePoint(i, cube_param*2, j*2, '+');
            }
        }

        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : '\n');
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
    }
    return 0;
}