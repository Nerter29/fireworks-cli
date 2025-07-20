#include "rocket.h"
#include <cmath>
#include <iostream>
//#include <iostream>
#include "utils.h"
#include "fireworks.h"

//constructor
Rocket::Rocket(float x, float y, float vx, float vy, float speed, float h, int hDiff, float s, float v, std::string skin, std::string trailSkin, int trailLength,
float gravity, int cooldownMS, std::string canSplit):
    x(x), y(y), intX(std::round(x)), intY(std::round(y)), vx(vx), vy(vy), speed(speed), color({h, s ,v}), hDiff(hDiff),
    skin(skin), trailSkin(trailSkin), trailLength(trailLength), isOut(false), gravity(gravity), cooldownMS(cooldownMS),timer(0), canSplit(canSplit){}


void Rocket::move() {
    x += vx * speed;
    y += vy * speed;

    //gravity
    vy += gravity;
    
    intX = std::round(x);
    intY = std::round(y);
}

void Rocket::initTrail(){
    for (int i = 0; i < trailLength; i++){
        trail.push_back({intX, intY});
    }
}
void Rocket::updateTrail(){
    if(!isOut){
        trail.push_back({intX, intY});
    }
    if(trail.size() >= 1){
        trail.erase(trail.begin());

    }



}

void Rocket::split(std::vector<Rocket>& rockets, int splitNumber, int splitDiff, int width, int height, float nhDiff
    , float ns, float nv, float splitSpeed, std::string splitSkin, std::string splitTrailSkin, int splitTrailLength, float splitGravity,
     int splitCooldownMS, int splitCooldownDiff, float doubleSplitChance){

    //remove current rocket
    isOut = true;
      

    int n = splitNumber  + std::round(randint(-splitDiff,splitDiff));

    int nx = x;
    int ny = y;
    float baseH = std::get<0>(color);

    std::string ncanSplit = "no";
    if(randint(0, 100) <= doubleSplitChance * 100 && canSplit != "yes but only once"){
        ncanSplit = "yes but only once";
    }


    for (int i = 0; i < n; i++){
        int midx = width / 2;

        float angle = randint(0, 2 * M_PI);
        float nvx = cos(angle);
        float nvy = sin(angle);


        int h = (((int)randint(baseH - nhDiff, baseH + nhDiff) % 360 + 360) % 360);

        std::string currentCanSplit = ncanSplit;
        if(i % 6 != 0){
            currentCanSplit = "no"; // regulate the doubleSplit
        }
        spawnRocket(rockets, nx, ny, nvx, nvy, splitSpeed, h, 0, ns, nv,
              splitSkin,  splitTrailSkin,  splitTrailLength,  splitGravity,  splitCooldownMS,
               splitCooldownDiff, currentCanSplit);
    }

    
}
