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
/*
cd 0-documents/0-perso/0-projets-executables/cpp/fireworks
g++ main.cpp rocket.cpp utils.cpp fireworks.cpp -o fireworks -std=c++17 && ./fireworks
*/
//x86_64-w64-mingw32-g++ main.cpp rocket.cpp utils.cpp fireworks.cpp -static -static-libgcc -static-libstdc++ -o fireworks.exe -std=c++17

void getWindowSize(int& width, int& height, std::vector<std::vector<std::string>>& screen, std::string bg){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col / 2;
    height = w.ws_row - 1;

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


    int baseSpawnCooldown = 1250;
    int spawnCooldown = baseSpawnCooldown;
    int spawnCooldownDiff = 250;
    int spawnTimer = baseSpawnCooldown;

    using namespace std::chrono;
    auto previousTime = high_resolution_clock::now();
    auto nowTime = high_resolution_clock::now();
    std::chrono::duration<float, std::milli>  remaining = 50ms;
    while (true) {
        //make a realistic state of how much time has elapsed since last time to correctrly add it to spawnTimer
        nowTime = high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float, std::milli> (nowTime - previousTime).count();
        previousTime = nowTime;

        std::cout << "\033[H\033[J";  // clear
        std::cout << static_cast<int>(remaining.count()) << " " << elapsedTime<< "    ";

        getWindowSize(width,height,screen, bg);

        resetScreen(screen, bg);

        spawnTimer += elapsedTime;
        if(spawnTimer >= spawnCooldown){
            spawnTimer = 0;
            spawnCooldown = randint(baseSpawnCooldown - spawnCooldownDiff, baseSpawnCooldown + spawnCooldownDiff);
            spawnFirework(rockets,width, height, speed, startH, endH,hDiff, s, v, skin, trailSkin ,trailLength, gravity, cooldownMS,
            cooldownDiff);
        }

        generateScreen(screen, rockets, delayInt, splitNumber, splitDiff,
        hDiff, s, v, skin, splitSpeed, splitSkin, splitTrailSkin, splitTrailLength, splitGravity, splitCooldownMS, splitCooldownDiff,
        doubleSplitChance);

        displayScreen(screen);

        //see the time that all the shit above took to substract it from the current delay so we always wait the right amount of time beetween every frames
        float executionTime = std::chrono::duration<float, std::milli> (high_resolution_clock::now() - previousTime).count();
        remaining = delay - milliseconds((int)executionTime);
        if (remaining > milliseconds(0))
            std::this_thread::sleep_for(remaining);
        
    }

    return 0;
}

