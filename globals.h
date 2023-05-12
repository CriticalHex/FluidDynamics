#pragma once
#include<SFML/Graphics.hpp>

const int winX = 1920;
const int winY = 1080;
static int index(int x, int y) { return x + y * winY; }
const int solveIterations = 16;