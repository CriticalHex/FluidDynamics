#pragma once

void step(float cVelocityX[], float cVelocityY[], float lVelocityX[], float lVelocityY[], float cDensity[], float lDensity[], float diffusion, float viscosity, float dt);
void diffuse(int b, float arrY[], float arrX[], float diffusion, float dt);
void advect(int axis, float arr1x[], float arr1y[], float arr2x[], float arr2y[], float dt);
void linearSolve(int b, float arrX[], float arrY[], float a, float c);
void project(float velocityX1[], float velocityY1[], float velocityX2[], float velocityY2[]);
void setBound(int axis, float arr[]);
