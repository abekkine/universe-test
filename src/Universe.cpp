#include <Universe.h>

#include <math.h>

#include <iostream>

Universe::Universe() {
    m_noise.SetOctaveCount(2);
    m_noise.SetFrequency(0.25);
    // m_noise.SetPersistence(0.02);
}

Universe::~Universe() {}

void Universe::GetStars(double centerX, double centerY, std::vector<StarInfo> & stars) {

    const double ds = 0.2;//1.0;
    const double frame_size = 15.0;

    m_stars.clear();
    int category_index;
    for (double dx=-frame_size; dx<frame_size; dx+=ds) {
        for (double dy=-frame_size; dy<frame_size; dy+=ds) {
            double x = floor(centerX/ds)*ds + dx;
            double y = floor(centerY/ds)*ds + dy;
            double value = 0.5 * (1.0 + m_noise.GetValue(x, y, 0.0));
            if (value > 0.2) {
                StarInfo p;
                p.x = x;
                p.y = y;
                // DEBUG
                p.value = value;
                // p.value = (value - 0.5) * 2.0;
                // category_index = GetCategoryIndex(p.value);
                // p.color_ptr = &star_categories_[category_index][e_COLOR_INDEX];
                p.dx = ds * m_noise.GetValue(p.x, p.y, 13.05);
                p.dy = ds * m_noise.GetValue(p.x, p.y, -19.103);
                m_stars.push_back(p);
            }
        }
    }

    stars = m_stars;
}

int Universe::GetCategoryIndex(double value) {
    int ix = e_NUM_CATEGORIES-1;
    for (int i=0; i<e_NUM_CATEGORIES-1; ++i) {
        if (value < star_categories_[i][e_PROB_INDEX]) {
            ix = i;
            break;
        }
    }
    return ix;
}
