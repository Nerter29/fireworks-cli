#include "utils.h"
#include <iostream>
#include <random>
#include <string>

bool isSkin(std::string text, std::string skin, std::string splitSkin) {
    //say if text like "\033[38;2;255;0;0m@\033[0m" contains skin
    if(text.size() == 1){
        return false;
    }
    for (int i = text.size() - 1; i >= 0; --i) {
        std::string characterStr(1, text[i]);

        if (characterStr == skin || characterStr == splitSkin) {
            return true;
        }
    }
    return false;
}

std::tuple<int, int, int> hsvToRgb(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - std::fabs(std::fmod(h / 60.0, 2) - 1));
    float m = v - c;

    float r_, g_, b_;

    if (h < 60)       { r_ = c; g_ = x; b_ = 0; }
    else if (h < 120) { r_ = x; g_ = c; b_ = 0; }
    else if (h < 180) { r_ = 0; g_ = c; b_ = x; }
    else if (h < 240) { r_ = 0; g_ = x; b_ = c; }
    else if (h < 300) { r_ = x; g_ = 0; b_ = c; }
    else              { r_ = c; g_ = 0; b_ = x; }

    int r = static_cast<int>((r_ + m) * 255);
    int g = static_cast<int>((g_ + m) * 255);
    int b = static_cast<int>((b_ + m) * 255);

    return {r, g, b};
}

float randint(float a, float b){
    static std::random_device rd;
    static std::mt19937 gen(rd());  // 🔒 Initialisé une seule fois
    std::uniform_real_distribution<> dist(a, b);
    return dist(gen);
}


int sign(double x) {
    return (x > 0) - (x < 0);
}