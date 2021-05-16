#pragma once

/**
 * https://mrl.cs.nyu.edu/~perlin/noise/
 */

class PerlinNoise {
private:
   int p[512];

   double fade(double);
   double lerp(double, double, double);
   double grad(int, double, double, double);

public:
   PerlinNoise();

   double noise(double, double, double);
};

