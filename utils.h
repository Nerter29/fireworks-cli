#ifndef UTILS_H
#define UTILS_H
#include <random>

bool isSkin(std::string text, std::string skin, std::string splitSkin);

std::tuple<int, int, int> hsvToRgb(float h, float s, float v);
float randint(float a, float b);
int sign(double x);



#endif