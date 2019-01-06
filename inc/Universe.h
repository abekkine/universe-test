#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <vector>
#include <noise.h>

class Universe {
public:
    struct Position {
        double x, y;
        double dx, dy;
        float value;
    };
public:
    Universe();
    ~Universe();
    void GetPoints(double centerX, double centerY, std::vector<Position> & points);

private:
    // noise::module::Perlin m_noise;
    noise::module::Billow m_noise;
    // noise::module::RidgedMulti m_noise;
    std::vector<Position> m_points;
};

#endif // UNIVERSE_H
