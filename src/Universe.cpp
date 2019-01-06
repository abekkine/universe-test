#include <Universe.h>

#include <math.h>

Universe::Universe() {
    m_noise.SetOctaveCount(2);
    m_noise.SetFrequency(0.25);
    // m_noise.SetPersistence(0.02);
}

Universe::~Universe() {}

void Universe::GetPoints(double centerX, double centerY, std::vector<Position> & points) {

    const double ds = 1.0;

    m_points.clear();
    for (double dx=-5.0; dx<5.0; dx+=ds) {
        for (double dy=-5.0; dy<5.0; dy+=ds) {
            Position p;
            p.x = floor(centerX/ds)*ds + dx;
            p.y = floor(centerY/ds)*ds + dy;
            p.value = m_noise.GetValue(p.x, p.y, 0.0);
            p.dx = ds * m_noise.GetValue(p.x, p.y, 13.05);
            p.dy = ds * m_noise.GetValue(p.x, p.y, -19.103);
            m_points.push_back(p);
        }
    }

    points = m_points;
}
