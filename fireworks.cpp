#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"
#include "rocket.h"
#include "fireworks.h"




void spawnRocket(std::vector<Rocket>& rockets, int x, int y, float vx, float vy, float speed, float h, int hDiff, float s, float v,
    std::string skin, std::string trailSkin, int trailLength, float gravity, int cooldownMS, int cooldownDiff, std::string canSplit){

    int cooldown = randint(cooldownMS - (float)cooldownDiff / 2, cooldownMS + (float)cooldownDiff / 2);
    
    Rocket rocket = Rocket(x, y, vx, vy, speed, h, hDiff, s, v, skin, trailSkin, trailLength, gravity, cooldown, canSplit);
    rocket.initTrail();

    rockets.push_back(rocket);
}



std::string getColor(bool isTrailUnit, int index, int maxIndex, std::tuple<float, float, float> color, std::string trailSkin){
    //get the colored string corresponding to the ratio of index / maxIndex, on a color gradient
    float lunimosityDiff = 0.2;
    int hueDiff = 3;

    float h = std::get<0>(color);
    float s = std::get<1>(color);
    float v = std::get<2>(color);

    float ratio = (float)index / maxIndex * (1 - lunimosityDiff);
    if(isTrailUnit){
        v = (float)v * ratio ;
        h = (int)((maxIndex - index) * hueDiff + h) % 360;
    }
    
    auto [r, g, b] = hsvToRgb(h, s, v);

    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + trailSkin + "\033[0m";

}

std::string getAsciiChar(int index, int maxIndex){
    //get the ascii character corresponding to the ratio of index / maxIndex, on the ascii gradient
    std::vector<std::string> gradient = {";", ":", "-","~", "+", "*", "^"};

    int n = gradient.size() - 1;
    int gradientIndex = std::round(((float)index /maxIndex) * n);
    return gradient[gradientIndex];
}

void generateScreen(std::vector<std::vector<std::string>>& screen, std::vector<Rocket>& rockets, int frameDelayMS,
int splitNumber,int splitDiff, float hDiff, float s, float v, std::string skin, float splitSpeed, std::string splitSkin, std::string splitTrailSkin ,
int splitTrailLength, float splitGravity, int splitCooldown, int splitCooldownAverageDiff, int splitCooldownDiff, float doubleSplitChance) {
    //create the content on the screen based on the current position of every elements
    //this is the main function where we update everything.


    int width = screen[0].size();
    int height = screen.size();

    std::vector<int> rocketsToErase; // list to remember the rockets to kill because we cant do it in the loop, we have to mark them.

    for (int i = 0; i < rockets.size(); ++i) { //i = index of the rockets
        Rocket& r = rockets[i];

        if(!r.isOut){ // it moves only if he is in the bounds
            r.move();
        } 
        else{
            r.skin = " ";
        }

        int trailLength = r.trail.size() - 1; // variable trail lenth (if the rocket is out of bouds, its trail shrinks gradualy so the length is not constant)

        for (int j = trailLength; j > 0 ; j--){ // j = index of the trailUnits
            std::vector<int> trailUnit = r.trail[j]; // trailUnit = {x, y}
            int x = trailUnit[0];
            int y = trailUnit[1];
            //std::cout << isSkin(screen[y][x], skin);
            if(x < width && y < height){
                if (!isSkin(screen[y][x], skin, splitSkin)){ // so the heads are always over the trails
                    //display trail
                    screen[y][x] = getColor(true, j, r.trailLength - 1, r.color, r.trailSkin); // we take the initial traiLength so we keep a coherant trail when the roket is out of bounds
                }
            }
        }

        if(r.intX >= 0 && r.intX < width && r.intY >= 0 && r.intY < height){ // if the rocket is in bounds
             //display head
             screen[r.intY][r.intX] = getColor(false, 0, r.trailLength - 1, r.color, r.skin);// we move it on the screen
            } 
        else { //else
            r.isOut = true; //we mark it as out and we don't move it further, but we keep the trail going for a bit
        } 

        if(trailLength <= 0){ //if the trail is empty, we add the rocket to the kill list
            rocketsToErase.push_back(i);
        }

        r.updateTrail();

        r.timer += frameDelayMS;

        
        if(r.timer >= r.cooldownMS && !r.isOut){
           //std::cout << rockets.size() << " " << std::get<0>(r.color) << " &\n";
            if(r.canSplit == "yes" || r.canSplit == "yes but only once"){
                r.timer = 0;
                int splitCooldownAverage = randint(splitCooldown - splitCooldownAverageDiff, splitCooldown + splitCooldownAverageDiff); // to have small fireworks and big fireworks
                r.split(rockets, splitNumber, splitDiff, width, height, hDiff, s, v, splitSpeed, splitSkin,
                splitTrailSkin ,splitTrailLength, splitGravity, splitCooldownAverage, splitCooldownDiff, doubleSplitChance);
            }
            else{
                r.isOut = true;
            }
        }  
        //std::cout << rockets.size() << " "; 
    }

    std::sort(rocketsToErase.begin(), rocketsToErase.end(), std::greater<int>()); // so there's no index error
    for (int k : rocketsToErase){ // we proceed to the killing of the rockets on the list
        rockets.erase(rockets.begin() + k);
    }
    rocketsToErase.clear();
}