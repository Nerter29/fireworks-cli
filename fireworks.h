#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <algorithm>
#include <cmath>
#include <vector>

#include "rocket.h"

void spawnRocket(std::vector<Rocket>& rockets, int x, int y, float vx, float vy, float speed, float h, int hDiff, float s, float v,
    std::string skin, std::string trailSkin, int trailLength, float gravity, int cooldownMS, int cooldownDiff, std::string canSplit);

std::string getColor(bool isTrailUnit, int index, int maxIndex, std::tuple<float, float, float> color, std::string trailSkin);

void generateScreen(std::vector<std::vector<std::string>>& screen, std::vector<Rocket>& rockets, int frameDelayMS,
int splitNumber,int splitDiff, float hDiff, float s, float v, std::string skin, float splitSpeed, std::string splitSkin, std::string splitTrailSkin ,
int splitTrailLength, float splitGravity, int splitCooldown, int splitCooldownAverageDiff, int splitCooldownDiff, float doubleSplitChance);


#endif