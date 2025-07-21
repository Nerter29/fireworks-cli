//#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <algorithm>
#include "rocket.h"
#include "fireworks.h"
#include "utils.h"
#include <chrono>

// g++ main.cpp rocket.cpp utils.cpp fireworks.cpp -o fireworks -std=c++17 && kitty ./fireworks
//x86_64-w64-mingw32-g++ main.cpp rocket.cpp utils.cpp fireworks.cpp -static -static-libgcc -static-libstdc++ -o fireworks.exe -std=c++17

void getWindowSize(int& width, int& height, std::vector<std::vector<std::string>>& screen, std::string bg){
    /*
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    width = w.ws_col / 2; // because there is spaces beetween points
    height = w.ws_row - 1;

    screen = std::vector<std::vector<std::string>>(height, std::vector<std::string>(width, bg));*/


    #ifdef _WIN32 //windows
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = (csbi.srWindow.Right - csbi.srWindow.Left + 1) / 2;
        height = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1) - 1;
    } else {
        width = 40;
        height = 20;
    }
    #else //linux
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col / 2;
    height = w.ws_row - 1;
    #endif

    screen = std::vector<std::vector<std::string>>(height, std::vector<std::string>(width, bg));

}

void resetScreen(std::vector<std::vector<std::string>>& screen, std::string bg) {
    for (int y = 0; y < screen.size(); y++) {
        screen[y].assign(screen[0].size(), bg);
    }
}


void displayScreen(const std::vector<std::vector<std::string>>& screen) {
    for (int y = 0; y < screen.size(); y++) {
        for (int x = 0; x < screen[0].size(); x++) {
            std::cout << screen[y][x] << " ";
        }
        std::cout << "\n";
    }
}

void spawnFirework(std::vector<Rocket>& rockets,int width, int height, float speed, float startH, float endH, int hDiff, float s, float v,
    std::string skin, std::string trailSkin, int trailLength, float gravity, int cooldownMS, int cooldownDiff){

    int x = randint((float)width * 2/5, (float)width * 3/5);
    int y = randint((float)height * 0.75, height);
    
    float baseRatio = (float)17/8;
    float screenRatio = (float)width / height ;
    float angleDiffMultiplier = M_PI / 8;
    float leftAngle = 3 * M_PI / 4 - angleDiffMultiplier * (baseRatio - screenRatio); //
    float rightAngle =  M_PI / 4 + angleDiffMultiplier * (baseRatio - screenRatio); //when the screen ratio is low, (width < height), the start angle for each firework decreases, bc there is not much space, vice versa
    leftAngle = std::min(11 * M_PI / 12, (double)leftAngle); //
    rightAngle = std::max( M_PI / 12, (double)rightAngle);  // set boundaries so it doesn't fire a work downside or horizontal
    float angle = -randint(rightAngle, leftAngle); // beetween 2 angles from the trigonometry circle (reversed bc we begin on the top)

    float vx = cos(angle);
    float vy = sin(angle);

    int h = randint(startH, endH);

    spawnRocket(rockets,x, y, vx, vy, speed, h, hDiff, s, v, skin, trailSkin ,trailLength, gravity, cooldownMS,
    cooldownDiff, "yes");
}



int main() {
    std::cout << "\033[?25l"; // hide cursor
    std::cout.flush();

    std::string bg = " ";

    int width, height;
    std::vector<std::vector<std::string>> screen;
    getWindowSize(width,height,screen, bg);
    std::vector<Rocket> rockets;


    int rocketNumber = 3;
    int trailLength = 50;

    int splitNumber = 30;
    int splitDiff = 2;

    float frameRate = 1.5; //decrease it to increase the frames (1 = 20 fps) (you may need to adjust the spawn cooldown)

    float speed = 0.6 * frameRate * height / 24;
    float gravity = 0.02 * frameRate;

    //colors
    int startH = 0;
    int endH = 360;
    int hDiff = 30;
    float s = 0.75;
    float v = 0.75;

    using namespace std::chrono_literals;
    auto delay = (50 * frameRate) * 1ms;
    int delayInt = static_cast<int>(delay.count());
    int cooldownMS = 2000;
    int cooldownDiff = 1000;

    std::string skin = "@";
    std::string trailSkin = "*";


    float splitSpeed = 0.4 * frameRate;
    std::string splitSkin = "#";
    std::string splitTrailSkin = "+";
    int splitTrailLength = 16;
    float splitGravity = 0.04 * frameRate;
    int splitCooldownMS = 1500;
    int splitCooldownDiff = 1000;

    float doubleSplitChance = 0.05;


    int spawnCooldown = 1250;
    int spawnCooldownDiff = 500;
    int spawnTimer = spawnCooldown;

    using namespace std::chrono;
    auto previousTime = high_resolution_clock::now();

    while (true) {
        auto nowTime = high_resolution_clock::now();
        auto loopDuration = std::chrono::duration<float, std::milli> (nowTime - previousTime);
        previousTime = nowTime;

        getWindowSize(width,height,screen, bg);

        std::cout << "\033[H\033[J";  // clear

        resetScreen(screen, bg);
        //std::cout << spawnCooldown << " c ";

        spawnTimer += loopDuration.count();
        if(spawnTimer >= spawnCooldown){
            spawnTimer = 0;
            spawnCooldown = randint(spawnCooldown - (float)spawnCooldownDiff / 2, spawnCooldown + (float)spawnCooldownDiff / 2);
            spawnFirework(rockets,width, height, speed, startH, endH,hDiff, s, v, skin, trailSkin ,trailLength, gravity, cooldownMS,
            cooldownDiff);
        }

        generateScreen(screen, rockets, delayInt, splitNumber, splitDiff,
        hDiff, s, v, skin, splitSpeed, splitSkin, splitTrailSkin, splitTrailLength, splitGravity, splitCooldownMS, splitCooldownDiff,
        doubleSplitChance);

        displayScreen(screen);

        std::this_thread::sleep_for(delay);
    }

    return 0;
}

