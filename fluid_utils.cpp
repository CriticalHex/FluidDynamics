#include"globals.h"
#include"fluid_utils.h"

void step(float cVelocityX[], float cVelocityY[], float lVelocityX[], float lVelocityY[], float cDensity[], float lDensity[], float diffusion, float viscosity, float dt) {
    diffuse(1, lVelocityY, cVelocityX, viscosity, dt);
    diffuse(2, lVelocityY, cVelocityY, viscosity, dt);

    project(lVelocityY, lVelocityY, cVelocityX, cVelocityY);

    advect(1, cVelocityX, lVelocityY, lVelocityY, lVelocityY, dt);
    advect(2, cVelocityY, lVelocityY, lVelocityY, lVelocityY, dt);

    project(cVelocityX, cVelocityY, lVelocityY, lVelocityY);
    diffuse(0, lDensity, cDensity, diffusion, dt);
    advect(0, cDensity, lDensity, cVelocityX, cVelocityY, dt);
}

void diffuse(int b, float arrY[], float arrX[], float diffusion, float dt) {
    float a = dt * diffusion * (winX - 2) * (winY - 2);
    linearSolve(b, arrY, arrX, a, 1 + 6 * a);
}

void advect(int axis, float arr1x[], float arr1y[], float arr2x[], float arr2y[], float dt) {
    float dx = dt * (winX - 2);
    float dy = dt * (winY - 2);
    // actual nonsense, this is just calculating what index is affected by a pixels velocity
    float s0, s1, t0, t1;
    float dvx, dvy, x, y;
    int ix, ix1, iy, iy1;
    int i, j;
    for (i = 1; i < winY - 1; i++) {
        for (j = 1; j < winX - 1; j++) {
            dvx = dx * arr2x[index(j, i)];
            dvy = dy * arr2y[index(j, i)];
            x = j - dvx;
            y = i - dvy;

            if (x < 0.5) x = 0.5;
            if (x > winX - 1.5) x = winX - 1.5;
            ix = int(x);
            ix1 = ix + 1;
            if (y < 0.5) y = 0.5;
            if (y > winY - 1.5) y = winY - 1.5;
            iy = int(y);
            iy1 = iy + 1;

            s1 = x - ix;
            s0 = 1 - s1;
            t1 = y - iy;
            t0 = 1 - t1;

            arr1x[index(j, i)] =
                s0 * (t0 * arr1y[index(ix, iy)] + t1 * arr1y[index(ix, iy1)]) +
                s1 * (t0 * arr1y[index(ix1, iy)] + t1 * arr1y[index(ix1, iy1)]);
        }
    }

    setBound(axis, arr1x);

}

void linearSolve(int b, float arrX[], float arrY[], float a, float c) {
    float cReciprocal = 1 / c;
    int iteration, x, y;
    for (iteration = 0; iteration < solveIterations; iteration++) {
        for (y = 1; y < winY - 1; y++) {
            for (x = 1; x < winX - 1; x++) {
                arrX[index(x, y)] =
                    (arrY[index(x, y)] +
                    a * (arrX[index(x + 1, y)] +
                    arrX[index(x - 1, y)] +
                    arrX[index(x, y + 1)] +
                    arrX[index(x, y - 1)])) * cReciprocal;
            }
        }
        setBound(b, arrX);
    }
}

void project(float velocityX1[], float velocityY1[], float velocityX2[], float velocityY2[]) {
    int x, y;
    for (y = 1; y < winY - 1; y++) {
        for (x = 1; x < winX - 1; x++) {
            velocityY2[index(x, y)] =
                (-0.5 * (velocityX1[index(x + 1, y)] -
                velocityX1[index(x - 1, y)] +
                velocityY1[index(x, y + 1)] -
                velocityY1[index(x, y - 1)])) / winY;
            velocityX2[index(x, y)] = 0;
        }
    }

    setBound(0, velocityY2);
    setBound(0, velocityX2);
    linearSolve(0, velocityX2, velocityY2, 1, 6);

    for (y = 1; y < winY - 1; y++) {
        for (x = 1; x < winX - 1; x++) {
            velocityX1[index(x, y)] -= 0.5 * (velocityX2[index(x + 1, y)] - velocityX2[index(x - 1, y)]) * winX;
            velocityY1[index(x, y)] -= 0.5 * (velocityX2[index(x, y + 1)] - velocityX2[index(x, y - 1)]) * winY;
        }
    }

    setBound(1, velocityX1);
    setBound(2, velocityY1);
}

void setBound(int axis, float arr[]) {
    for (int x = 1; x < winX - 1; x++) {
        arr[index(x, 0)] = axis == 2 ? -arr[index(x, 1)] : arr[index(x, 1)];
        arr[index(x, winY - 1)] = axis == 2 ? -arr[index(x, winY - 2)] : arr[index(x, winY - 2)];
    }
    for (int y = 1; y < winY - 1; y++) {
        arr[index(0, y)] = axis == 1 ? -arr[index(1, y)] : arr[index(1, y)];
        arr[index(winX - 1, y)] = axis == 1 ? -arr[index(winX - 2, y)] : arr[index(winX - 2, y)];
    }

    arr[index(0, 0)] = 0.5 * (arr[index(1, 0)] + arr[index(0, 1)]);
    arr[index(0, winY - 1)] = 0.5 * (arr[index(1, winY - 1)] + arr[index(0, winY - 2)]);
    arr[index(winX - 1, 0)] = 0.5 * (arr[index(winX - 2, 0)] + arr[index(winX - 1, 1)]);
    arr[index(winX - 1, winY - 1)] = 0.5 * (arr[index(winX - 2, winY - 1)] + arr[index(winX - 1, winY - 2)]);
}