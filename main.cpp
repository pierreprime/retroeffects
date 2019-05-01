#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "quickcg.h"
using namespace QuickCG;

// define the width and height of screen and buffers
const int screenWidth = 640;
const int screenHeight = 128;

// Y-coordinate first because horizontal scanlines are wanted
Uint32 fire[screenHeight][screenWidth]; // buffer containing the fire
Uint32 buffer[screenHeight][screenWidth]; // buffer to be drawn
Uint32 palette[256]; // contains color palette

int main(int argc, char *argv[])
{
    screen(screenWidth, screenHeight, 0, "fire");

    // declarations
    ColorRGB color;
    float time = getTime(), oldTime; // time of this frame and the previous one

    // buffer at 0 in beginning
    for(int y=0; y<h; y++)
        for(int x=0; x<w; x++)
            fire[y][x] = 0;

    // generate palette
    for(int x=0; x < 256; x++){
        // HSLtoRGB used to generate colors
        // Hues goes from 0 to 85: red to yellow
        // Saturation is at maximum : 255
        // Lightness is 0..255 for x=0..128 and 255 for x=128..255
        color = HSLtoRGB(ColorHSL(x / 3, 255, std::min(255, x * 2)));
        // set palette to calculated RGB
        palette[x] = RGBtoINT(color);
    }

    // start loop (one frame per loop)
    while(!done()){
        // timing : set to maximum 50 ms per frame = 20 frames per second
        oldTime = time;
        waitFrame(oldTime, 0.05);
        time = getTime();

        // randomize bottom row of fire buffer
        for(int x = 0; x < w; x++)
            fire[h - 1][x] = abs(32768 + rand()) % 256;

        // do fire calculations for every pixel, from top to bottom
        for(int y=0; y < h - 1; y++){
            for(int x=0; x < w; x++){
                fire[y][x] =
                    ((fire[(y+1) % h][(x - 1 + w) % w]
                    + fire[(y + 1) % h][(x) % w]
                    + fire[(y + 1) % h][(x + 1) % w]
                    + fire[(y + 2) % h][(x) % w])
                    * 32) / 129;
            }
        }

        // set drawing buffer to the fire buffer, using palette colors
        for(int y=0; y < h; y++)
            for(int x=0; x < w; x++)
                buffer[y][x] = palette[fire[y][x]];

        // draw buffer && redraw screen
        drawBuffer(buffer[0]);
        redraw();
    }
}
