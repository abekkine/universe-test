#include "Universe.h"

#include <math.h>

#include <iostream>

Universe::Universe() {

    m_noise.SetOctaveCount(m_params.octaveCount);
    m_noise.SetFrequency(m_params.frequency);
    // m_noise.SetPersistence(0.02);
}

Universe::~Universe() {}

void Universe::setOctaveCount(const double & value) {
    m_params.octaveCount = 1 + static_cast<int>(7.0 * value);
    m_noise.SetOctaveCount(m_params.octaveCount);
}
double Universe::getOctaveCount() {
    return ((double)m_params.octaveCount - 1.0) / 7.0;
}

void Universe::setFrequency(const double & value) {
    m_params.frequency = (value * 15.9) + 0.1;
    m_noise.SetFrequency(m_params.frequency);
}
double Universe::getFrequency() {
    return ((m_params.frequency - 0.1) / 15.9);
}

void Universe::setStepSize(const double & value) {
    m_params.stepSize = 0.1 + value * 1.9;
}
double Universe::getStepSize() {
    return ((m_params.stepSize - 0.1) / 1.9);
}

void Universe::setXPosition(const double & value) {
    m_params.x = value * 40.0 - 20.0;
}
double Universe::getXPosition() {
    return ((m_params.x + 20.0) / 40.0);
}

void Universe::setYPosition(const double & value) {
    m_params.y = value * 40.0 - 20.0;
}
double Universe::getYPosition() {
    return ((m_params.y + 20.0) / 40.0);
}

void Universe::setMinValue(const double & value) {
    m_params.minValue = value * 0.9;
}
double Universe::getMinValue() {
    return (m_params.minValue / 0.9);
}

void Universe::GetStars(double centerX, double centerY, std::vector<StarInfo> & stars) {

    const double ds = m_params.stepSize; // 0.2;//1.0;
    const double frame_size = 15.0;

    m_stars.clear();
    int category_index;
    for (double dx=-frame_size; dx<frame_size; dx+=ds) {
        for (double dy=-frame_size; dy<frame_size; dy+=ds) {
            double x = floor(centerX/ds)*ds + dx;
            double y = floor(centerY/ds)*ds + dy;
            // double value = 0.5 * (1.0 + m_noise.GetValue(x, y, 0.0));
            double value = 0.5 + m_noise.GetValue(x, y, 0.0);
            {
                static double min_v = 1000.0;
                static double max_v = -1000.0;
                if (value > max_v) {
                    max_v = value;
                    // std::cout << "MAX(" << max_v << "), ";
                    // std::cout << "MIN(" << min_v << ")\n";
                }
                else if (value < min_v) {
                    min_v = value;
                    // std::cout << "MAX(" << max_v << "), ";
                    // std::cout << "MIN(" << min_v << ")\n";
                }
            }
            if (value > m_params.minValue) { // 0.2) {
                StarInfo p;
                p.x = x;
                p.y = y;
                // DEBUG
                p.value = value;
                // p.value = (value - 0.5) * 2.0;
                category_index = GetCategoryIndex(p.value);
                p.color_ptr = &star_categories_[category_index][e_COLOR_INDEX];
                p.dx = ds * m_noise.GetValue(p.x, p.y, m_params.x);
                p.dy = ds * m_noise.GetValue(p.x, p.y, m_params.y);
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
