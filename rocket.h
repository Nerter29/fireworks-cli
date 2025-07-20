#ifndef ROCKET_H
#define ROCKET_H

#include <string>
#include <vector>

class Rocket {
public:
    float x;
    float y;
    int intX;
    int intY; 
    float vx;
    float vy;
    float speed;
    std::string skin;
    std::string trailSkin;
    std::tuple<float, float, float> color;
    int hDiff;
    std::vector<std::vector<int>> trail;
    int trailLength;
    float gravity;
    int timer;
    int cooldownMS;
    std::string canSplit;
    bool isOut;


    Rocket(float x, float y, float vx, float vy, float speed, float h , int hDiff, float s, float v, std::string skin, std::string trailSkin, int trailLength,
    float gravity, int cooldownMS, std::string canSplit);

    void move();
    void initTrail();
    void updateTrail();
    void split(std::vector<Rocket>& rockets, int splitNumber, int splitDiff, int width, int height, float nhDiff,
    float ns, float nv, float splitSpeed, std::string splitSkin, std::string splitTrailSkin, int splitTrailLength, float splitGravity,
    int splitCooldownMS, int splitCooldownDiff, float doubleSplitChance);

};

#endif // ROCKET_H
